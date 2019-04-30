##
## Common configuration for subprojects
##


## debug_and_release - builds intermediate products in release and debug subdirs
## debug_and_release_target - puts final products in release and debug subdirs
#CONFIG += debug_and_release debug_and_release_target

## QtCreator's Profile mode is configured as Release


SOURCE_ROOT_DIR = $$PWD
BUILD_ROOT_DIR = $$OUT_PWD/..

BUILD_DIR = .
#CONFIG(debug, debug|release) {
#	BUILD_DIR = debug
#}
#CONFIG(release, debug|release) {
#	BUILD_DIR = release
#}



# linkStaticLibrary(lib_build_dir, lib_target_name):
# Link static library to the project that depend on other library
defineTest(linkStaticLibrary) {
    lib_build_dir = $$1
    lib_target_name = $$2

    isEmpty(lib_target_name) {
        lib_target_name = $$lib_build_dir
    }

    LIB_DIR = $$BUILD_ROOT_DIR/$$lib_build_dir/$$BUILD_DIR
    !exists( $$LIB_DIR ) {
          message( "Static lib build dir not found:" $$LIB_DIR)
    }
    LIB_FILE_NAME = $$staticLibName($$lib_target_name)
    LIB_PATH = $$LIB_DIR/$$LIB_FILE_NAME
    message("lib path:" $$LIB_PATH )
    !exists( $$LIB_PATH ) {
          message( "Static lib not found:" $$LIB_PATH)
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
    return ($$lib_name)
}
