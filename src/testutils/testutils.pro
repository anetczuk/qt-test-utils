TEMPLATE = lib
TARGET = testutils

QT += qml quick testlib

include(../common.pri)

CONFIG += c++11 staticlib


#RESOURCES += src/eclipse-qt.qrc

#qml.files = src/eclipse-qt.qml

HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)
