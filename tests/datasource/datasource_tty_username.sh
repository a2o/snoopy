#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE tty_username`
if [ "(unknown)" == "$VAL_SNOOPY" ]; then
    VAL_SNOOPY="(none)"
fi

if ! tty -s; then
    VAL_REAL="(none)"
else
    VAL_REAL=`tty`
    if [ "not a tty" == "$VAL_REAL" ]; then
        VAL_REAL="(none)"
    else
        VAL_REAL=`echo "$VAL_REAL" | xargs ls -la | awk '{print $3}'`
    fi
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
