#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE snoopy_version`



### Evaluate
#
if [[ $VAL_SNOOPY =~ ^[0-9]+\.[0-9]+\.[0-9]+(rc[0-9]+)?$ ]]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "$VAL_SNOOPY"
fi
