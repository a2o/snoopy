#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE timestamp`
VAL_REAL=`date +"%s"`

# If they differ, just do it again - maybe we were lucky :)
if [ "$VAL_REAL" != "$VAL_SNOOPY" ]; then
    VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE timestamp`
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
