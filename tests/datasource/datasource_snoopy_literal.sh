#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="test-string-`date +%s`"
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE snoopy_literal "$VAL_REAL"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
