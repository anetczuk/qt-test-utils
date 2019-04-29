TEMPLATE = app
TARGET = tests

QT += qml quick widgets testlib

include(../common.pri)

CONFIG += c++11


#RESOURCES += src/eclipse-qt.qrc

#qml.files = src/eclipse-qt.qml


HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)


# include library
linkStaticLibrary(testutils)
linkStaticLibrary(alib, aStaticLib)
