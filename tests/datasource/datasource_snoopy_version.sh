#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE snoopy_version`



### Evaluate
#
if [[ $VAL_SNOOPY =~ ^[0-9]+\.[0-9]+\.[0-9]+(rc[0-9]+)?(-[0-9]+-[0-9a-z]+)(-dirty)?$ ]]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "$VAL_SNOOPY"
fi
