#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE cwd`
VAL_REAL="$PWD"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
