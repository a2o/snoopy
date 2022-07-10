#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource snoopy_configure_command`



### Evaluate
#
if [[ $VAL_SNOOPY =~ \./configure ]]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "The output does not start with './configure'. Instead, we got this: $VAL_SNOOPY"
fi
