#!/bin/bash

set -e


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


## include setttings file
. $SCRIPT_DIR/env.sh


echo "Params: $@"


PROJECT_DIR=$SCRIPT_DIR
BUILD_DIR=$SCRIPT_DIR/build


RELEASE_DIR=$BUILD_DIR/release
DEBUG_DIR=$BUILD_DIR/debug


cd $RELEASE_DIR
make $@

echo ""

cd $DEBUG_DIR
make $@

echo -e "\nDone"
