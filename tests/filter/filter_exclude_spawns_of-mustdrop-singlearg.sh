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
. /etc/os-release
if [[ $ID =~ ^(alpine|arch|almalinux|centos|fedora|rhel|opensuse) ]]; then
    PARENT_PROC_NAME="sh"
fi



### Get data
#
if ! $SNOOPY_TEST_CLI run filter   "exclude_spawns_of"   "$PARENT_PROC_NAME" > /dev/null; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Message passed through when it shouldn't."
fi
