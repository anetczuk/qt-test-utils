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


TARGET_NAME = testutils

LIB_DIR = $$BUILD_ROOT_DIR/$$TARGET_NAME/$$BUILD_SUBDIR
LIB_FILE_NAME = $$staticLibName( $$TARGET_NAME )
LIB_PATH = $$LIB_DIR/$$LIB_FILE_NAME

# link library
LIBS += -L$$LIB_DIR -l$$TARGET_NAME
# add library headers directory to search path
INCLUDEPATH += $$SOURCE_ROOT_DIR/$$TARGET_NAME/src
# configure dependency - causes target to rebuild when library changes (otherwise it causes nasty problems, at least under qmake 3.1)
PRE_TARGETDEPS += $$LIB_PATH
