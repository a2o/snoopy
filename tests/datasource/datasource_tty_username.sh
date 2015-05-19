#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE tty_username`
if ! tty -s; then
    VAL_REAL="(none)"
else
    VAL_REAL=`tty | xargs ls -la | awk '{print $3}'`
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
