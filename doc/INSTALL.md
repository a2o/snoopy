# Snoopy Logger Installation Instructions



  * [Snoopy Logger Installation Instructions](#snoopy-logger-installation-instructions)
    * [Available installation methods:](#available-installation-methods)
      * [1. Automated installation procedure](#1-automated-installation-procedure)
      * [2. Installing Snoopy from distribution package repositories](#2-installing-snoopy-from-distribution-package-repositories)
        * [2.1 Building distribution-specific packages yourself](#21-building-distribution-specific-packages-yourself)
      * [3. Building Snoopy from sources](#3-building-snoopy-from-sources)
        * [3.1 Downloading source](#31-downloading-source)
        * [3.2 Installation procedure](#32-installation-procedure)
        * [3.3 Build configuration](#33-build-configuration)
          * [3.3.1 Configuring log output](#331-configuring-log-output)
          * [3.3.2 Configuring filtering](#332-configuring-filtering)
          * [3.3.3 Optional configuration file support](#333-optional-configuration-file-support)
      * [4 How to enable/activate Snoopy](#4-how-to-enableactivate-snoopy)
        * [4.1 Enable for specific program](#41-enable-for-specific-program)
        * [4.2 Enable system-wide Snoopy on single architecture systems](#42-enable-system-wide-snoopy-on-single-architecture-systems)
        * [4.3 For multiarch systems](#43-for-multiarch-systems)
      * [5 Snoopy output](#5-snoopy-output)
      * [6 How to disable Snoopy](#6-how-to-disable-snoopy)



## Available installation methods:

* automated installation procedure (preferred, installs latest version), or
* using packages provided by your Linux distribution, or
* building it manually.





### 1. Automated installation procedure

To start automated installation procedure for **STABLE** version of Snoopy,
run this command:

    rm -f install-snoopy.sh &&
    wget -q -O install-snoopy.sh https://github.com/a2o/snoopy/raw/install/install/install-snoopy.sh &&
    chmod 755 install-snoopy.sh &&
    ./install-snoopy.sh stable

To start automated installation procedure for **DEVELOPMENT** version of Snoopy,
run this command:

    rm -f install-snoopy.sh &&
    wget -q -O install-snoopy.sh https://github.com/a2o/snoopy/raw/install/install/install-snoopy.sh &&
    chmod 755 install-snoopy.sh &&
    ./install-snoopy.sh git-master

**WARNING: This manual is for latest Snoopy version only. Any version that is not the latest is not supported. Please do not submit any bug/feature/change requests related to unsupported versions.**





### 2. Installing Snoopy from distribution package repositories

This is generally simple, if some maintainer already packaged Snoopy for you.
How to install distribution-provided packages is out of scope of this manual.

Generally you would do something like this:

    apt-get install snoopy
    yum     install snoopy
    zypper  install snoopy



#### 2.1 Building distribution-specific packages yourself

The following Linux distributions have their corresponding package definition
files available in the contrib/ directory:

* Debian/Ubuntu, in contrib/debian/
* RHEL/CentOS/Fedora, in contrib/rhel/
* SLES/OpenSUSE, in contrib/sles/

The contents of contrib/ directory is user-supplied and therefore not supported. Patches
however are gladly accepted.





### 3. Building Snoopy from sources

#### 3.1 Downloading source

You can download Snoopy release tarballs from this location: https://github.com/a2o/snoopy/releases
Alternatively, you can clone the latest Snoopy sources from GitHub.
```
git clone https://github.com/a2o/snoopy.git
cd snoopy

# This generates ./configure script
./bootstrap.sh
```


#### 3.2 Installation procedure

Snoopy supports various features that  can be enabled by supplying arguments
to configure command. Consult `./configure --help' for more information.

    # Only if you are building directly from git repository:
    ./bootstrap.sh

    # Check configuration options, see section 3.3 for details:
    ./configure --help

    # Then continue with normal build procedure:
    ./configure [OPTIONS]
    make
    make install

    # At this point, Snoopy is **installed but not yet enabled**.
    # Enable it
    make enable

    # Reboot your system for Snoopy to be picked by all programs.
    reboot



#### 3.3 Build configuration

##### 3.3.1 Configuring log output

Snoopy already has default log message format configured, but by using
"./configure --with-message-format=FORMAT" you can adjust it to your
needs.

Log message format specification example:

    --with-message-format="text1:%{datasource1} text2:%{datasource2} text3:%{datasource3:arg}"

Text outside %{...} is considered literal and is copied as-is to final
log message. On the other hand, text found within %{...} has special
meaning: it retrieves data from specified data source. If data source
specification contains a colon, then text before colon is considered
data source name, and text following the colon is passed as an argument
to the data source provider in question.



##### 3.3.2 Configuring filtering

Snoopy supports message filtering. Filtering support must be
enabled at build time, here is an example:

    # REQUIRED TO ENABLE FILTERING FEATURE
    --enable-filtering

    # HOW TO DEFINE FILTER CHAINS
    --with-filter-chain="FILTER_CHAIN_SPEC"

By default, if FILTER_CHAIN_SPEC is not configured. An empty string is
used instead, which effectively disables filtering and all Snoopy messages
are passed to the configured output.

See sample configuration file etc/snoopy.ini for list and description
of supported filter configurations.


##### 3.3.3 Optional configuration file support

Snoopy supports optional configuration file, which may help with
development and/or configuration endeavours. Configuration file must
be enabled at build time:

    --enable-config-file

Configuration file is installed as SYSCONFDIR/snoopy.ini. SYSCONFDIR
can be changed with --sysconfdir=PATH configuration directive.
See sample configuration file etc/snoopy.ini for list and description
of supported configuration directives.





### 4 How to enable/activate Snoopy

#### 4.1 Enable for specific program

If you wish to monitor only certain applications you can do so through
the LD_PRELOAD environmental variable - simply set it to the full path
to libsnoopy.so shared library before starting the application.

Example:

    export LD_PRELOAD=/usr/local/lib/libsnoopy.so    # default path
    lynx http://linux.com/
    unset LD_PRELOAD



#### 4.2 Enable system-wide Snoopy on single architecture systems

    # Use special Snoopy-enabling script
    snoopy-enable

    # Or enable it using build tools
    make enable

Under the hood:

An entry is created in /etc/ld.so.preload file  which  causes  execv()
and execve() system calls to be intercepted by Snoopy.



#### 4.3 For multiarch systems

Snoopy does not natively support installation on systems that concurrently support multiple architectures.
However, for an example installation on such systems, consult the [doc/INSTALL-MULTIARCH.md](INSTALL-MULTIARCH.md) guide.





### 5 Snoopy output

The exact location  of  your  Snoopy output  depends  on  your  syslog
configuration. Usually it gets stored in one of the following files:

    /var/log/auth*
    /var/log/messages
    /var/log/secure



### 6 How to disable Snoopy

The simplest way is by using special script:

    snoopy-disable

To manually disable Snoopy, simply edit /etc/ld.so.preload and remove
reference to libsnoopy.so. Also unset any environmental variable that
references Snoopy (LD_PRELOAD, LD_PRELOAD_32 and LD_PRELOAD_64).  Then
you may also delete Snoopy shared library from  your  system.  Default
installation path of Snoopy shared library is:

    /usr/local/lib/libsnoopy.so
