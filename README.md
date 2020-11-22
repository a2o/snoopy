# Snoopy Logger

[![Build Status - Master](https://travis-ci.org/a2o/snoopy.svg?branch=master)](https://travis-ci.org/a2o/snoopy/branches)
[![Coverity Scan](https://scan.coverity.com/projects/4973/badge.svg?flat=1)](https://scan.coverity.com/projects/4973)



## Table of contents

* [What is Snoopy?](#what-is-snoopy)
* [News](#news)
* [Latest version](#latest-version)
* [Installation](#installation)
* [Sample output](#sample-output)
* [Configuration](#configuration)
* [Security disclaimer](#security-disclaimer)
* [Contributing to Snoopy development](#contributing-to-snoopy-development)
* [Getting support](#getting-support)
* [License](#license)
* [Online resources](#online-resources)
* [Credits](#credits)



## What is Snoopy?

Snoopy is a small library that logs all program executions on your Linux/BSD system.

[Developer documentation](doc/INTERNALS.md) outlines how it actually does that (a fairly technical read).
And don't miss the [security disclaimer](#security-disclaimer) about it.



## News

| Date         | What?                    | Details   |
| ------------ | ------------------------ | --------- |
| 2020-11-13   | Snoopy 2.4.9 released!   | Maintenance release. On Friday 13th - let's hope for the best! :wink: |
| 2020-10-04   | Snoopy 2.4.8 released!   | Maintenance release. |
| 2020-10-04   | Snoopy 2.4.7 released!   | Maintenance release. |
| 2016-09-14   | Snoopy 2.4.6 released!   | Maintenance release. |

Consult [ChangeLog](ChangeLog) for more information.



## Latest version

| Version                      | Download URI                                                                     |
| ---------------------------- | -------------------------------------------------------------------------------- |
| Latest stable release        | https://github.com/a2o/snoopy/releases/download/snoopy-2.4.9/snoopy-2.4.9.tar.gz |
| Latest development release   | (Clone [this](https://github.com/a2o/snoopy.git) git repository)                 |

All historic releases can be found over there :point_right: in the [Releases](https://github.com/a2o/snoopy/releases) section.



## Installation

The fastest way to start using Snoopy is to execute the following commands (as root):
```shell
rm -f install-snoopy.sh &&
wget -O install-snoopy.sh https://github.com/a2o/snoopy/raw/install/install/install-snoopy.sh &&
chmod 755 install-snoopy.sh &&
./install-snoopy.sh stable
```
That is it.

Other installation options (i.e. installing latest development version from this
git repository) are described in the [doc/INSTALL.md](doc/INSTALL.md) file.



## Sample output

This is what typical Snoopy output looks like:

    2015-02-11T19:05:10+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/cat]: cat /etc/fstab.BAK
    2015-02-11T19:05:15+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/rm]: rm -f /etc/fstab.BAK
    2015-02-11T19:05:19+00:00 labrat-1 snoopy[896]: [uid:0 sid:11679 tty:/dev/pts/2 cwd:/root filename:/usr/bin/tail]: tail -f /var/log/messages

These are default output locations on various Linux distributions:

| Distribution | Snoopy output location | Notes                             |
| ------------ | ---------------------- |---------------------------------- |
| CentOS       | `/var/log/secure`      |                                   |
| Debian       | `/var/log/auth.log`    |                                   |
| Ubuntu       | `/var/log/auth.log`    |                                   |
| (others)     | `/var/log/messages`    | (potentially, could be elsewhere) |

For actual output destination check your syslog configuration.



## Configuration

If the configuration file support is available in your Snoopy build (it probably is),
Snoopy can be reconfigured on-the-fly.

The configuration file is (most likely, but depending on the build) located at `/etc/snoopy.ini`.

Explanations of available configuration directives are available in the [default configuration file](etc/snoopy.ini.in).



## Security disclaimer

**WARNING: Snoopy is not a reliable auditing solution.**

Rogue users can easily manipulate environment to avoid their actions being logged by Snoopy.
Consult [this FAQ entry](https://github.com/a2o/snoopy/blob/master/doc/FAQ.md#5-i-see-no-snoopy-output-after-initial-user-login) for more information.



## Contributing to Snoopy development

Consult the [CONTRIBUTING.md](.github/CONTRIBUTING.md) document for the process.

Consult [HACKING.md](doc/HACKING.md) and [INTERNALS.md](doc/INTERNALS.md) documents
for a brief description of internal structure and how all the pieces fit together.



## Getting support

Information is available in a [dedicated document about getting support](https://github.com/a2o/snoopy/blob/master/doc/GETTING-SUPPORT.md).



## License

Snoopy is released under [GNU General Public License version 2](https://github.com/a2o/snoopy/blob/master/COPYING).



## Online resources

Snoopy development is located at the following URI:
- https://github.com/a2o/snoopy/

Additional git repository mirrors (read-only) are available here:
- https://gitlab.com/a2o/snoopy/
- https://bitbucket.org/bostjan/snoopy/



## Credits

Snoopy Logger was originally created and maintained by:
- Marius Aamodt Eriksen <marius@umich.edu>
- Mike Baker <mbm@linux.com>

Contribution acknowledgements are available at the following locations:
- In the [ChangeLog](ChangeLog),
- In [pull requests](https://github.com/a2o/snoopy/pulls),
- In [git history](https://github.com/a2o/snoopy/commits/master).

Snoopy is currently maintained by [Bostjan Skufca Jese](https://github.com/bostjan).
