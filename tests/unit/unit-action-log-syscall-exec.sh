#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Run the mock test
#
VAL_EXPECTED="Test error message, sent out via stdout output."
VAL_SNOOPY=`$SNOOPY_TEST_CLI unit action log-syscall-exec | head -n1`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
