# Контроллер теплицы на Arduino Mega
<p>
Когда возник вопрос - <a href="https://github.com/Porokhnya/GreenhouseProject/wiki">здесь уже может быть половина ответа</a>. Вторая половина ответа, как правило, <a href="https://www.forumhouse.ru/threads/367227">расположена тут</a>.
# ВНИМАНИЕ!
Перед закачиванием прошивки в контроллер её необходимо настроить, все настройки в файле Globals.h! Я тестирую и выкладываю прошивку практически <b>в 
максимальной конфигурации</b>, следовательно, вы должны исключить из прошивки те модули, которым нет соответствия в виде подключённых железных 
"ответок". Скажем, если у вас нет модуля SD-карты, то совершенно недопустимо включать в прошивку модуль логгирования информации (`USE_LOG_MODULE`) 
и модуль Wi-Fi (`USE_WIFI_MODULE`). Также хочу обратить внимание на то, что некоторые модули для своей нормальной работы требуют подключённого 
железного соратника DS3231, т.к. без модуля реального времени их штатная работа невозможна. При любых затруднениях в работе (прошивка не работает как надо,
почему-то ничего не получается, что-то не выводится на экран) - прежде всего смотрите настройки прошивки и схему подключения. Подчёркиваю - предложенная 
прошивка не готовое решение, а конструктор, который вы можете настроить под свои нужды. И, как любое модульное изделие, требует вдумчивого подхода перед 
началом использования.
<p>
# Лицензия
Проект свободен для <b>некоммерческого использования</b> и исключительно в личных целях. Любое другое использование, например, встраивание любой части кода в коммерческий продукт, распространение любой части исходных кодов на сторонних ресурсах без разрешения автора проекта - не допускается. Все права интеллектуальной собственности на любую программную часть продукта принадлежат их авторам, использование программного кода в сторонних проектах без согласия авторов и упоминания об исходном авторстве - не допускается.

Автор проекта оставляет за собой право в любой момент пересмотреть лицензию распространения продукта.

# Структура архива
<ul>
<li>В папке <b>Main</b> - исходники прошивки для Меги;</li>
<li>В папке <b>SOFT</b> - текущая версия конфигуратора, коннектится к Меге по COM-порту;</li>
<li>В папке <b>Libraries</b> - сторонние библиотеки, искользуемые в проекте (их количество неуклонно приближается к нулю, но пока - как есть);</li>
<li>В папке <b>SD</b> - файлы, которые надо закачать на SD-карту;</li>
<li><b>arduino-1.6.7-windows.exe</b> - версия Arduino IDE, используемая в проекте;</li>
<li>В папке <b>CHANGED_IDE_FILES</b> - файлы, которые надо заменить, переписав стандартные, из поставки Arduino IDE;</li>
<li>Файл <b>NewPlan.spl7</b> - файл схемы для программы SPlan 7.0;</li>
<li>В папке <b>Nextion</b> - файл прошивки для дисплея Nextion 320x240, и проект прошивки;</li>
<li>В папке <b>Nextion1WireModule</b> - прошивка универсального исполнительного модуля с дисплеем Nextion, работающего по шине 1-Wire;</li>
<li>В папке <b>UniversalSensorsModule</b> - прошивка универсального модуля с поддержкой до трёх датчиков разных типов, работающего по шине 1-Wire и по шине RS-485;</li>
<li>В папке <b>UniversalExecutionModule</b> - прошивка универсального исполнительного модуля на 8 слотов настраиваемых привязок, работающего по шине 1-Wire и по шине RS-485;</li>
<li>В папке <b>WEB</b> - текущая версия вебморды, ставится под любой веб-сервер с поддержкой PHP и sqlite3.</li>
</ul>

# Настройки прошивки по умолчанию

Выкладываемая прошивка - это слепок того, что я делаю на текущий момент. Поэтому её настройки по умолчанию могут сильно разниться с теми,
которые нужны вам. Примерная конфигурация, которая собрана у меня на макетке на текущий момент:

