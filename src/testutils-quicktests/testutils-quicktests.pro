## MIT License
##
## Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in all
## copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.
##

TEMPLATE = app
TARGET = testutilsquicktests

QT += qml quick widgets testlib

CONFIG += c++11 qmltestcase


include( ../common.pri )


HEADERS += $$files(src/*.h, true)

SOURCES += $$files(src/*.cpp, true)

OTHER_FILES += $$files(*.qml, true)
OTHER_FILES += $$files(data/*.*, true)

#QML_FILES = $$files(qml/*.qml, true)
#GEN_RES_PATH = "gen/$$PROJECT_NAME""res.qrc"
#generateResFile($$GEN_RES_PATH, $$QML_FILES, "../")


## prepare data
include( testutils-quicktests-data.pri )


# include library
include( ../testutils/testutilsLink.pri )
