#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
. `dirname $0`/_bootstrap.sh



### Run autoscan
#
autoscan



### Fix the autoscan's output
#
# AC_CONFIG_SRCDIR is somewhat randomly set to the first encountered file in the source tree.
#
# Enountered so far:
# - lib/inih/src/ini.h (long time default)
# - lib/inih/src/ini.c (encountered in 2023 on Ubuntu 20.04)
# - tests/bin/action-run-filter.c (encountered in GitHub's Actions)
# 
# Let's just stop this nonsense and fix the output manually.
#
sed -i 's%^AC_CONFIG_SRCDIR.\+%# AC_CONFIG_SRCDIR commented out by dev-tools/autoscan.sh%' configure.scan
