#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource env_all`



### Evaluate
#
if [ "$VAL_SNOOPY" != "" ]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "$VAL_SNOOPY" ""
fi
