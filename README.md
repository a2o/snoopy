# Snoopy Logger

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)



## 1. What does Snoopy do?

It logs all commands executed on your system.



## 2. Installation

The easiest way to start using Snoopy is to execute the following command (as root):

    wget -q -O - https://github.com/a2o/snoopy/raw/master/bin/snoopy-install.sh | sh

That is it.
Detailed installation instructions are available here: [doc/INSTALL.md](doc/INSTALL.md)



## 3. Snoopy output

These are default destinations on various Linux distributions:

* CentOS: /var/log/secure
* Debian: /var/log/auth.log
* Ubuntu: /var/log/auth.log
* others: /var/log/messages (potentially, not necessarily)

For actual destination check your syslog configuration.



## 4. Configuration

Most parts of Snoopy are/can be/should be configured at build time.

However, since version 2.0.0 Snoopy supports optional configuration file.
Snoopy's automated installation procedure enables configuration file support
by default. Configuration file path is /etc/snoopy.ini.

For additional information please consult comments in /etc/snoopy.ini and
[doc/INSTALL.md](doc/INSTALL.md).



## 5. Support

**Only latest released version of Snoopy is supported!**
Any version that is not the latest is not supported. Please do not submit any bugfix/feature/change
requests related to old and unsupported Snoopy versions.

You should consult FAQ first: [doc/FAQ.md](doc/FAQ.md).
Then you should search the Snoopy issue tracker: https://github.com/a2o/snoopy/issues
Then google your issue up.
If the problem persist up to this point, create a new issue:)



## 6. How does Snoopy actually work?

See [doc/INTERNALS.md](doc/INTERNALS.md).



## 7. Contributing to Snoopy development

See [doc/CONTRIBUTING.md](doc/CONTRIBUTING.md).



## 8. Credits

Snoopy Logger was created by:
     marius@umich.edu
        mbm@linux.com

Snoopy is currently maintained by:
    Bostjan Skufca, bostjan@a2o.si

Development is located at the following URI (see pull requests for contributor credits):
    http://github.com/a2o/snoopy/
