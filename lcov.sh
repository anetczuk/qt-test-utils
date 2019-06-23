#!/bin/bash

set -e


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


## include setttings file
. $SCRIPT_DIR/env.sh


COVERAGE_ROOT=tmp/lcov
COVERAGE_FILE=$COVERAGE_ROOT/coverage-gcov.info
COVERAGE_HTML=$COVERAGE_ROOT/coverage

mkdir -p $COVERAGE_ROOT

REAL_PWD=$(realpath $PWD)


## capture coverage data
lcov --capture --directory . --path . --output-file $COVERAGE_FILE --no-external

## exclude certain source files like test files or from build directory
lcov --output-file $COVERAGE_FILE --remove $COVERAGE_FILE '*/build*/*'
lcov --output-file $COVERAGE_FILE --remove $COVERAGE_FILE '*/testutils-qttests/*'
lcov --output-file $COVERAGE_FILE --remove $COVERAGE_FILE '*/testutils-quicktests/*'

## generate html view
genhtml $COVERAGE_FILE --output-directory $COVERAGE_HTML --prefix $REAL_PWD/src

echo ""
echo "Code coverage result can be found in $COVERAGE_HTML"

