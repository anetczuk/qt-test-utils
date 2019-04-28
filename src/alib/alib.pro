TEMPLATE = lib
TARGET = aStaticLib

QT += qml quick

include(../common.pri)

CONFIG += c++11 staticlib


qml.files = $$files(res/*.qml, true)

RESOURCES += res/alibres.qrc \
             $$files(res/*.jpeg, true) \
             $$files(res/*.png, true)

HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)
