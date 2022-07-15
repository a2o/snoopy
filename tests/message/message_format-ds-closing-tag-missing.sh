#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="[ERROR: Closing data source tag ('}') not found.]"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "%{failure"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