<details> 
  <summary style='cursor:pointer;margin-bottom:10px;'>Кликните, чтобы посмотреть</summary>

  * `USE_UNIVERSAL_SENSORS` - модуль поддержки универсальных модулей (в разработке, подключен эмулятор на Uno);
  * `USE_UNI_NEXTION_MODULE` - модуль поддержки дисплея Nextion по шине 1-Wire;
  * `USE_DS3231_REALTIME_CLOCK` - подключён модуль часов реального времени DS3231;
  * `USE_TEMP_SENSORS` - модуль хранения информации о температуре и управления фрамугами, подключены DS18B20 и восьмиканальный модуль реле;
  * `USE_WINDOWS_SHIFT_REGISTER` - управление фрамугами идёт через сдвиговый регистр 74HC595;
  * `USE_WATERING_MODULE` - модуль управления поливом, подключен железный модуль реле на два канала;
  * `USE_LUMINOSITY_MODULE` - модуль досветки, подключёны два датчика BH1750, требуется железный модуль реле на один канал;
  * `USE_HUMIDITY_MODULE` - модуль влажности, подключены DHT22 и Si7021;
  * `USE_SOIL_MOISTURE_MODULE` - модуль влажности почвы, пока подключён китайский влагомер;
  * `USE_LOG_MODULE` - модуль логгирования информации, подключен железный SD-модуль;
  * `USE_TIMER_MODULE` - модуль таймеров (4 таймера периодических операций);
  * `USE_RESERVATION_MODULE` - модуль резервирования показаний датчиков (когда датчики одной группы дублируют друг друга);
  * `USE_WATERFLOW_MODULE` - модуль расхода воды, подключён китайский расходомер;
  * `USE_WIFI_MODULE` - модуль доступа по WI-FI, подключена ESP8266, требует подключение SD-модуля;
  * `USE_LCD_MODULE` - модуль экрана, подключён графический LCD 128x64 на контроллере ST7920, а также тактовая кнопка и энкодер;
  * Определено ещё кучу всяких `USE_*`, поэтому, прошу вас - будьте внимательны с настройками прошивки, ибо перечислять их здесь все - анрил полный;
  * Конвертеры уровней, DC-DC-преобразователи - подключены там, где это требуется;
  * Neoway M590 - не подключен, директива `USE_SMS_MODULE` закомментирована;
  * Модули реле у меня включаются по низкому уровню;
  * К Меге у меня подключен как минимум один универсальный модуль, для тестирования работы по 1-Wire и RS-485.
</details>

Настройки прошивки по умолчанию могут меняться с обновлениями, поэтому будьте внимательны, и проверяйте, пожалуйста,
соответствие настроек прошивки вашим чаяниям. Обращаю внимание, что прошивку можно настроить и на другой экран (Nextion), и
на другой шлюз в локальную сеть (W5100), можно менять кол-во датчиков, номера пинов для них, можно управлять пинами реле фрамуг 
напрямую, а не через сдвиговый регистр - всё это шаманство вы можете совершить, чтобы подогнать прошивку под ваши требования. Я 
лишь даю инструмент, которым можно (и нужно) воспользоваться с умом.



<br/>
# Скриншоты конфигуратора

<details> 
  <summary style='cursor:pointer;margin-bottom:10px;'>Кликните, чтобы посмотреть</summary>
  
<img src="screen1.png" hspace='10'/>
<img src="screen2.png" hspace='10'/>
<img src="screen3.png" hspace='10'/>
<img src="screen4.png" hspace='10'/>
<img src="screen5.png" hspace='10'/>
<img src="screen6.png" hspace='10'/>
<img src="screen7.png" hspace='10'/>
<img src="screen8.png" hspace='10'/>
<img src="screen9.png" hspace='10'/>
<img src="screen10.png" hspace='10'/>
<img src="screen11.png" hspace='10'/>

