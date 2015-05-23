#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
DATASOURCE="snoopy_version"
VAL_SNOOPY=`$SNOOPY_TEST_MESSAGE_FORMAT "%{$DATASOURCE}"`
VAL_REAL=`$SNOOPY_TEST_DATASOURCE "$DATASOURCE"`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
