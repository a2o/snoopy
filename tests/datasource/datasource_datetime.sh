#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE datetime`
VAL_REAL=`date "+%Y-%m-%dT%H:%M:%S%z"`

# If they differ, just do it again - maybe we were lucky :)
if [ "$VAL_REAL" != "$VAL_SNOOPY" ]; then
    VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE datetime`
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
