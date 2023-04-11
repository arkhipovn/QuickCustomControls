Чтобы начать пользоваться необходимо добавить pri файл модуля
include(QuickCustomControls/qcc.pri)

После создания QQmlApplicationEngine нужно зарегистрировать модуль
Для этого нужно вызвать функцию QCC::registerTypes(engine);


Есть модуль, который не использует qml.
include("QuickCustomControls/hc.pri")
В нем сейчас только класс для работы с переводами, но он будет пополнятся вспомогательными классами




Модули QCC:

# Элементы управления
QCC.Controls

# наборы шрифтов
QCC.Fonts
Сейчас есть только один. awesome
Можно получить следющим образом: CFontLoader.awesome

# Построение графика
QCC.DataView.ChartView 1.0

# Построение таблицы
QCC.DataView.TableView 1.0

# Валидаторы
QCC.Validators
