#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Run the mock test
#
$SNOOPY_TEST_CLI unit outputregistry



### Success
#
snoopy_testResult_pass
