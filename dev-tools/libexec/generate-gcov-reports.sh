#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/../_bootstrap.sh



### Generate reports
#
for CFILE in `find . -name '*.c' | sort`; do
    CDIR=`dirname "$CFILE"`
    CFILENAME=`basename $CFILE`
    CFILENAME_NOEXT=`echo "$CFILENAME" | sed -e 's/\.c$//'`
    if [ -f $CDIR/$CFILENAME_NOEXT.gcno ]; then
        GCNO_FILE="$CFILENAME_NOEXT.gcno"
    elif [ -f $CDIR/.libs/$CFILENAME_NOEXT.gcno ]; then
        GCNO_FILE=".libs/$CFILENAME_NOEXT.gcno"
    else
        echo "ERROR: Cannot locate .gcno file for $CFILE ($CDIR, $CFILENAME, $CFILENAME_NOEXT), exiting."
        exit 1
    fi
    echo "Generating coverage analysis for $CFILE (from $GCNO_FILE)..."
    (
        cd $CDIR &&
        gcov $GCNO_FILE
    )
done
