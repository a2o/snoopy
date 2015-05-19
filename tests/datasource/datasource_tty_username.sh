#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE tty_username`
VAL_REAL=`tty | xargs ls -la | awk '{print $3}'`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
