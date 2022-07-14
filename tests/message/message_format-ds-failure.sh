#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="[ERROR: Data source 'failure' failed with the following error message: 'Artificial datasource failure triggered']"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "%{failure}"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
