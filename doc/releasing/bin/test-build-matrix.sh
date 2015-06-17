#!/bin/bash



### Configure shell
#
set -e
set -u
set -o pipefail



### Which configurations are we testing
#
CONFIGURE_ARGS_MATRIX=(
    '--enable-output-devlog'   # This is a normal build - we must not have "" so we imitate it with something that is always enabled
    '--enable-everything'
    '--enable-everything --disable-config-file'
    '--enable-everything --disable-filtering'
    '--enable-everything --disable-config-file --disable-filtering'
    '--enable-everything --disable-thread-safety'
    '--disable-everything'
    '--disable-everything --enable-config-file'
    '--disable-everything --enable-config-file --enable-output-file'
    '--disable-everything --enable-filtering'
    '--disable-everything --enable-filtering --enable-datasource-uid'
    '--disable-everything --enable-config-file --enable-filtering'
    '--disable-everything --enable-thread-safety'
)



### Display stuff
#
echo -e "\n\n\n"
cat <<EOF
###
### Starting matrix build test.
### List of ./configure arugments to be tested:
###
EOF
i=0
arrSize=${#CONFIGURE_ARGS_MATRIX[@]}
while [ "x${CONFIGURE_ARGS_MATRIX[i]}" != "x" ]; do
    echo "###    ./configure ${CONFIGURE_ARGS_MATRIX[i]}";

    i=$((i + 1))
    if [ "$i" -ge "$arrSize" ]; then
        break;
    fi
done
echo    "###"



### Initialize build environment
#
echo -e "\n\n\n"
cat <<EOF
###
### Bootstrapping the build environment
###
EOF
./bootstrap.sh
./configure --enable-everything
make maintainer-clean
./bootstrap.sh



### Loop through the builds
#
i=0
arrSize=${#CONFIGURE_ARGS_MATRIX[@]}
while [ "x${CONFIGURE_ARGS_MATRIX[i]}" != "x" ]; do
    CONFIGURE_ARGS="${CONFIGURE_ARGS_MATRIX[i]}"

    echo -e "\n\n\n"
    cat <<EOF
###
### Starging build with './configure $CONFIGURE_ARGS'
###
EOF

    ./configure $CONFIGURE_ARGS
    make -j16 check

    # Clean with this, otherwise it fails.
    # This probably has to do something with iniparser being a subpackage.
    ./configure --enable-everything
    make distclean

    i=$((i + 1))
    if [ "$i" -ge "$arrSize" ]; then
        break;
    fi
done



### Tell the user what has been done
#
echo -e "\n\n\n"
cat <<EOF
###
### All builds from build matrix passed.
### All done.
###
EOF
