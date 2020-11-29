#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Shell workaround
#
# CI usually runs commands in a way similar to `docker exec ...`, and certain
# distro containers report shell in a process table as `sh` and not as `bash`.
#
PARENT_PROC_NAME="bash"
if [ -f /etc/os-release ]; then
    . /etc/os-release
    if [[ $ID =~ ^(arch|centos|opensuse) ]]; then
        PARENT_PROC_NAME="sh"
    fi
elif [ -f /etc/redhat-release ]; then
    if fgrep "CentOS release 6." /etc/redhat-release > /dev/null; then
        PARENT_PROC_NAME="sh"
    fi
fi



### Get data
#
if ! $SNOOPY_TEST_FILTER   "msg"   "exclude_spawns_of"   "aaaa,$PARENT_PROC_NAME,bbbb" > /dev/null; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Message passed through when it shouldn't."
fi
