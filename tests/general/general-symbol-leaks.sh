#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Check for leaked global symbols
#
RES=`nm $SNOOPY_LIBSNOOPY_TEST_SO_PATH | grep ' T ' | grep -v ' snoopy_' | grep -Ev ' (execve?|_init|_fini|__gcov_sort_n_vals|mangle_path)$' | grep -c . | cat`
if [ "$RES" != "0" ]; then
    nm $SNOOPY_LIBSNOOPY_TEST_SO_PATH | grep ' T ' | grep -v ' snoopy_' | grep -Ev ' (execve?|_init|_fini)$'  | cat
    snoopy_testResult_fail "The .so library is leaking global symbols that are not prefixed with 'snoopy_' prefix or declared as 'static'. Inspect with 'nm $SNOOPY_LIBSNOOPY_TEST_SO_PATH' manually."
else
    snoopy_testResult_pass "No leaked symbols found."
fi
