##
## This file is automatically generated from template "/media/L3/home/bob/Stuff/cpp/qt-test-utils/src/autosubdirs/prjtest.pro.template".
## Any changes will be lost.
##

TEMPLATE = app
TARGET = testrunner

QT += qml quick widgets testlib

CONFIG += c++11 testcase qmltestcase


include( $$SOURCE_ROOT_DIR/common.pri )


DEFINES += EXEC_PER_TESTCASE


SOURCES += "../../testutils-qttests/src/tst_QmlUnitRunner.cpp"


# include library
include( $$SOURCE_ROOT_DIR/testutils/testutilsLink.pri )

