##
## This file is automatically generated from template "/media/L3/home/bob/Stuff/cpp/qt-test-utils/src/testutils-subdirtests/prjtest.pro.template".
## Any changes will be lost.
##

TEMPLATE = app
TARGET = testrunner

QT += qml quick widgets testlib

CONFIG += c++11 testcase qmltestcase


include( "/media/L3/home/bob/Stuff/cpp/qt-test-utils/src/common.pri" )


DEFINES += EXEC_PER_TESTCASE


SOURCES += "/media/L3/home/bob/Stuff/cpp/qt-test-utils/src/testutils-quicktests/src/tst_SetupExample.cpp"


# include library
linkStaticLibrary(testutils)
