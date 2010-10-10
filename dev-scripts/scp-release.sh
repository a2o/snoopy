#!/bin/bash

if [ "x$1" == "x" ]; then
    echo "ERROR: No file specified, please use:   $0 file-to-copy-to-sourceforge"
    exit 1
fi

scp $1 bostjanskufca,snoopylogger@frs.sourceforge.net:/home/frs/project/s/sn/snoopylogger/
