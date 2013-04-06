# Snoopy Logger #



## 1. DESCRIPTION ##

Snoopy is designed to aid system administrators by providing a log of commands
that were executed on the system. Snoopy should be completely transparent to
users and to applications. It acts as a preloaded library that provides a
wrapper around calls to execve() syscall. Logging is done via syslog.



## 2. INSTALLATION ##

Snoopy supports various features that  can be enabled by supplying arguments
to configure command. Consult `./configure --help' for more information.


### Download options:

    # You can download snoopy release directly from this location:
    http://source.a2o.si/download/snoopy/

    # Alternatively you can download it from GitHub, by selecting a release tag
    # at the branch dropdown selector, and then clicking the ↓ZIP button, next
    # to the repository URL, below project description.
    # (I know this is awkward, but GitHub has discontinued the "Uploads" feature)
    https://github.com/a2o/snoopy/


### Installation procedure is simple:

    # If you have pulled snoopy from GIT, you must run autoconf first:
    autoheader
    autoconf

    # Check configuration options:
    ./configure --help

    # Then continue with normal build procedure:
    ./configure [OPTIONS]
    make
    make install

At this point, snoopy is **installed but not yet enabled**.



## 3. HOW TO ENABLE ##

### 3.a) Enable for specific program ###

If you wish to monitor only certain applications you can do so through
the LD_PRELOAD environmental variable - simply set it to the full path
to snoopy.so shared library before starting the application.

Example:

    export LD_PRELOAD=/usr/local/lib/snoopy.so    # default path
    lynx http://linux.com/
    unset LD_PRELOAD


### 3.b) Enable system-wide snoopy on 32-bit-only or 64-bit-only systems ###

WARNING: Using this method on multilib systems (64-bit systems capable
WARNING: of running 32-bit applications) can cause malfunction because
WARNING: preload config file /etc/ld.so.preload makes  no  distinction
WARNING: between 32- and 64-bit programs and shared libraries.

    # Enable it using entry in /etc/ld.so.preload
    make enable

An entry is created in /etc/ld.so.preload file  which  causes  execv()
and execve() system calls to be intercepted by snoopy and logged via
syslog.


### 3.c) For multilib systems ###

Content of /etc/ld.so.preload should include the following line:

    /usr/local/$LIB/snoopy.so

This applies only when you have installed both 32bit and 64bit version
of the library in the appropriate paths.


### 3.d) For multilib systems with LD_PRELOAD_* environmental variables ###

On systems that support LD_PRELOAD_32 and LD_PRELOAD_64  you  can  use
those variables to force loading of snoopy. If you with to  enable  it
system-wide, ensure that correct values are held  by  those  variables
at boot time. Consult section  4.a  of  this  README  on  how  to  set
environmental variables. Setting them at boot time is usually  distro-
dependent.
Users are also required to compile 32-bit version of library. To do so
on 64-bit systems it is usually enough to set appropriate CFLAGS:

    CFLAGS=-m32 ./configure [OPTIONS]

Of course your system must be cross-compilation capable. Consult  your
OS documentation for details on this subject.



## 4. OUTPUT ##

The exact location  of  your  snoopy output  depends  on  your  syslog
configuration. Usually it gets stored in one of the following files:

    /var/log/auth*
    /var/log/messages
    /var/log/secure



## 5. HOW TO DISABLE ##

To disable snoopy, simply edit /etc/ld.so.preload and remove the
reference to snoopy.so. Also unset  any  environmental  variable  that
references snoopy (LD_PRELOAD, LD_PRELOAD_32 and LD_PRELOAD_64).  Then
you may also delete snoopy shared library from  your  system.  Default
installation path of snoopy shared library is:

    /usr/local/lib/snoopy.so



## 6. NOTE ##

execv() calls are now explicitly logged.   Although,  according to the
man page for execv(),  it is supposed to call execve().   To this date
the reason why execv()  calls weren't being logged is unknown,  but we
are working to find out why.



## 7. CREDITS ##

Snoopy Logger was created by:
     marius@umich.edu
        mbm@linux.com

Currently it is maintained by:
    bostjan@a2o.si

Development is currently located at the following URI:
http://github.com/a2o/snoopy/



## X. WARNING TO FEDORA/REDHAT/CENTOS USERS - OBSOLETE, FIXED IN 1.9.0 ##
(and possibly other distributions)

Due to bug described at the links below, **please make sure you test your Snoopy
installation before you deploy it to production**. The bug manifests itself at
shutdown, when system hangs instead of finishing the whole shutdown procedure.

So far this has only been reproduced on Fedora, and there is a reference to
Hyper-Threading, but no one has come up with a valid solution, only workarounds.

Bug descriptions:
https://bugzilla.redhat.com/show_bug.cgi?id=745603
https://github.com/a2o/snoopy/issues/1

**How to test**
- Configure, build, install, enable
- Make sure it is working by tailing log files
- Reboot #1
- Reboot #2. If second reboot is successful, you are on the safe side

Distributions known to work OK:
* Ubuntu Server 12.04.01
* Slackware 12.2+
