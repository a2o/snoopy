#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Output data
#
VAL_REAL=`date +%s`
VAL_SNOOPY=`$SNOOPY_TEST_CLI run output   "$VAL_REAL"   "stdout" 2>/dev/null`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
