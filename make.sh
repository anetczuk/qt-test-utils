#!/bin/bash

set -e


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


## include setttings file
. $SCRIPT_DIR/env.sh



rebuild=0

for i in "$@"; do
case $i in
    --rebuild)      rebuild=1
                    shift                   # past argument with no value
                    ;;
    *)              ;;                      # unknown option
esac
done



echo "Params: $@"


if [ $rebuild -ne 0 ]; then
    echo "Cleaning"
    $SCRIPT_DIR/clean.sh
    $SCRIPT_DIR/configure.sh
fi


cd $RELEASE_DIR
make $@

echo ""

cd $DEBUG_DIR
make $@

echo -e "\nDone"
