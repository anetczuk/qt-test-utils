

## copySingleFile( $$source_file, $$destination_file )
##      source_file         - source file path
##      destination_file    - destination file path
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


## copyFiles( $$source_files_list, $$destination_dir )
##      source_files_list   - list source files paths
##      destination_dir     - destination directory
##      recursive           - should find files recursively in case of wildcard?
defineTest(copyFiles) {
    source_files_list = $$1
    destination_dir = $$2
    recursive = $$3

    !equals(recursive, true) {
        recursive = false
    }

    sources_list = $$prepareSourceFilesList( $$source_files_list, $$recursive )
    destination_list =
    for(source_file, sources_list) {
        ## file
        rel_path = $$basename( source_file )
        dest_file = $$destination_dir/$$rel_path
        destination_list += $$dest_file
        map_key = $$prepareTargetNameFromPath( $$source_file )
        map_push_value( $$map_key, $$dest_file );
    }

    !exists( destination_dir ) {
        mkpath( $$destination_dir )
    }

    compiler_name = $$prepareTargetNameFromPath( $$destination_dir )

    eval( $${compiler_name}_input_var = $$sources_list )

    eval( $${compiler_name}.name = copy files )
    eval( $${compiler_name}.input = $${compiler_name}_input_var )
    eval( $${compiler_name}.output_function = getOutputFileForInput )
    eval( $${compiler_name}.commands = $(COPY_FILE) ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT} )
    eval( $${compiler_name}.CONFIG = no_link target_predeps )
    QMAKE_EXTRA_COMPILERS += $${compiler_name}

    export( $${compiler_name}_input_var )

    export( $${compiler_name}.name )
    export( $${compiler_name}.input )
    export( $${compiler_name}.output_function )
    export( $${compiler_name}.commands )
    export( $${compiler_name}.CONFIG )
    export( QMAKE_EXTRA_COMPILERS )

    return(true)
}


defineTest(copyDir) {
    source_dir = $$1
    destination_dir = $$2

    sources_list = $$prepareSourceFilesList( $$source_dir/*, true )
    destination_list =
    for(source_file, sources_list) {
        ## file
        rel_path = $$relative_path( $$source_file, $$source_dir/.. )
        out_file = $$destination_dir/$$rel_path
        destination_list += $$out_file
        map_key = $$prepareTargetNameFromPath( $$source_file )
        map_push_value( $$map_key, $$out_file );
    }

    compiler_name = $$prepareTargetNameFromPath( $$destination_dir )

    eval( $${compiler_name}_input_var = $$sources_list )

    eval( $${compiler_name}.name = copy directory )
    eval( $${compiler_name}.input = $${compiler_name}_input_var )
    eval( $${compiler_name}.output_function = getOutputFileForInput )
    eval( $${compiler_name}.commands = $(COPY_DIR) $$source_dir $$destination_dir )
    eval( $${compiler_name}.CONFIG = no_link target_predeps )
    QMAKE_EXTRA_COMPILERS += $${compiler_name}

    export( $${compiler_name}_input_var )

    export( $${compiler_name}.name )
    export( $${compiler_name}.input )
    export( $${compiler_name}.output_function )
    export( $${compiler_name}.commands )
    export( $${compiler_name}.CONFIG )
    export( QMAKE_EXTRA_COMPILERS )

    return(true)
}


#==================================================================================


win32 {
    defineReplace(increaseCounter) {
        counter_var = $$1
        eval( counter_value = $$counter_var )
        counter_value = $$system("set /a $$counter_value + 1")
        eval( $$counter_var = $$counter_value )
        export( $$counter_var )
        return($$counter_var)
    }
} else:unix {
    defineReplace(increaseCounter) {
        counter_var = $$1
        counter_value = $$eval( $$counter_var )
        counter_value = $$system("echo $(($$counter_value + 1))")
        eval( $$counter_var = $$counter_value )
        export( $$counter_var )
        return($$counter_var)
    }
}


defineReplace( getOutputFileForInput ) {
    input_file = $$1
    map_key = $$prepareTargetNameFromPath( $$input_file )
    map_value = $$map_pop_value( $$map_key )
    message("getting output file:" $$input_file $$map_value )
    return($$map_value)
}


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


defineReplace( map_pop_value ) {
    map_key = $$1
    key_variable = map.$${map_key}

    isEmpty( $$key_variable ) {
        return()
    }
    val = $$eval( $$key_variable )
#    export( $$key_variable )
    return($$val)
}


defineTest( map_push_value ) {
    map_key = $$1
    map_value = $$2
    key_variable = map.$${map_key}
    !isEmpty( $$key_variable ) {
        warning("file copying collision -- copying the same file multiple times not supported, file:" $${map_key} )
    }
    eval( $$key_variable = $$map_value )
    export( $$key_variable )
    return(true)
}


defineReplace(prepareTargetNameFromPath) {
    targetname = $$replace(1, [/\\\\], _)
    targetname = $$replace(targetname, -, _)
    targetname = $$replace(targetname, \., _)
    targetname = $$replace(targetname, " ", _)
    return($$targetname)
}
