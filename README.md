# Snoopy Logger

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

Snoopy is a tiny library that logs all executed commands (+ arguments) on your system.



Table of Contents
=================

  * [Snoopy Logger](#snoopy-logger)
    * [1. Installation](#1-installation)
    * [2. Snoopy output](#2-snoopy-output)
    * [3. Configuration](#3-configuration)
    * [4. Support](#4-support)
    * [5. How does Snoopy actually work?](#5-how-does-snoopy-actually-work)
    * [6. Contributing to Snoopy development](#6-contributing-to-snoopy-development)
    * [7. Credits](#7-credits)



## 1. Installation

The easiest way to start using Snoopy is to execute the following command (as root):

    wget -q -O - https://github.com/a2o/snoopy/raw/master/bin/snoopy-install.sh | sh

That is it.
Detailed installation instructions are available here: [doc/INSTALL.md](doc/INSTALL.md)



## 2. Snoopy output

This is what typical Snoopy output looks like:

    2015-02-11T19:05:10+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/cat]: cat /etc/fstab.BAK
    2015-02-11T19:05:15+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/rm]: rm -f /etc/fstab.BAK
    2015-02-11T19:05:19+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/tail]: tail -f /var/log/messages

These are default output locations on various Linux distributions:

* CentOS: /var/log/secure
* Debian: /var/log/auth.log
* Ubuntu: /var/log/auth.log
* others: /var/log/messages (potentially, not necessarily)

For actual output destination check your syslog configuration.



## 3. Configuration

Most parts of Snoopy are/can be/should be configured at build time.

However, since version 2.0.0 Snoopy supports optional configuration file.
Snoopy's automated installation procedure enables configuration file support
by default. Configuration file path is /etc/snoopy.ini.

For additional information consult comments in default configuration file
[etc/snoopy.ini](etc/snoopy.ini) and read appropriate sections of
[doc/INSTALL.md](doc/INSTALL.md).



## 4. Support

**Only latest released version of Snoopy is supported!**
Any version that is not the latest is not supported. Please do not submit any bugfix/feature/change
requests related to old and unsupported Snoopy versions.

* You should consult FAQ first: [doc/FAQ.md](doc/FAQ.md)
* Then you should search the Snoopy issue tracker: https://github.com/a2o/snoopy/issues
* Then google your issue up.
* If the problem persist up to this point, create a new issue:)



## 5. How does Snoopy actually work?

See [doc/internals/README.md](doc/internals/README.md)



## 6. Contributing to Snoopy development

See [doc/CONTRIBUTING.md](doc/CONTRIBUTING.md)



## 7. Credits

Development is currently located at the following URI:
    http://github.com/a2o/snoopy/

Snoopy Logger was originally created by:
     marius@umich.edu
        mbm@linux.com

Snoopy is currently maintained by:
    Bostjan Skufca, bostjan@a2o.si

List of contributors is available at the following locations:
- in the Snoopy's ChangeLog file;
- in the list of pull requests on GitHub.
