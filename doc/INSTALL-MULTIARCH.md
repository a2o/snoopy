# Support for Multiple Architectures - Snoopy Command Logger

Table of contents:
* [Disclaimer](#disclaimer)
* [Installation](#installation)
* [Verification](#verification)
* [Notes](#notes)



## Disclaimer

* Snoopy does not natively provide installation support for environments that enable running binaries compiled for different architectures ("multiarch");
* Providing support for issues related to multiarch installations is really low on Snoopy's priority list, thus
* Anybody dabbling with multiarch Snoopy installation should be intimately familiar with how shared library loading works on their system.



## Installation

The following procedure is an example of how to approach a multiarch Snoopy installation.
It was tested on [Ubuntu 20.04](https://releases.ubuntu.com/20.04/).
It may work with other Linux distributions too.

i1. Install the build/install prerequisites:
```
apt-get update
apt-get install gcc gzip make procps socat tar wget   # Standard Snoopy build/install prerequisites
apt-get install gcc-multilib   # For 32-bit builds
```

i2. Download and unpack Snoopy sources:
```
wget https://github.com/a2o/snoopy/releases/download/snoopy-2.4.15/snoopy-2.4.15.tar.gz &&
tar -xzf snoopy-2.4.15.tar.gz &&
cd snoopy-2.4.15
```

i3. Build and install the 64-bit version first (using the same configure flags as `install-snoopy.sh` (as of 2021-11-28), with an added `--libdir` flag):
```
./configure --enable-config-file --sysconfdir=/etc --enable-filtering --libdir=/usr/lib/x86_64-linux-gnu &&
make &&
make install
```

i4. Build and install the 32-bit variation too (notice the different `--libdir` value, and there is an additional `CFLAGS=-m32` environment variable for the `./configure` command):
```
make distclean &&
CFLAGS=-m32 ./configure --enable-config-file --sysconfdir=/etc --enable-filtering --libdir=/usr/lib/i386-linux-gnu &&
make &&
make install
```

i5. Finally, enable Snoopy:
```
echo 'libsnoopy.so' > /etc/ld.so.preload
```



## Verification

This is an example verification process (again, tested on Ubuntu 20.04) for making sure that both Snoopy instances work as expected.

v1. Enable installation of packages built for the 32-bit architecture:
```
dpkg --add-architecture i386
```

v2. Install a 32-bit version of `zsh`:
```
apt-get update
apt-get install zsh:i386
```

v3. Get into the 32-bit `zsh`:
```
zsh
```

v4. Run a unique command to see if 32-bit logging works:
```
tail -f /var/log/auth.log -n14
```

v5. And sure enough, in the output of the last command, the expected `-n14` (generated by the 32-bit Snoopy instance) appears in the very last line.
Earlier log entries (ending with the `filename:/usr/bin/zsh]: zsh` entry) were produced by the 64-bit Snoopy instance:
```
t-snoopy-2004-multiarch# tail -f /var/log/auth.log -n14
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14408]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/bin/chown]: chown --reference=/etc/shells /etc/shells.tmp
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14409]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/bin/mv]: mv /etc/shells.tmp /etc/shells
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14410]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/bin/ln]: ln -s /bin/zsh /usr/bin/zsh
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14411]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/usr/bin/dpkg-maintscript-helper]: dpkg-maintscript-helper symlink_to_dir /usr/share/doc/zsh zsh-common 5.0.7-3 -- configure
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14412]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/usr/bin/basename]: basename /usr/bin/dpkg-maintscript-helper
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14413]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/usr/bin/dpkg]: dpkg --validate-version -- 5.0.7-3
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14415]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/sbin/ldconfig]: ldconfig
Nov 26 19:40:35 t-snoopy-2004-multiarch snoopy[14415]: [uid:0 sid:14352 tty:/dev/pts/5 cwd:/ filename:/sbin/ldconfig.real]: /sbin/ldconfig.real
Nov 26 19:40:36 t-snoopy-2004-multiarch snoopy[14421]: [uid:0 sid:278 tty:/dev/pts/4 cwd:/tmp filename:/usr/lib/ubuntu-advantage/apt-esm-hook]: /usr/lib/ubuntu-advantage/apt-esm-hook post-invoke-success
Nov 26 19:40:42 t-snoopy-2004-multiarch snoopy[14422]: [uid:0 sid:278 tty:/dev/pts/4 cwd:/snoopy-2.4.15 filename:/usr/bin/zsh]: zsh
Nov 26 19:40:42 t-snoopy-2004-multiarch snoopy[14423]: [uid:0 sid:278 tty:/dev/pts/4 cwd:/snoopy-2.4.15 filename:/usr/bin/getent]: getent group root
Nov 26 19:40:42 t-snoopy-2004-multiarch snoopy[14442]: [uid:0 sid:278 tty:/dev/pts/4 cwd:/snoopy-2.4.15 filename:/bin/mv]: mv -f /root/.zcompdump.t-snoopy-2004-multiarch.14422 /root/.zcompdump
Nov 26 19:40:57 t-snoopy-2004-multiarch snoopy[14445]: [uid:0 sid:278 tty:(none) cwd:/snoopy-2.4.15 filename:/usr/bin/tail]: tail --version
Nov 26 19:41:09 t-snoopy-2004-multiarch snoopy[14446]: [uid:0 sid:278 tty:/dev/pts/4 cwd:/snoopy-2.4.15 filename:/usr/bin/tail]: tail -f /var/log/auth.log -n14
^C
t-snoopy-2004-multiarch#
```



## Notes

* In the example above, both Snoopy instances (the 64-bit one residing in `/usr/lib/x86_64-linux-gnu`, and the 32-bit one in `/usr/lib/i386-linux-gnu`) will use the same `/etc/snoopy.ini` configuration file.
* The absence of full path to `libsnoopy.so` in `/etc/ld.so.preload` will make the linker choose the correct version, depending on the architecture for which the executed program was built.
* Alternatively (tested on Ubuntu 20.04), specifying full path in `/etc/ld.so.preload` as `/usr/$LIB/libsnoopy.so` will yield the same result, as `$LIB` is expanded to `lib/x86_64-linux-gnu` or `lib/i386-linux-gnu`.
* In the past, apparently `$LIB` used to expand to one of `lib64`/`lib32`/`lib` (depending on the actual libc implementation/configuration, [multilib](https://de.wikipedia.org/wiki/Multilib)). This is apparently not the case anymore (as of 2021).
* Some systems may support `LD_PRELOAD_32` and `LD_PRELOAD_64` environmental variables to load appropriate variations of architecture-dependent shared libraries.
