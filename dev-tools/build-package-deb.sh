#!/bin/bash



### Shell configuration and error handler
#
set -e
set -u

_fatalError() {
    echo "ERROR($0): $1" 1>&2
    exit 1
}
_echo() {
    echo "[$0]: $1"
}



### Check the runtime environment
#
if [ ! -d .git ]; then
    _fatalError "This script must be run from the root of the git repository"
fi



### Get the release tag
#
RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh`
if [ "$RELEASE_TAG" == "" ]; then
    _fatalError "Unable to determine release tag, got: '$RELEASE_TAG'"
fi



### Check if release tag is properly formatted
#
if [[ ! "$RELEASE_TAG" =~ ^snoopy- ]]; then
    _fatalError "Release tag is not properly formatted - snoopy-* format is required"
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`
DEB_PACKAGE_RELEASE_NUMBER="1"
DEB_PACKAGE_VERSION_TAG="$RELEASE_VERSION-$DEB_PACKAGE_RELEASE_NUMBER"
ARCH=`dpkg-architecture -q DEB_BUILD_ARCH`
DEB_PACKAGE_FULL_TAG="${DEB_PACKAGE_VERSION_TAG}_${ARCH}"
_echo "Determined release tag:          $RELEASE_TAG"
_echo "Determined release version:      $RELEASE_VERSION"
_echo "Determined DEB package version:  $DEB_PACKAGE_VERSION_TAG"
_echo "Determined DEB package full tag: $DEB_PACKAGE_FULL_TAG"



### If this is a production build, do additional checking
#
# Alternatively, if this is not an actual release, adjust the version info in the
# packaging/deb/changelog file.
#
# Why such a convoluted approach?
#
# Well, we can't do that with ./configure that
# translates `changelog.in` template to `changelog`, as dpkg-buildpackage first
# cleans the directory, making subsequent step fail due to a missing `changelog`
# file.
#
RELEASE_IS_STABLE="false"
if [[ $RELEASE_TAG =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+(-rc[0-9]+)?$ ]]; then
    _echo "This is a stable production build, running additional consistency checks..."
    ./dev-tools/libexec/verify-last-version-everywhere.sh "$RELEASE_VERSION"
    RELEASE_IS_STABLE="true"
else
    _echo "This is a non-stable/non-production build, additional consistency checks will be skipped."

    _echo "Temporarily adjusting the version information in packaging/deb/changelog..."
    sed -i "s/^snoopy [(][^)]\+[)] unstable/snoopy ($DEB_PACKAGE_VERSION_TAG) unstable/" packaging/deb/changelog
fi



### Does the target release package already exist?
#
ARCH=`dpkg-architecture -q DEB_BUILD_ARCH`
RELEASE_PACKAGE_FILE="../snoopy_${DEB_PACKAGE_FULL_TAG}.deb"
if [ -e $RELEASE_PACKAGE_FILE ]; then
    _fatalError "Release package file already exists: $RELEASE_PACKAGE_FILE"
fi



### Create the release package
#
# (But first, clean the source directory thoroughly.)
# ((Disabled for now, looks like dpkg-buildpackage runs `distclean` by itself.))
#
#./bootstrap.sh
#./configure --enable-everything
#make gitclean

rm -f debian
ln -s packaging/deb debian
dpkg-buildpackage --build=binary --no-sign



### Revert the temporary changelog changes
#
if [[ "$RELEASE_IS_STABLE" != "true" ]]; then
    _echo "Reverting the temporarily changes to packaging/deb/changelog..."
    git checkout packaging/deb/changelog
fi



### Cleanup
#
rm -rf packaging/deb/.debhelper/
rm -f  packaging/deb/autoreconf.after
rm -f  packaging/deb/autoreconf.before
rm -f  packaging/deb/debhelper-build-stamp
rm -f  packaging/deb/files
rm -f  packaging/deb/snoopy.debhelper.log
rm -f  packaging/deb/snoopy.postrm.debhelper
rm -f  packaging/deb/snoopy.substvars
rm -rf packaging/deb/snoopy/
rm -f  debian



### Report success
#
_echo ""
_echo "SUCCESS: Build complete."
_echo "SUCCESS: Result: $RELEASE_PACKAGE_FILE"
_echo ""



### Suggest next step(s)
#
if [ "$RELEASE_IS_STABLE" == "true" ]; then
    _echo ""
    _echo "Next step:"
    _echo "=========="
    _echo "Publish the release using the following command - FIXME:"
    _echo ""
    _echo "    ./dev-tools/publish-release.sh"
    _echo ""
else
    _echo ""
    _echo "Next step:"
    _echo "=========="
    _echo "This is a non-stable version, skipping the publishing suggestion."
    _echo ""
fi
