#!/bin/bash



### Configure shell
#
set -e
set -u



### Set test configuration
#
ARG_1="ls -la /"
ARG_2="%{cmdline}"
ARG_3=""
ARG_4="1"
ARG_5="ls -la /"



### Call/source common combined test implementation
#
. `dirname $BASH_SOURCE`/combined-generic.sh
