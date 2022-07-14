#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Evaluate
#
if $SNOOPY_TEST_CLI run datasource failure ; then
    snoopy_testResult_fail "Datasource 'failure' didn't fail as expected"
fi

snoopy_testResult_pass
