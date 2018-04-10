#!/bin/bash



### Show everything from test suite
#
for FILE in `find tests -name '*.log' -or -name '*.trs' | sort`; do
    echo
    echo "FILE: $FILE"
    cat $FILE
done
