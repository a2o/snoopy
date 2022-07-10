#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
DATASOURCE="snoopy_version"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "%{$DATASOURCE}"`
VAL_REAL=`$SNOOPY_TEST_CLI run datasource "$DATASOURCE"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
