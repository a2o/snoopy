#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource snoopy_version`



### Evaluate
#
# Supported formats examples:
#   - 1.0.0 - Stable versions
#
#   - 1.0.0rc1   - RC format used until 2.5.0
#   - 1.0.0-rc.1 - Semver-compatible rc format, potentially used since 2.5.0
#   - 1.0.0rc.1  - Lazy test regex programming artifact #1, not really used nor supported
#   - 1.0.0-rc1  - Lazy test regex programming artifact #2, not really used nor supported
#
#   - 1.0.0-1-gdeadbeef - 1 commit after git tag snoopy-1.0.0. Commit's sha hash starts with "deadbeef")
#
#   - deadbeef - Git commit id, used when no tags are present in the repo (i.e. when a GitHub fork is cloned)
#
#   - [any-valid-version-here]-dirty - When the git working directory is dirty
#
if [[ $VAL_SNOOPY =~ ^([0-9]+\.[0-9]+\.[0-9]+(-?rc\.?[0-9]+)?(-[0-9]+-g[0-9a-f]+)?|[0-9a-f]+)(-dirty)?(-geezer-os)?$ ]]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "$VAL_SNOOPY"
fi
