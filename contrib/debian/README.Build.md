# Snoopy Logger Debian / Ubuntu packages generation #

You can find recipe to build a Debian/Ubuntu package in
`contrib/debian`. You first need to symlink the `contrib/debian` to the
project root directory. You also need to install all build dependencies:

- debuild
- debhelper
- autoconf
- dh-autoreconf

The package is built into parent directory. You can install it using `dpkg`.

Basically you need to run following commands:

    ln -nfs contrib/debian .
    apt-get install devscripts debhelper autoconf dh-autoreconf
    debuild --no-tgz-check -I.git -i'\.git/'
    dpkg -i ../libsnoopy_2.0.0rc5-1_amd64.deb


NOTES: Debian package versionning is tricky when working on git
repositories. You can keep track on per-commit version number using `git
describe --tags`.

For example the git version `snoopy-2.0.0rc5-3-g7845db6` should be read as:

- `snoopy-2.0.0rc5`: Version as given by `git tag`
- `3`: Number of commits ahead of current tag
- `g7845db6`: the git commit SHA1 id.

The Debian package version should be then `2:2.0.0rc5+git.3-1`.
