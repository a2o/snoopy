#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
COMMAND_NO_ARGS="$SNOOPY_TEST_CLI"
VAL_SNOOPY=`$COMMAND_NO_ARGS run datasource filename`
VAL_REAL="$COMMAND_NO_ARGS"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