</details>

<p>
# Как использовать
<p>
<b>Установить библиотеки (в папке Libraries архива) в среду Arduino IDE!</b>
<p>
<b>Все настройки прошивки перед компиляцией - в файле `Globals.h`!</b>
<p>
Распаковать архив в папку на диске, установить библиотеки из папки <b>Libraries</b> архива в среду Arduino IDE. Затем открыть в Arduino IDE файл <b>Main.ino</b>, настроить директивы условной компиляции (файл `Globals.h`), скомпилировать прошивку и закачать в Мегу. Подсоединить железки в зависимости от выбранных настроек прошивки. <b>Если используется модуль Wi-Fi или модуль логгирования информации - подключить модуль SD-карты и закачать файлы из папки SD в корень SD-карточки!</b>
<p>
Открыть конфигуратор и подключиться к COM-порту, на котором висит Мега, и начать общение с контроллером. Принципиальная схема подключения некоторого железного добра указана ниже:<br/>
<img src="plan.png"/>

<h2>Модуль LCD 128x64 на базе контроллера ST7920</h2>

В текущей версии используется LCD-экран 128х64 на базе контроллера ST7920, подключенный в режиме последовательного соединения. Подробности подключения - в файле `Globals.h`. 
Если вам не нужно использование этого модуля - закомментируйте `USE_LCD_MODULE` в файле `Globals.h`.
<p><b>Внимание!</b> Для того, чтобы использовать указанный экран - необходимо установить библиотеку `U8GLib` (есть в архиве проекта) в среду Arduino IDE!
<p> Экран работает совместно с энкодером и тактовой кнопкой (подробности подключения - в `Globals.h`). Энкодером перелистываются закладки, затем кнопкой - производится переход внутрь выбранной закладки и пролистывание вариантов настроек. На экране ожидания нажатия кнопки пролистывают показания с датчиков, не дожидаясь времени их программного пролистывания. На экране ожидания можно вывести информацию со сколь угодно большого количества датчиков (настраивается в `Globals.h`).

<h2>Модуль поддержки дисплеев Nextion</h2>

В текущей версии используется модуль поддержки дисплеев Nextion (320x240). Если вам не нужно использование этого модуля - закомментируйте `USE_NEXTION_MODULE` в файле `Globals.h`. Прошивка для дисплея и её исходники - лежат в папке Nextion. Подробности по закачиванию прошивки в дисплей - в файле Nextion\320x240\Readme.txt.

<h2>Модуль датчиков расхода воды</h2>

В текущей версии используется модуль датчиков расхода воды на эффекте Холла (трёхпроводные с Али, искать по словам "water flow meter", они обычно около 300 рублей стоимостью). Если вам не нужно использование этого модуля - закомментируйте `USE_WATERFLOW_MODULE` в файле `Globals.h`. Датчиков может быть до двух, сидят они на пинах `2` и `3`. Подробные настройки - в `Globals.h`.


<h2>Модуль часов реального времени DS3231</h2>

В текущей версии используется модуль часов реального времени на микросхеме DS3231 (пин 20 - SDA, пин 21 - SCL). Если вам не нужно использование этого модуля - закомментируйте `USE_DS3231_REALTIME_CLOCK` в файле `Globals.h`.

<h2>GSM-модуль Neoway M590</h2>

В текущей версии используется GSM-модуль на базе чипа NEOWAY M590. Если вы не хотите использовать управление по SMS - закомментируйте `USE_SMS_MODULE` в файле `Globals.h`. 
Если у вас возникли проблемы с работой SMS-модуля - можно включить отладочный режим (<b>не работает совместно с конфигуратором!</b>), раскомментировав `GSM_DEBUG_MODE` в файле `Globals.h`. 

<p>
Список команд для управления по SMS - смотрите в конфигураторе. Если вы посылаете команды на открытие или закрытие окон, включение/выключение полива или досветки - контроллер автоматически переходит в ручной режим работы!

