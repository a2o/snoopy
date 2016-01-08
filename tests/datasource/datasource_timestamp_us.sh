#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY_1=`$SNOOPY_TEST_DATASOURCE timestamp_us`
sleep 0.005
VAL_SNOOPY_2=`$SNOOPY_TEST_DATASOURCE timestamp_us`
if [ "$VAL_SNOOPY_1" -gt "$VAL_SNOOPY_2" ]; then
    VAL_SNOOPY_1=`$SNOOPY_TEST_DATASOURCE timestamp_us`
    sleep 0.005
    VAL_SNOOPY_2=`$SNOOPY_TEST_DATASOURCE timestamp_us`
fi



### Evaluate
#
VAL_DIFF=`expr $VAL_SNOOPY_2 - $VAL_SNOOPY_1`
if [[ "$VAL_DIFF" -ge "5000" ]] && [[ "$VAL_DIFF" -lt "50000" ]]; then
    snoopy_testResult_pass "$VAL_DIFF"
else
    snoopy_testResult_fail "${VAL_SNOOPY_1}ms ${VAL_SNOOPY_2}ms"
fi
