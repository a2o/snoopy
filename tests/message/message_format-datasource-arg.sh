#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Check if there is ENV datasource available
#
RES=`$SNOOPY_TEST_CLI run datasource --list | grep -E '^ *(snoopy_version|env)$' -c`
if [ "$RES" -eq "1" ]; then
    snoopy_testResult_skip "Required datasource 'env' is not available"
fi



### Get data
#
VAR_NAME="SNOOPY_TEST_MESSAGE_FORMAT_`date +%s`"
VAR_VALUE="${VAR_NAME}-value"
export $VAR_NAME="$VAR_VALUE"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run messageformat "%{env:$VAR_NAME}"`
VAL_REAL="$VAR_VALUE"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
