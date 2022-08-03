#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Test for error(s)
#
EXPECTED_STRING="message_format"
if ! $SNOOPY_CLI conf | fgrep "$EXPECTED_STRING" > /dev/null ; then
    snoopy_testResult_fail "Expected string not encountered: '$EXPECTED_STRING'"
fi



### Evaluate
#
snoopy_testResult_pass