<h2>Модуль Wi-Fi ESP8266</h2>

В текущей версии используется WI-FI-модуль на основе чипа ESP8266. Если вы не хотите использовать WI-FI в проекте - закомментируйте `USE_WIFI_MODULE` в файле `Globals.h`.
Если у вас возникли проблемы с работой WI-FI-модуля - можно включить отладочный режим (<b>не работает совместно с конфигуратором!</b>), раскомментировав `WIFI_DEBUG` в файле `Globals.h`. Модуль ESP8266 соединён с Serial2 Меги (пины `16` и `17`, см. схему выше).
<p>
В текущей редакции реализован мост UART-TCP/IP: соединившись с IP, которое выдал роутер модулю ESP - можно посылать команды контроллеру так же, как если бы это было посредством соединения через UART, т.е.: любая команда начинается с `CTGET=` или `CTSET=`, и заканчивается переводом строки \r\n.
<p><b>Работа с ESP протестирована на версии AT-прошивки 0.40, работоспособность на других прошивках не гарантируется!</b>

<h2>Модуль управления поливом</h2>

<b>ВНИМАНИЕ!</b> Реле, используемые в текущей версии прошивки, включаются по низкому уровню. Если у вас наоборот - смотрите настройки `RELAY_ON` и `RELAY_OFF` в файле `Globals. h`.
<p>
Для индикации ручного режима управления поливом на пин 8 выведен светодиод (см. схему), который будет мигать при переключении в ручной режим работы. Для управления каналами реле задействованы пины 22, 23, 24, с которых идёт управление тремя каналами реле. Можно рулить каким угодно количеством реле, вплоть до 8. Для изменения кол-ва реле в файле `Globals.h` установите `WATER_RELAYS_COUNT` в нужное число - от 1 до 8. Пины прописываются в `WATER_RELAYS_PINS`, каждый пин указывается через запятую, их общее количество должно быть равным количеству, указанному в настройке `WATER_RELAYS_COUNT`!
<p>
При подаче любой команды модулю управления поливом (начать/закончить полив, а также когда настройка "Автоматическое управление поливом" в положении "выключено") - контроллер переходит в ручной режим управления поливом, при этом мигает светодиод на пине 8. Номер пина для диода можно изменить с помощью настройки `DIODE_WATERING_MANUAL_MODE_PIN` в файле `Globals.h`.

<h2>Модуль освещенности BH1750</h2>

В текущей версии используется модуль освещенности на базе чипа BH1750. Если вам не нужно использование этого модуля - закомментируйте `USE_LUMINOSITY_MODULE` в файле `Globals.h`. Схема подключения модуля освещенности приведена выше.

<h2>Модуль влажности</h2>

В текущей версии используется модуль опроса датчиков влажности на базе различных чипов. Поддерживаются следующие типа: DHT - DHT11 и старше, Si7021. Для добавления датчиков смотрите примеры `ADD_HUMIDITY_SENSOR` в файле `Globals.h`. Если вам не нужно использование этого модуля - закомментируйте  `USE_HUMIDITY_MODULE` в файле `Globals.h`.

<h2>Другие модули</h2>
На самом деле модулей - вагон и тележка, и расписать их все здесь - не представляется возможным, т.к. прошивка постоянно усовершенствуется. Так что, если у  вас возникли какие-либо вопросы, добро пожаловать: https://www.forumhouse.ru/threads/341712 - в этой теме я бываю регулярно и готов ответить на все ваши вопросы.

# Конфигуратор

Конфигуратор автоматически подцепляет прошитые в контроллер модули, и показывает вкладки управления этими модулями, после соединения с контроллером. Если вы не видите вкладку управления поливом (например) - смотрите директивы условной компиляции на предмет того, выключен ли модуль из компиляции или нет. При перезагрузке контроллера он переходит в автоматический режим работы, даже если при последнем обращении к нему был выставлен ручной режим.
