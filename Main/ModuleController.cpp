#include "ModuleController.h"
#include "InteropStream.h"

#include "UniversalSensors.h"
#include "AlertModule.h"
//--------------------------------------------------------------------------------------------------------------------------------------
PublishStruct PublishSingleton;
ModuleController* MainController = NULL;
SdFat SDFat;
//--------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::RemoveFiles(const String& dirName, bool recursive)
{
  const char* dirP = dirName.c_str();
  
  if(!SDFat.exists(dirP))
  {
    return;
  }

  SdFile root;
  if(!root.open(dirP,FILE_READ))
  {
    return;
  }

  root.rewind();


  SdFile entry;
  while(entry.openNext(&root,FILE_READ))
  {
    
    if(entry.isDir())
    {
      if(recursive)
      {
        String subPath = dirName + "/";
        subPath += FileUtils::GetFileName(entry);        
        FileUtils::RemoveFiles(subPath,recursive);

        if(!SDFat.rmdir(subPath.c_str()))
        {
        }
        else
        {
          entry.close();
        }
      }
    }
    else
    {
     String subPath = dirName + "/" + FileUtils::GetFileName(entry);

      if(!SDFat.remove(subPath.c_str()))
      {
      }
      else
      {
        entry.close();
      }
    }
  } // while


  root.close();
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
int FileUtils::CountFiles(const String& dirName, bool recursive)
{
  int result = 0;
  const char* dirP = dirName.c_str();
  
  if(!SDFat.exists(dirP))
    return result;

  SdFile root;
  if(!root.open(dirP,O_READ))
    return result;

  root.rewind();

  SdFile entry;
  while(entry.openNext(&root,O_READ))
  {
    if(entry.isDir())
    {
      if(recursive)
      {
        String subPath = dirName + "/";
        subPath += FileUtils::GetFileName(entry);
        result += FileUtils::CountFiles(subPath,recursive);      
      }
    }
    else
    {      
      result++;
    }
    entry.close();
  } // while


  root.close();
  return result;

}
//--------------------------------------------------------------------------------------------------------------------------------------
String FileUtils::GetFileName(SdFile& f)
{
      char nameBuff[50] = {0};
      f.getName(nameBuff,50);
      return nameBuff;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void FileUtils::readLine(SdFile& f, String& result)
{
  if(!f.isOpen())
    return;
    
    while(1)
    {
      int iCh = f.read();
      
      if(iCh == -1)
        break;

      char ch = (char) iCh;

      if(ch == '\r')
        continue;

      if(ch == '\n')
        break;

      result += ch;
    }  
}
//--------------------------------------------------------------------------------------------------------------------------------
ModuleController::ModuleController() : cParser(NULL)
#ifdef USE_LOG_MODULE
,logWriter(NULL)
#endif
{
  reservationResolver = NULL;
  httpQueryProviders[0] = NULL;
  httpQueryProviders[1] = NULL;
  PublishSingleton.Text.reserve(SHARED_BUFFER_LENGTH); // 500 байт для ответа от модуля должно хватить.
}
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_DS3231_REALTIME_CLOCK
DS3231Clock& ModuleController::GetClock()
{
  return _rtc;
}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::begin()
{
 // тут можно написать код, который выполнится непосредственно перед началом работы
 
 UniDispatcher.Setup(); // настраиваем диспетчера универсальных датчиков

 #ifdef USE_FEEDBACK_MANAGER
 FeedbackManager.Setup();
 #endif
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
OneState* ModuleController::GetReservedState(AbstractModule* sourceModule, ModuleStates sensorType, uint8_t sensorIndex)
{
  if(!reservationResolver)
    return NULL;

  return reservationResolver->GetReservedState(sourceModule,sensorType, sensorIndex);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Setup()
{  
  MainController = this;

#ifdef USE_DS3231_REALTIME_CLOCK
_rtc.begin();
#endif

#if  defined(USE_WIFI_MODULE) || defined(USE_LOG_MODULE) || defined(USE_SMS_MODULE)|| (defined(SENSORS_SETTINGS_ON_SD_ENABLED) && defined(USE_LCD_MODULE))

  pinMode(SDCARD_CS_PIN,OUTPUT);
  digitalWrite(SDCARD_CS_PIN,HIGH);

 #if TARGET_BOARD == DUE_BOARD
  delay(400);
  for(int i=0;i<5;i++)
  {
      sdCardInitFlag = SDFat.begin(SDCARD_CS_PIN, SPI_HALF_SPEED); // пробуем инициализировать SD-модуль
      if(sdCardInitFlag)
        break;
        
    delay(500);
  }
 #else
    sdCardInitFlag = SDFat.begin(SDCARD_CS_PIN); // пробуем инициализировать SD-модуль
 #endif
 
  WORK_STATUS.PinMode(SDCARD_CS_PIN,OUTPUT,false);
  WORK_STATUS.PinMode(MOSI,OUTPUT,false);
  WORK_STATUS.PinMode(MISO,INPUT,false);
  WORK_STATUS.PinMode(SCK,OUTPUT,false);
#endif

  #if defined(USE_MCP23S17_EXTENDER) && COUNT_OF_MCP23S17_EXTENDERS > 0
    WORK_STATUS.InitMcpSPIExtenders();
  #endif
  #if defined(USE_MCP23017_EXTENDER) && COUNT_OF_MCP23017_EXTENDERS > 0
    WORK_STATUS.InitMcpI2CExtenders();
  #endif 
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Log(AbstractModule* mod, const String& message)
{
 #ifdef USE_LOG_MODULE
  if(!logWriter)
    return;

  LogAction la;
  la.RaisedModule = mod;
  la.Message = message;
  logWriter->WriteAction(la); // пишем в лог
 #else
  UNUSED(mod);
  UNUSED(message);
 #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::RegisterModule(AbstractModule* mod)
{
  if(mod)
  {
    mod->Setup(); // настраиваем
    modules.push_back(mod);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::PublishToCommandStream(AbstractModule* module,const Command& sourceCommand)
{

  Stream* ps = sourceCommand.GetIncomingStream();
 
  // Публикуем в переданный стрим
  if(!ps)
  {
#ifdef _DEBUG
  if(PublishSingleton.Text.length())
    Serial.println(String(F("No ps, but have answer: ")) + PublishSingleton.Text);
#endif    
    PublishSingleton.Flags.Busy = false; // освобождаем структуру
    return;
  }

     ps->print(PublishSingleton.Flags.Status ? OK_ANSWER : ERR_ANSWER);
     ps->print(COMMAND_DELIMITER);

    if(PublishSingleton.Flags.AddModuleIDToAnswer && module) // надо добавить имя модуля в ответ
    {
       ps->print(module->GetID());
       ps->print(PARAM_DELIMITER);
    }
    
     ps->println(PublishSingleton.Text);

   
   PublishSingleton.Flags.Busy = false; // освобождаем структуру
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::CallRemoteModuleCommand(AbstractModule* mod, const String& command)
{

  UNUSED(mod);
  UNUSED(command);
  
#ifdef _DEBUG  
  Serial.println("BROADCAST THE COMMAND \"" + command + "\"");
#endif
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Publish(AbstractModule* module,const Command& sourceCommand)
{

  PublishToCommandStream(module,sourceCommand);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
AbstractModule* ModuleController::GetModuleByID(const String& id)
{
  size_t sz = modules.size();
  for(size_t i=0;i<sz;i++)
  { 
    AbstractModule* mod = modules[i];
    if(!strcmp(mod->GetID(),id.c_str()) )
      return mod;
  } // for
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::ProcessModuleCommand(const Command& c, AbstractModule* mod)
{

#ifdef _DEBUG
///Serial.println("called: " +  c.GetTargetModuleID() + PARAM_DELIMITER + c.GetRawArguments());
#endif  

if(!mod) // ничего не передали, надо искать модуль
  mod =  GetModuleByID(c.GetTargetModuleID());
  
 if(!mod)
 {
  // Сообщаем в тот поток, откуда пришел запрос.
  PublishSingleton.Flags.AddModuleIDToAnswer = false;
  PublishSingleton.Flags.Status = false;
  PublishSingleton = UNKNOWN_MODULE;
  PublishToCommandStream(mod,c);
  return;
 }
     // нашли модуль

CHECK_PUBLISH_CONSISTENCY;

 PublishSingleton.Reset(); // очищаем структуру для публикации
 PublishSingleton.Flags.Busy = true; // говорим, что структура занята для публикации
 mod->ExecCommand(c,true);//c.GetIncomingStream() != NULL); // выполняем его команду
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::Alarm(AlertRule* rule)
{
  #ifdef USE_ALARM_DISPATCHER
    alarmDispatcher.Alarm(rule);
  #else
    UNUSED(rule);
  #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ModuleController::UpdateModules(uint16_t dt, CallbackUpdateFunc func)
{  
  
 #ifdef USE_FEEDBACK_MANAGER
 FeedbackManager.Update(dt); // обновляем состояние менеджера обратной связи
 #endif
  
  size_t sz = modules.size();
  for(size_t i=0;i<sz;i++)
  { 
    AbstractModule* mod = modules[i];
   
      // ОБНОВЛЯЕМ СОСТОЯНИЕ МОДУЛЕЙ
      mod->Update(dt);

    if(func) // вызываем функцию после обновления каждого модуля
      func(mod);
  
  } // for
}
//--------------------------------------------------------------------------------------------------------------------------------------

