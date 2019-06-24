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


##
## Common configuration for subprojects
##


## debug_and_release - builds intermediate products in release and debug subdirs
## debug_and_release_target - puts final products in release and debug subdirs
#CONFIG += debug_and_release debug_and_release_target

## QtCreator's Profile mode is configured as Release


!equals( COMMON_PRI_GUARD, "" ) {
    message("common.pri file can be loaded only once, loaded from:");
    message( "-" $$COMMON_PRI_PREVIOUS_PRO );
    message( "-" $$_PRO_FILE_ );
    error("exiting")
}
COMMON_PRI_GUARD="xxx"
COMMON_PRI_PREVIOUS_PRO=$$_PRO_FILE_


CONFIG += object_parallel_to_source


PROJECT_NAME = $$fileName( $$_PRO_FILE_ )
PROJECT_FILE_NAME = $$basename( _PRO_FILE_ )
PROJECT_DIR_NAME = $$dirName( $$_PRO_FILE_ )

##SOURCE_ROOT_DIR = $$realPath( $$PWD )                 ## moved to .qmake.conf
SOURCE_CURRENT_DIR = $$realPath( $$_PRO_FILE_PWD_ )
BUILD_ROOT_DIR = $$shadowedRootPath()
BUILD_CURRENT_DIR = $$realPath( $$OUT_PWD )


#message("common SOURCE_ROOT_DIR:" $$SOURCE_ROOT_DIR)
#message("common SOURCE_CURRENT_DIR:" $$SOURCE_CURRENT_DIR)
#message("common BUILD_ROOT_DIR:" $$BUILD_ROOT_DIR)
#message("common BUILD_CURRENT_DIR:" $$BUILD_CURRENT_DIR)


BUILD_SUBDIR = .
#CONFIG(debug, debug|release) {
#	BUILD_SUBDIR = debug
#}
#CONFIG(release, debug|release) {
#	BUILD_SUBDIR = release
#}


equals(PROJECT_DIR_NAME, $$PROJECT_NAME) {
    BUILD_PROJECT_SUBDIR = $$BUILD_SUBDIR
} else {
    BUILD_PROJECT_SUBDIR = $$PROJECT_NAME/$$BUILD_SUBDIR
}

OBJECTS_DIR = $$BUILD_PROJECT_SUBDIR
MOC_DIR = $$BUILD_PROJECT_SUBDIR
RCC_DIR = $$BUILD_PROJECT_SUBDIR
UI_DIR = $$BUILD_PROJECT_SUBDIR
DLLDESTDIR = $$BUILD_PROJECT_SUBDIR
DESTDIR = $$BUILD_PROJECT_SUBDIR



include(codecoverage.pri)



##
## ===============================================================
##



# QMAKE_SUBSTITUTES


# linkStaticLibrary(lib_build_dir, lib_target_name):
# Link static library to the project that depend on other library
defineTest(linkStaticLibrary) {
    lib_build_dir = $$1
    lib_target_name = $$2

    isEmpty(lib_target_name) {
        lib_target_name = $$lib_build_dir
    }

    LIB_DIR = $$BUILD_ROOT_DIR/$$lib_build_dir/$$BUILD_SUBDIR
    !exists( $$LIB_DIR ) {
          message( "Static lib build dir not found:" $$LIB_DIR )
    }
    LIB_FILE_NAME = $$staticLibName($$lib_target_name)
    LIB_PATH = $$LIB_DIR/$$LIB_FILE_NAME
    #message("lib path:" $$LIB_PATH )
    !exists( $$LIB_PATH ) {
          # message( "Static lib not found:" $$LIB_PATH)
    }

    # link library
    LIBS += -L$$LIB_DIR -l$$lib_target_name
    # add library headers directory to search path
    INCLUDEPATH += $$SOURCE_ROOT_DIR/$$lib_build_dir/src
    # configure dependency - causes target to rebuild when library changes (otherwise it causes nasty problems, at least under qmake 3.1)
    PRE_TARGETDEPS += $$LIB_PATH

    # export variables to global scope
    export(LIBS)
    export(INCLUDEPATH)
    export(PRE_TARGETDEPS)

    return(true)
}


defineReplace(staticLibName) {
    lib_target_name = $$1
    lib_name = ""
    win32 {
        lib_name = "$$lib_target_name"".lib"
    } else {
        ## unix/macx
        lib_name = "lib$$lib_target_name"".a"
    }
    return($$lib_name)
}


##
## params:
##      out_file_path   - path to output resource file
##      qml_files_list  - list of qml files
##      prefix          - prefix
##
defineTest(generateResFile) {
    file_path = $$1
    content = $$2
    prefix = $$3
    files_prefix = ""
    isEmpty(prefix) {
        prefix = "/"
    } else {
        files_prefix = $$prefix
    }

#    message( "xxx:" $$file_path $$content )

    FILE_TEMPLATE  = ""
    FILE_TEMPLATE += "<RCC>"
    ##FILE_TEMPLATE += "    <qresource prefix=\""$$prefix"\">"
    FILE_TEMPLATE += "    <qresource prefix=\"/\">"
    for(FILENAME, content){
        ##FILE_TEMPLATE += "        <file>"$$prefix$$FILENAME"</file>"
        FILE_TEMPLATE += "        <file alias=\""$$FILENAME"\">"$$prefix$$FILENAME"</file>"
    }
    FILE_TEMPLATE += "    </qresource>"
    FILE_TEMPLATE += "</RCC>"

#    message( "generating resource file: " $$file_path )
    write_file( $$file_path, FILE_TEMPLATE )

    RESOURCES += $$file_path
    export(RESOURCES)

    return(true)
}
