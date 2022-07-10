#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
STR=""
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "$STR"`
VAL_REAL="$STR"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
