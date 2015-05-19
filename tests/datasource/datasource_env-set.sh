#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
export SNOOPY_TEST_DATASOURCE_ENV_SET="asdf"
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE env "SNOOPY_TEST_DATASOURCE_ENV_SET"`
VAL_REAL="$SNOOPY_TEST_DATASOURCE_ENV_SET"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
