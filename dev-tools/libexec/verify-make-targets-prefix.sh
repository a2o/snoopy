#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/../_bootstrap.sh



### Generate reports
#
RES=`make -p | grep -Eo '^[a-z][^:]+\.lo:' | grep -E '^libsnoopy' -c | cat`
if [ "$RES" -ne "0" ]; then
    make -p | grep -Eo '^[a-z][^:]+\.lo:' | grep -E '^libsnoopy'
    _fatalError "Some *.lo make targets contain 'libsnoopy...' prefix (listed above)." $LINENO
fi



### All good
#
_echo "All relevant make targets do not contain 'libsnoopy...' prefix, good."
