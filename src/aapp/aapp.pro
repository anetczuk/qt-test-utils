TEMPLATE = app
TARGET = aproject

QT += qml quick gui

include(../common.pri)

CONFIG += c++11


HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)


# include library
linkStaticLibrary(alib, aStaticLib)
