#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


## include setttings file
. $SCRIPT_DIR/env.sh


mkdir -p $RELEASE_DIR
cd $RELEASE_DIR
qmake $PROJECT_DIR/src/$PROJECT_FILE $@ CONFIG+=release

echo ""

mkdir -p $DEBUG_DIR
cd $DEBUG_DIR
qmake $PROJECT_DIR/src/$PROJECT_FILE $@ CONFIG+=debug
