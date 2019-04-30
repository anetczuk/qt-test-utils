TEMPLATE = app
TARGET = tests

QT += qml quick widgets testlib

include(../common.pri)

CONFIG += c++11 qmltestcase


HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)

RESOURCES += $$files(qml/*.qml, true)


# include library
linkStaticLibrary(testutils)
