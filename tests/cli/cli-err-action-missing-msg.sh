#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Test for error(s)
#
EXPECTED_ERROR_MESSAGE="[ERROR] No action specified"
RES=`$SNOOPY_CLI 2>&1 | fgrep -c "$EXPECTED_ERROR_MESSAGE"` || true
if [ "$RES" != "1" ]; then
    snoopy_testResult_fail "Expected error message not encountered: '$EXPECTED_ERROR_MESSAGE'"
fi



### Success
#
snoopy_testResult_pass
