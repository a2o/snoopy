# Snoopy Logger

Snoopy is a tiny library that logs all executed commands (+ arguments) on your system.
[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

Master:
[![Build Status - Master](https://travis-ci.org/a2o/snoopy.svg?branch=master)](https://travis-ci.org/a2o/snoopy/branches)
[![Coverity Scan](https://scan.coverity.com/projects/4973/badge.svg?flat=1)](https://scan.coverity.com/projects/4973)

Stable:
[![Build Status - Stable](https://travis-ci.org/a2o/snoopy.svg?branch=stable)](https://travis-ci.org/a2o/snoopy/branches)

Chat:
[![Join the chat at https://gitter.im/a2o/snoopy](https://badges.gitter.im/a2o/snoopy.svg)](https://gitter.im/a2o/snoopy?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

INFO: Snoopy is not a reliable auditing solution. Rogue users can easily manipulate
environment to avoid logging by Snoopy. See [this FAQ entry](https://github.com/a2o/snoopy/blob/master/doc/FAQ.md#5-i-see-no-snoopy-output-after-initial-user-login).



## Table of contents

  * [Snoopy Logger](#snoopy-logger)
    * [Table of contents](#table-of-contents)
    * [News](#news)
    * [Installation](#installation)
      * [I.1 Version information and download links](#i1-version-information-and-download-links)
    * [Output](#output)
    * [Configuration](#configuration)
    * [Support](#support)
      * [S.1 Keep all communication public](#s1-keep-all-communication-public)
      * [S.2 Supported Snoopy versions](#s2-supported-snoopy-versions)
      * [S.3 Reporting bugs](#s3-reporting-bugs)
      * [S.4 Feature requests](#s4-feature-requests)
      * [S.5 Free support](#s5-free-support)
      * [S.6 Commercial support](#s6-commercial-support)
    * [Contributing to Snoopy development](#contributing-to-snoopy-development)
    * [License](#license)
    * [Credits](#credits)



## News

2016-09-14:
**Snoopy 2.4.6 released!**
Maintenance release.

2016-03-05:
**Snoopy 2.4.5 released!**
Maintenance release.

2015-08-28:
**Snoopy 2.4.4 released!**
Mainly maintenance release, with new filter: only_tty, which should cut down
noise from non-tty processes considerably.

2015-06-18:
**Snoopy 2.4.0 released!**
Many changes, see the ChangeLog. Experimental thread safety added.
All users are encouraged to upgrade to Snoopy version 2.4.0+ immediately.



## Installation

The easiest way to start using Snoopy is to execute one of the following
commands (as root).

To install the latest **STABLE** version of Snoopy, use this command:

    rm -f snoopy-install.sh &&
    wget -O snoopy-install.sh https://github.com/a2o/snoopy/raw/install/doc/install/bin/snoopy-install.sh &&
    chmod 755 snoopy-install.sh &&
    ./snoopy-install.sh stable

To install the latest **DEVELOPMENT** version of Snoopy, use this:

    rm -f snoopy-install.sh &&
    wget -O snoopy-install.sh https://github.com/a2o/snoopy/raw/install/doc/install/bin/snoopy-install.sh &&
    chmod 755 snoopy-install.sh &&
    ./snoopy-install.sh git-master

That is it.
Detailed installation instructions are available here: [doc/INSTALL.md](doc/INSTALL.md)


### I.1 Version information and download links

| Version                      | Download URI                                               |
| ---------------------------- | ---------------------------------------------------------- |
| Latest stable release        | http://source.a2o.si/download/snoopy/snoopy-2.4.6.tar.gz   |
| Latest development release   | (N/A, clone this git repo, use master branch)              |
| All releases                 | http://source.a2o.si/download/snoopy/                      |




## Output

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



## Configuration

Most parts of Snoopy are/can be/should be configured at build time.

However, since version 2.0.0 Snoopy supports optional configuration file.
Snoopy's automated installation procedure enables configuration file support
by default. Configuration file path is /etc/snoopy.ini.

For additional information consult comments in default configuration file
[etc/snoopy.ini](etc/snoopy.ini.in) and read appropriate sections of
[doc/INSTALL.md](doc/INSTALL.md).



## Support

### S.1 Keep all communication public

Due to the fact that you need support, there is a good chance someone else will
stumble upon the same issue soon too. For this reason it is crucial that all
communication is kept public. Search engines can then pick this content up,
index it, and hopefully present it to the next soul with the same problem.

GitHub is currently *the* meeting point for all Snoopy-related content.
Therefore you should use GitHub issues for any kind of communication about
Snoopy, with the sole exception of commercial support - that should be the only
reason for contacting maintainer directly via email.

### S.2 Supported Snoopy versions

**Only latest released version of Snoopy is "supported"!**
Any version that is not the latest (stable or development) is not supported.
Please do not submit any bugfix/feature/change requests related to old and thus
unsupported Snoopy versions.


### S.3 Reporting bugs

**If you encounter an issue with Snoopy, proceed with the following steps:**

1. make sure you are using the latest version of Snoopy!
  1. this DOES NOT mean the last version 'apt-get install snoopy' provides,
  2. this means the latest version available at https://github.com/a2o/snoopy/
2. consult FAQ first: [doc/FAQ.md](doc/FAQ.md),
3. search the internets (using Google for example),
4. search the Snoopy issue tracker: https://github.com/a2o/snoopy/issues/,
5. if the problem still persist, create a new issue on GitHub, but you must:
  1. make sure your issue is reproducible using the latest version of Snoopy,
  2. provide the following data:
    1. Snoopy version used and how did you install it?
    2. Linux distribution you are using?
    3. Which software is crashing because of Snoopy (whole system, one program)?
    4. Trace of your program that crashes.
  3. if possible, provide a patch (by creating a pull request on GitHub) that
        fixes (or at least tries to fix) the reported issue.


### S.4 Feature requests

"I need Snoopy to do this and that!"

Heard too many times. This is Open Source software. It is created, maintained
and contributed to by people who donate their time and expertise to the project.
Therefore donated time is primarily spent on topics of their interest,
so naturally Snoopy is subjected to FOSS toilet paradox: designing a futuristic
toilet is fun, but fixing it when it gets clogged is something no one is willing
to do for free. You can read the extensive explanation of this analogy here:
http://blog.enfranchisedmind.com/2009/07/fyi-my-open-source-users/

If you need a specific feature that is currently not implemented, you have three
options:

1. Implement it yourself. Once done, you can:
  1. contribute it back to Snoopy by creating a pull request - you are welcome to do so;
  2. keep your code private, no one is stopping you (but make sure you comply with license restrictions).
2. Ask someone (who has required expertise) to do it. Payment may be expected.
3. Commercial support.

**However**, if you think you have came up with a very nice feature, but you
do not know how to implement it, yet you think other people might find use for
it, do not hesitate and open an Issue on GitHub: https://github.com/a2o/snoopy/issues
That way the idea will be kept around and maybe someone will pick it up and
implement it.


### S.5 Free support

Use GitHub Issues: https://github.com/a2o/snoopy/issues
Do not send email to maintainers for free support.


### S.6 Commercial support

Commercial support for Snoopy is available. Contact current maintainer via email
for details.



## Contributing to Snoopy development

See [CONTRIBUTING.md](CONTRIBUTING.md).

See [doc/internals/README.md](doc/internals/README.md) for information about
internal structure of Snoopy and how all pieces fit together.



## License

Snoopy is released under GNU General Public License version 2.

Snoopy was originally released under GNU GPLv2 license, and during the course of
it's life various people contributed code to it under the same license. Therefore
Snoopy continues to be released under GNU GPLv2 license, as this is the only
license all contributors agreed to, up to this point.

Should some entity (individual or corporation) desire to obtain Snoopy under a
different license (commercial, for example), a consent from all contributors
will be required.



## Credits

Snoopy development is currently located at the following URI:
    http://github.com/a2o/snoopy/

Additional git mirrors (read-only) are available here:
    https://gitlab.com/a2o/snoopy/
    https://bitbucket.org/bostjan/snoopy/
    https://git.teon.si/a2o/snoopy/

Snoopy Logger was originally created and maintained by:
    Marius Aamodt Eriksen <marius@umich.edu>
    Mike Baker <mbm@linux.com>

List of contributors is available at the following locations:
- in the Snoopy's ChangeLog file;
- in the list of pull requests on GitHub.

Snoopy is currently maintained by:
    Bostjan Skufca <bostjan@a2o.si>
