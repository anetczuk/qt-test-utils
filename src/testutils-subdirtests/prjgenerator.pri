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


PRJ_TEMPLATE_PATH = "prjtest.pro.template"
ABS_PRJ_TEMPLATE_PATH = $$absolute_path( $$PRJ_TEMPLATE_PATH )

##
## params:
##      out_file_path   - path to output pro file
##      source_file     - source file to include
##
defineTest(generatePrj) {
    file_path = $$1
    include_file = $$2

    FILE_TEMPLATE = $$cat( $$PRJ_TEMPLATE_PATH, blob )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @TEMPLATE_FILE_PATH@, $$ABS_PRJ_TEMPLATE_PATH )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SOURCE_FILE@, $$include_file )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SOURCE_ROOT_DIR@, $$SOURCE_ROOT_DIR )

    write_file( $$file_path, FILE_TEMPLATE )
}


##
## Generate projects for list of source files.
##
defineReplace(generatePrjFromList) {
    sources_list = $$1
    prj_list =
    for(FILENAME, sources_list){
        REL_PATH = $$relative_path( $$FILENAME, $$SOURCE_ROOT_DIR )
        PRJ_DIR = $$dirname( REL_PATH )
        PRJ_NAME = $$fileName($$FILENAME)
        #GEN_PATH = $$BUILD_CURRENT_DIR"/gen/"$$PRJ_DIR/$$PRJ_NAME/$$PRJ_NAME".pro"
        GEN_PATH = $$BUILD_CURRENT_DIR/$$PRJ_DIR/$$PRJ_NAME/$$PRJ_NAME".pro"
        generatePrj($$GEN_PATH, $$FILENAME)
        prj_list += $$GEN_PATH
    }
    return ($$prj_list)
}


###
### Function based on: https://wiki.qt.io/Undocumented_QMake
###
# addSubdirs(subdirs,deps): Adds directories to the project that depend on
# other directories
defineTest(addFiles) {
    subdirs_param = $$1
#    deps_param = $$2
    for(subdirs, subdirs_param) {
        entries = $$files($$subdirs, true)
        for(entry, entries) {
            subtarget = $$replace(entry, [/\\\\], _)
            subtarget = $$replace(subtarget, -, _)
            subtarget = $$replace(subtarget, \., _)
            SUBDIRS += $$subtarget
            eval ($${subtarget}.file = $$entry)
##            for(dep, deps_param):eval ($${subtarget}.depends += $$replace(dep, [/\\\\], _))
            export ($${subtarget}.file)
            export ($${subtarget}.depends)
        }
    }
    export (SUBDIRS)
}
