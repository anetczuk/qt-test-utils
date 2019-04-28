#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


PROJECT_DIR=$SCRIPT_DIR
BUILD_DIR=$SCRIPT_DIR/build

PROJECT_FILE="qt-test-utils.pro"

RELEASE_DIR=$BUILD_DIR/release
DEBUG_DIR=$BUILD_DIR/debug
