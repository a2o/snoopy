#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE egroup`
MY_PID=$$
MY_EGID=`snoopy_test_getValue_fromPs "$MY_PID" "egid"`
VAL_REAL=`getent group "$MY_EGID" | cut -d: -f1`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
