#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Run the mock test
#
$SNOOPY_TEST_CLI unit util systemd



### Success
#
snoopy_testResult_pass
