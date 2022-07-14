#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="[ERROR: Data source 'dataSourceThatDoesNotExist' not found.]"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "%{dataSourceThatDoesNotExist}"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
