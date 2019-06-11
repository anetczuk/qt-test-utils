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


## copySingleFile( $$source_file, $$destination_file )
##      source_file         - source file path
##      destination_file    - destination file path
##
##      In case of copying multiple files or directories see "file_copies" qmake configuration feature.
##
defineTest(copySingleFile) {
    source_file = $$1
    destination_file = $$2

    destination_dir = $$dirname( destination_file )
    !exists( $$destination_dir ) {
        mkpath( $$destination_dir )
    }

    compiler_name = $$prepareTargetNameFromPath( $$destination_file )

    eval( $${compiler_name}_input_var = $$source_file )
    eval( $${compiler_name}.name = copy file )
    eval( $${compiler_name}.input = $${compiler_name}_input_var )
    eval( $${compiler_name}.output = $$destination_file )
    eval( $${compiler_name}.commands = $(COPY_FILE) ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT} )
    eval( $${compiler_name}.CONFIG = no_link target_predeps )
    QMAKE_EXTRA_COMPILERS += $${compiler_name}

    export( $${compiler_name}_input_var )
    export( $${compiler_name}.name )
    export( $${compiler_name}.input )
    export( $${compiler_name}.output )
    export( $${compiler_name}.commands )
    export( $${compiler_name}.CONFIG )
    export( QMAKE_EXTRA_COMPILERS )

    return(true)
}


#==================================================================================


defineReplace( prepareSourceFilesList ) {
    source_files_list = $$1
    recursive = $$2

    sources_list =
    for(source_file, source_files_list) {
        found_sources = $$files( $$source_file, $$recursive )
        for(in_file, found_sources) {
            isDir( $$in_file ) {
                ## directory -- do nothing
            } else {
                ## file
                sources_list += $$in_file
            }
        }
    }
    return($$sources_list)
}


defineReplace(prepareTargetNameFromPath) {
    targetname = $$replace(1, [/\\\\], _)
    targetname = $$replace(targetname, -, _)
    targetname = $$replace(targetname, \., _)
    targetname = $$replace(targetname, " ", _)
    targetname = $$replace(targetname, :, _)
    targetname = $$join(targetname, "_")
    return($$targetname)
}
