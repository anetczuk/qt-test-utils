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


# addSubprojects(subdirs,deps): Adds directories to the project that depend on
# other directories
defineTest(addSubprojects) {
    projects_paths_list = $$1
    for(project_path, projects_paths_list) {
        found_files = $$files($$project_path, false)
        for(item_path, found_files) {
            equals( item_path, $$PROJECT_FILE_NAME ) {
                # found itself or parent -- skip
            } else {
                isDir( $$item_path ) {
                    addSubprojectDir( $$item_path )
                } else {
                    addSubprojectFile( $$item_path )
                }
            }
        }
    }

    return(true)
}


###
### Functions based on: https://wiki.qt.io/Undocumented_QMake
###

defineTest(addSubprojectFile) {
    subdir_path = $$1
#    deps_param = $$2

    item_ext = $$fileExtension( $$subdir_path )
    !equals( item_ext, "pro" ) {
        ## file with wrong extension -- exiting
        return(false)
    }

    subtarget = $$prepareTargetNameFromPath($$subdir_path)
    eval ($${subtarget}.file = $$subdir_path)
##            for(dep, deps_param):eval ($${subtarget}.depends += $$replace(dep, [/\\\\], _))
    SUBDIRS += $$subtarget
    export ($${subtarget}.file)
    export ($${subtarget}.depends)
    export (SUBDIRS)

    return(true)
}

defineTest(addSubprojectDir) {
    subdir_path = $$1
#    deps_param = $$2

    pro_file_path = $$subdir_path/$$lastElement( $$subdir_path, "/" )".pro"
    !exists( $$pro_file_path ) {
        ## pro file not exists -- exiting
        return(false)
    }

    subtarget = $$prepareTargetNameFromPath($$subdir_path)
    eval ($${subtarget}.subdir = $$subdir_path)
##            for(dep, deps_param):eval ($${subtarget}.depends += $$replace(dep, [/\\\\], _))
    SUBDIRS += $$subtarget
    export ($${subtarget}.subdir)
    export ($${subtarget}.depends)
    export (SUBDIRS)

    return(true)
}

defineReplace(prepareTargetNameFromPath) {
    targetname = $$replace(1, [/\\\\], _)
    targetname = $$replace(targetname, -, _)
    targetname = $$replace(targetname, \., _)
    targetname = $$replace(targetname, " ", _)
    return($$targetname)
}
