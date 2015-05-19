#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`../../src/bin/snoopy-test-datasource uid`



### Get data from elsewhere
#
VAL_REAL=`id -a | grep -Eo 'uid=[0-9]+' | grep -Eo '[0-9]+'`



### Evaluate
#
if [ "$VAL_SNOOPY" != "$VAL_REAL" ]; then
    snoopy_testResult_fail "Values do not match (snoopy=$VAL_SNOOPY, real="$VAL_REAL")"
fi



### All went well
#
snoopy_testResult_pass
