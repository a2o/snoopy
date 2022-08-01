#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Search for failures
#
ORIG_MESSAGE="msg"
FILTER_CHAIN="someUnknownFilter:someBogusArgument"

if ! $SNOOPY_TEST_CLI run filterchain   "$FILTER_CHAIN" > /dev/null; then
    snoopy_testResult_fail "Filter unexpectedly dropped the message"
fi

ADJ_MESSAGE=`$SNOOPY_TEST_CLI run filterchain "$FILTER_CHAIN" | grep -E '^ *Adjusted message: ' | sed -e 's/^ *Adjusted message: //'`
if [[ "$ORIG_MESSAGE" == "$ADJ_MESSAGE" ]]; then
    snoopy_testResult_fail "Filter chain processor didn't adjust the message to report an unknown filter"
fi



### Success
#
snoopy_testResult_pass
