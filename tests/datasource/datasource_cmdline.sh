#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
COMMAND="$SNOOPY_TEST_DATASOURCE cmdline"
VAL_SNOOPY=`$COMMAND`
VAL_REAL="$COMMAND"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
