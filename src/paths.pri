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


defineReplace(fileName) {
    path = $$1
    base_name = $$basename(path)
    base_name_parts = $$split(base_name, ".")
    FILE_NAME = $$first(base_name_parts)
    return ($$FILE_NAME)
}

!linux {
    defineReplace(realPath) {
        ## do nothing
        return ($$1)
    }
    defineReplace(shadowedRootPath) {
        path = $$shadowed($$SOURCE_ROOT_DIR )
        return ($$path)
    }
} else {
    defineReplace(realPath) {
        path = $$system(realpath $$1)
        return ($$path)
    }
    defineReplace(shadowedRootPath) {
        ## "shadowed" function is broken under Linux -- it returns invalid result in case of symlinks
        REAL_OUT_PWD = $$realPath( $$OUT_PWD )
        REL_PATH = $$relative_path( $$REAL_OUT_PWD, $$SOURCE_ROOT_DIR)
        DIRS_LIST = $$split(REL_PATH, /))
        $$take_first( DIRS_LIST )
        BUILD_ROOT_DIR = $$SOURCE_ROOT_DIR"/../"$$first(DIRS_LIST)
        BUILD_ROOT_DIR = $$clean_path($$BUILD_ROOT_DIR)
        return ($$BUILD_ROOT_DIR)
    }
}

