# Installation Instructions - Snoopy Logger 

* Building and installing:
  * [Automated installation procedure](#automated-installation-procedure)
  * [Building from source](#building-from-source)
  * [Distribution-native packages](#distribution-native-packages) (and [how to build them](#building-a-distribution-native-package))
  * [Multiarch systems](#multiarch-systems)

* Configuration:
  * [Important build configuration flags](#important-build-configuration-flags)
  * [/etc/snoopy.ini](#etcsnoopyini)

* Enabling:
  * [Enable for a specific command](#enable-for-a-specific-command)
  * [Enable system-wide](#enable-system-wide)
    * [Under the hood](#under-the-hood)

* Removing:
  * [What gets installed](#what-gets-installed)
  * [How to disable Snoopy](#how-to-disable-snoopy)



## Automated installation procedure

(This is the preferred installation method.)

To start the automated installation procedure of the latest **stable** version of Snoopy,
run this command:
```
wget -q -O install-snoopy.sh https://github.com/a2o/snoopy/raw/install/install/install-snoopy.sh &&
chmod 755 install-snoopy.sh &&
./install-snoopy.sh stable
```

To use the automated installation procedure for installing the latest **development**
version of Snoopy, simply modify the `./install-snoopy.sh` command above by replacing
the `stable` argument with `git-master`.



## Building from source

Download the Snoopy source tarball (you can find the releases [here](https://github.com/a2o/snoopy/releases)):
```
wget https://github.com/a2o/snoopy/releases/download/snoopy-2.4.15/snoopy-2.4.15.tar.gz
tar -xzf snoopy-2.4.15.tar.gz
cd snoopy-2.4.15
```

Alternatively, clone the latest Snoopy git repository from GitHub:
```
git clone https://github.com/a2o/snoopy snoopy
cd snoopy

# Run the ./bootstrap.sh script to generate the ./configure script
./bootstrap.sh
```

Configure the build:
```
./configure [OPTIONS]
```
For information about available build configuration options, consult the section below
listing [the most important build configuration flags](#important-build-configuration-flags)
or run the `./configure --help` to list all available flags.

Build it:
```
make
```

Optionally run the test suite:
```
make tests
```

Install it:
```
make install
```

At this point, Snoopy is **installed but not yet enabled**.
What is missing is (optionally) [adjusting the snoopy.ini configuration file](#etcsnoopyini)
and [enabling Snoopy for a specific command](#enable-for-a-specific-command) or [enabling Snoopy system-wide](#enable-system-wide).



## Distribution-native packages

Providing distribution-specific instructions on how to install packages is out of the scope of this manual.
However, installing a distribution-native Snoopy package should look something like this:
```
apt     install snoopy   # Debian / Ubuntu
yum     install snoopy   # RHEL / CentOS
zypper  install snoopy   # SLES / OpenSUSE
```


#### Building a distribution-native package

**Disclaimer:**

* There are distribution-native Snoopy packages in the wild, created by distribution package maintainers.
* Efforts of distribution package maintainers and efforts of Snoopy core developers are (mostly) independent of each other (as of late 2021).

That said, some distribution package maintainers (and other interested individuals) have contributed their work to the Snoopy upstream repository (this repository).
Their contributions are collected in the [contrib/](../contrib/) directory:

* Debian/Ubuntu, in `contrib/debian/`
* RHEL/CentOS/Fedora, in `contrib/rhel/`
* SLES/OpenSUSE, in `contrib/sles/`



## Multiarch systems

Snoopy does not natively support installation on systems that concurrently support multiple architectures.
However, [doc/INSTALL-MULTIARCH.md](INSTALL-MULTIARCH.md) guide contains an example multiarch installation.



## Important build configuration flags

| Flag                   | Meaning |
|------------------------|---------|
| --disable-config-file  | Disable `snoopy.ini` support (default: enabled) |
| --with-message-format  | Modify the built-in default log message format. Useful when config file support is disabled. |
| --with-default-output  | Modify the built-in default output target. Useful when config file support is disabled. |
| --enable-thread-safety | Enable per-thread data structures (experimental, default: disableD) |
| --prefix               | Overall installation target directory prefix (default: `/usr/local`) |
| --libdir               | Target `libsnoopy.so` installation directory (default: `PREFIX/lib`) |
| --sysconfdir           | Target `snoopy.ini` installation directory (default: `PREFIX/etc`) |

Run the `./configure --help` command to get a list of all available build flags.



## /etc/snoopy.ini

When Snoopy has been built with a configuration file support (which is enabled by default),
then Snoopy runtime configuration is picked up from the `/etc/snoopy.ini` file (or `PREFIX/etc/snoopy.ini` or `SYSCONFDIR/snoopy.ini`, depending on your build flags).

All supported runtime configuration directives are listed and described in the default [snoopy.ini](../etc/snoopy.ini.in) configuration file.
Configuration changes are picked up instantly, as Snoopy re-reads its configuration file on each invocation.



## Enable for a specific command

When only a certain application should be monitored for its execution of external programs,
the `LD_PRELOAD` environment variable can be used:
```
LD_PRELOAD="/usr/local/lib/libsnoopy.so" bash
```
The new `bash` shell instance will log any executed command. Exiting this `bash` will stop the logging.



## Enable system-wide

There are two ways to enable Snoopy.
The first one is a dedicated Snoopy-enabling script that is installed by the `make install` step above:
```
snoopy-enable
```

Alternatively, when Snoopy has been built from source, the following `make` command can be used from within the source directory:
```
make enable
```

A system reboot is usually necessary to make all programs pick up the newly installed Snoopy.


####  Under the hood

`snoopy-enable` or `make enable` command creates an entry in the `/etc/ld.so.preload` file.
This entry causes the [dynamic linker](https://man7.org/linux/man-pages/man8/ld.so.8.html) to preload the Snoopy shared library,
which interposes Snoopy between the `execv()`/`execve()` function calls of a program and the real `execv()`/`execve()` implementations in the libc library.
When `execv()`/`execve()` is called, the interposed Snoopy code is responsible for creating a log entry for a command that is to be executed,
and then the execution is forwarded to the real `execv()`/`execve()` libc functions to do the actual program invocation.



## What gets installed

Commands `make install` and `make enable`/`snoopy-enable` perform the following changes on the system:

| File             | Location             | Change |
|------------------|----------------------|--------|
| `libsnoopy.so*`  | `/usr/local/lib/`*   | File installed |
| `snoopy-enable`  | `/usr/local/bin/`*   | File installed |
| `snoopy-disable` | `/usr/local/bin/`*   | File installed |
| `snoopy.ini`     | `/etc/`*             | File installed |
| `ld.so.preload`  | `/etc/ld.so.preload` | Entry added |
*Locations of these files may vary, depending on your [build configuration flags](#important-build-configuration-flags).



## How to disable Snoopy

The simplest way to disable Snoopy is to use the script installed for this purpose:
```
snoopy-disable
```
Once disabled, a system reboot may be necessary to force a reload of all the running programs.


#### How to disable Snoopy manually

To manually disable Snoopy, remove any references to `libsnoopy.so` from the `/etc/ld.so.preload` file.
Additionally, unset any environment variable that references Snoopy (`LD_PRELOAD`, `LD_PRELOAD_32`, and `LD_PRELOAD_64`).
Reboot.
