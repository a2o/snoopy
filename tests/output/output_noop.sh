#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Output data
#
VAL_REAL=""
VAL_SNOOPY=`$SNOOPY_TEST_CLI run output   "$VAL_REAL"   "noop"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
