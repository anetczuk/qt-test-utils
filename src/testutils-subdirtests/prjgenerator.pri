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


PRJ_FILE_TEMPLATE_PATH = "prjtest.pro.template"
ABS_PRJ_FILE_TEMPLATE_PATH = $$absolute_path( $$PRJ_FILE_TEMPLATE_PATH )

PRJ_DIR_TEMPLATE_PATH = "prjdir.pro.template"
ABS_PRJ_DIR_TEMPLATE_PATH = $$absolute_path( $$PRJ_DIR_TEMPLATE_PATH )


##
## params:
##      out_file_path   - path to output pro file
##      include_file     - source file to include
##
defineTest(generatePrj) {
    out_file_path = $$1
    include_file = $$2

    FILE_TEMPLATE = $$cat( $$ABS_PRJ_FILE_TEMPLATE_PATH, blob )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @TEMPLATE_FILE_PATH@, $$ABS_PRJ_FILE_TEMPLATE_PATH )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SOURCE_FILE@, $$include_file )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SOURCE_ROOT_DIR@, $$SOURCE_ROOT_DIR )

    write_file( $$out_file_path, FILE_TEMPLATE )
}


##
## params:
##      out_file_path   - path to output pro file
##      subdir_name     - source file to include
##
defineTest(generateSubdirPrj) {
    out_file_path = $$1

    subdir_name = $$fileName($$out_file_path)

#    message("generating dir prj:" $$out_file_path )

    FILE_TEMPLATE = $$cat( $$ABS_PRJ_DIR_TEMPLATE_PATH, blob )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @TEMPLATE_FILE_PATH@, $$ABS_PRJ_DIR_TEMPLATE_PATH )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SOURCE_ROOT@, $$SOURCE_ROOT_DIR )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SUBDIRS_ROOT@, $$SOURCE_CURRENT_DIR )
    FILE_TEMPLATE = $$replace( FILE_TEMPLATE, @SUBDIR@, $$subdir_name )

    write_file( $$out_file_path, FILE_TEMPLATE )
}


##
## Generate projects for list of source files.
##
defineReplace(generatePrjFromList) {
    sources_list = $$1
    gen_root_dir = $$2

    handled_dirs =

    for(FILENAME, sources_list){
        REL_PATH = $$relative_path( $$FILENAME, $$SOURCE_ROOT_DIR )
        PRJ_DIR = $$dirname( REL_PATH )

        prj_dirs_list = $$split( PRJ_DIR, /)
        curr_dir = "."
        for(DIRITEM, prj_dirs_list){
            curr_dir = $$curr_dir"/"$$DIRITEM
            !contains( handled_dirs, $$curr_dir ) {
                handled_dirs += $$curr_dir
                ## create subdir prj
#                GEN_PATH = $$gen_root_dir/$$curr_dir/$$DIRITEM".pro"
                GEN_PATH = $$gen_root_dir/$$curr_dir/../$$DIRITEM".pro"
#                message("generating subdir:" $$GEN_PATH)
                generateSubdirPrj( $$GEN_PATH )
            }
        }

        ## create subproject
        PRJ_NAME = $$fileName($$FILENAME)
        GEN_PATH = $$gen_root_dir/$$PRJ_DIR/$$PRJ_NAME".pro"
#        GEN_PATH = $$gen_root_dir/$$PRJ_DIR/$$PRJ_NAME/$$PRJ_NAME".pro"
#        message("generating project:" $$GEN_PATH $$FILENAME)
        generatePrj($$GEN_PATH, $$FILENAME)
    }

    return ()
}
