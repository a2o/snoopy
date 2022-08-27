#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource tty_username`
if [ "(unknown)" == "$VAL_SNOOPY" ]; then
    VAL_SNOOPY="(none)"
fi

if ! tty -s; then
    VAL_REAL="(none)"
else
    TTY_PATH=`tty`
    if [ "not a tty" == "$TTY_PATH" ]; then
        VAL_REAL="(none)"
    else
        VAL_REAL=`stat -c %U $TTY_PATH`
    fi
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
