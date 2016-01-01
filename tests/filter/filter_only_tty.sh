#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Check if currently running on tty
#
# These do not work correctly:
#   if [[ $- == *i* ]]; then
#   if [ -t 1 ]; then
#
# In $(tty), tty is a program. When test suite is run with -j1, it runs in
# interactive shell. If -j16 is used, then it runs in background.
#
RES="$(tty)" || RES="not-on-tty"   # This || is here to compensate for non-zero exit status.
if [[ $RES == /dev/* ]]; then
    ON_TTY="true"
else
    ON_TTY="false"
fi



### Execute on-tty-dependent check
#
if [ "$ON_TTY" == "true" ]; then
    if $SNOOPY_TEST_FILTER   "msg"   "only_tty" > /dev/null; then
        snoopy_testResult_pass
    else
        snoopy_testResult_fail "only_tty check running in non-interactive mode failed"
    fi
else
    if ! $SNOOPY_TEST_FILTER   "msg"   "only_tty" > /dev/null; then
        snoopy_testResult_pass
    else
        snoopy_testResult_fail "only_tty check NOT running on interactive tty failed"
    fi
fi
