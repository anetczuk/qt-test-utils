#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


PROJECT_DIR=$SCRIPT_DIR
BUILD_DIR=$SCRIPT_DIR/build


rm -rf $BUILD_DIR
rm -rf $BUILD_DIR-*
