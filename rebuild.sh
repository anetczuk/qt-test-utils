#!/bin/bash

set -e


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


## include setttings file
. $SCRIPT_DIR/env.sh


$SCRIPT_DIR/clean.sh

$SCRIPT_DIR/configure.sh

$SCRIPT_DIR/make.sh all
