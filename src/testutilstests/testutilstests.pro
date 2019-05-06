TEMPLATE = app
TARGET = tests

QT += qml quick widgets testlib

include(../common.pri)

CONFIG += c++11 qmltestcase


HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)

QML_FILES = $$files(qml/*.qml, true)
##RESOURCES += $$QML_FILES
GEN_RES_PATH = gen/res.qrc
generateResFile($$GEN_RES_PATH, $$QML_FILES, "../")
RESOURCES += $$GEN_RES_PATH


## copy data dir
copydata.commands = $(COPY_DIR) $$PWD/data $$OUT_PWD
QMAKE_EXTRA_TARGETS += copydata
PRE_TARGETDEPS += copydata


# include library
linkStaticLibrary(testutils)
