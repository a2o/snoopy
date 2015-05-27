#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE tid_kernel`



### Evaluate
#
if [[ $VAL_SNOOPY =~ ^[0-9]{1,7}$ ]]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Unexpected thread ID format: $VAL_SNOOPY"
fi
