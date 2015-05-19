#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE egid`
MY_PID=$$
VAL_REAL=`snoopy_test_getValue_fromPs "$MY_PID" "egid"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
