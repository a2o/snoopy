#!/bin/bash



### Configure shell
#
set -e
set -u



### Set test configuration
#
ARG_1="ls -la /; ls /"
ARG_2="%{cmdline}"
ARG_3=""
ARG_4="2"
ARG_5=$'ls -la /\nls /'



### Call/source common combined test implementation
#
. `dirname $BASH_SOURCE`/combined-generic.sh
