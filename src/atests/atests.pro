TEMPLATE = app
TARGET = tests

QT += qml quick testlib

include(../common.pri)

CONFIG += c++11


#RESOURCES += src/eclipse-qt.qrc

#qml.files = src/eclipse-qt.qml


SOURCES += $$files(src/*.cpp, true)


# include library
linkStaticLibrary(testutils)
linkStaticLibrary(alib, aStaticLib)
