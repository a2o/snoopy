# Snoopy Logger



## Table of contents

* [What is Snoopy?](#what-is-snoopy)
* [News](#news)
* [Latest version](#latest-version)
* [Installation](#installation)
* [Output](#output)
* [Configuration](#configuration)
* [FAQ - Frequently asked questions](#faq-frequently-asked-questions)
* [Security disclaimer](#security-disclaimer)
* [Contributing to Snoopy development](#contributing-to-snoopy-development)
* [Getting support](#getting-support)
* [License](#license)
* [Online resources](#online-resources)
* [Credits](#credits)



## What is Snoopy?

Snoopy is a small library that logs all program executions on your Linux/BSD system.

[Developer documentation](doc/HACKING-INTERNALS.md) outlines how it actually does that (a fairly technical read).
And don't miss the [security disclaimer](#security-disclaimer) about it.



## News

| Date         | What?                    | Details   |
| ------------ | ------------------------ | --------- |
| 2021-04-20   | Snoopy 2.4.14 released.  | Bugfix release (long commands can cause segmentation fault, [#198](https://github.com/a2o/snoopy/issues/198)). |
| 2021-04-14   | Snoopy 2.4.13 released.  | Minor enhancement release (added custom datetime formatting support). |
| 2021-02-09   | Snoopy 2.4.12 released.  | Maintenance release - fairly important, see [#191](https://github.com/a2o/snoopy/issues/191) for details. |
| 2021-02-02   | Snoopy 2.4.11 released.  | Maintenance release. |
| 2020-11-30   | Snoopy 2.4.10 released.  | <br/> **IMPORTANT NOTICE:** This Snoopy version contains a bugfix for a condition ([#119](https://github.com/a2o/snoopy/issues/119)) that can make your systems unstable in specific situations. <br/> **All users are encouraged to upgrade to this version of Snoopy ASAP**. |
| 2020-11-13   | Snoopy 2.4.9 released.   | Maintenance release. On Friday 13th - let's hope for the best! :wink: |
| 2020-10-04   | Snoopy 2.4.8 released.   | Maintenance release. |

Consult [ChangeLog](ChangeLog) for more information.



## Latest version

| Latest release | Version  | Status | Download location |
| -------------- | -------- | ------ | ----------------- |
| Stable         | 2.4.14   | [![Build QA - OS Matrix (Large) / stable](https://github.com/a2o/snoopy/workflows/Build%20QA%20-%20OS%20Matrix%20(Large)/badge.svg?branch=snoopy-2.4.14)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Build+QA+-+OS+Matrix%20(Large)%22+branch%3Asnoopy-%2A) <br/> [![Build QA - Config Matrix (Large) / stable](https://github.com/a2o/snoopy/workflows/Build%20QA%20-%20Config%20Matrix%20(Large)/badge.svg?branch=snoopy-2.4.14)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Build+QA+-+Config+Matrix+%28Large%29%22+branch%3Asnoopy-%2A) <br/> [![Code QA - Autoreconf / stable](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Autoreconf/badge.svg?branch=snoopy-2.4.14)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Autoreconf%22+branch%3Asnoopy-%2A) <br/> [![Code QA - Autoscan / stable](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Autoscan/badge.svg?branch=snoopy-2.4.14)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Autoscan%22+branch%3Asnoopy-%2A) <br/> [![Code QA - Valgrind / stable](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Valgrind/badge.svg?branch=snoopy-2.4.14)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Valgrind%22+branch%3Asnoopy-%2A) | All release packages can be found over there :point_right: in the [Releases](https://github.com/a2o/snoopy/releases) section. |
| Development    | `master` | [![Build QA - OS Matrix (Large) / master](https://github.com/a2o/snoopy/workflows/Build%20QA%20-%20OS%20Matrix%20(Large)/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Build+QA+-+OS+Matrix+%28Large%29%22+branch%3Amaster) <br/> [![Build QA - Config Matrix / master](https://github.com/a2o/snoopy/workflows/Build%20QA%20-%20Config%20Matrix%20(Large)/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Build+QA+-+Config+Matrix+%28Large%29%22+branch%3Amaster) <br/> [![Code QA - Autoreconf / master](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Autoreconf/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Autoreconf%22+branch%3Amaster) <br/> [![Code QA - Autoscan / master](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Autoscan/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Autoscan%22+branch%3Amaster) <br/> [![Code QA - Valgrind / master](https://github.com/a2o/snoopy/workflows/Code%20QA%20-%20Valgrind/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Code+QA+-+Valgrind%22+branch%3Amaster) <br/> [![Coverity Scan](https://scan.coverity.com/projects/4973/badge.svg?flat=1)](https://scan.coverity.com/projects/4973) <br/> SonarCloud: <br/> [![SonarCloud - Bugs](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=bugs)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Code Smells](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=code_smells)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Coverage](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=coverage)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=duplicated_lines_density)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=sqale_rating)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=alert_status)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=reliability_rating)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Security Rating](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=security_rating)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Technical Debt](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=sqale_index)](https://sonarcloud.io/dashboard?id=snoopy) <br/> [![SonarCloud - Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=snoopy&metric=vulnerabilities)](https://sonarcloud.io/dashboard?id=snoopy) | `git clone git:github.com:a2o/snoopy` |



## Installation

[![Install QA - README.md / master](https://github.com/a2o/snoopy/workflows/Install%20QA%20-%20README.md/badge.svg?branch=master)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Install+QA+-+README.md%22+branch%3Amaster)
[![Install QA - install-snoopy.sh / install](https://github.com/a2o/snoopy/workflows/Install%20QA%20-%20install-snoopy.sh/badge.svg?branch=install)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Install+QA+-+install-snoopy.sh%22+branch%3Ainstall)
[![Install QA - OS Matrix - Install from git](https://github.com/a2o/snoopy/workflows/Install%20QA%20-%20OS%20Matrix/badge.svg?branch=install)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Install+QA+-+OS+Matrix%22+branch%3Ainstall)
[![Release QA - OS Matrix - Install from source](https://github.com/a2o/snoopy/workflows/Release%20QA%20-%20OS%20Matrix%20-%20Install%20from%20source/badge.svg)](https://github.com/a2o/snoopy/actions?query=workflow%3A%22Release+QA+-+OS+Matrix+-+Install+from+source%22)

The simplest way to start using the latest Snoopy is to execute the following commands (as root):
```shell
wget -O install-snoopy.sh https://github.com/a2o/snoopy/raw/install/install/install-snoopy.sh &&
chmod 755 install-snoopy.sh &&
sudo ./install-snoopy.sh stable
```
That's all.

Other installation options (i.e. installing latest development version from this
git repository) are described in the [doc/INSTALL.md](doc/INSTALL.md) document.



## Output

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

For actual output format and destination, check your Snoopy and syslog configuration.



## Configuration

If the configuration file support is available in your Snoopy build (it probably is),
Snoopy can be reconfigured on-the-fly.

The configuration file is (most likely, but depending on the build) located at `/etc/snoopy.ini`.

Supported configuration directives are explained in the [default configuration file](etc/snoopy.ini.in).



## FAQ - Frequently asked questions

Frequently asked questions and answers are collected in the [doc/FAQ.md](https://github.com/a2o/snoopy/blob/master/doc/FAQ.md) file in this repository.



## Security disclaimer

**WARNING: Snoopy is not a reliable auditing solution.**

Rogue users can easily manipulate environment to avoid their actions being logged by Snoopy.
Consult [this FAQ entry](https://github.com/a2o/snoopy/blob/master/doc/FAQ.md#5-i-see-no-snoopy-output-after-initial-user-login) for more information.



## Contributing to Snoopy development

Consult the following documents for information related to Snoopy development:

- [CONTRIBUTING.md](.github/CONTRIBUTING.md)
- [HACKING-OVERVIEW.md](doc/HACKING-OVERVIEW.md)
- [HACKING-INTERNALS.md](doc/HACKING-INTERNALS.md)
- [HACKING-QA.md](doc/HACKING-QA.md) (Autoscan, Travis-CI and Valgrind-related sections)



## Getting support

Information is available in a [dedicated document about getting support](https://github.com/a2o/snoopy/blob/master/doc/GETTING-SUPPORT.md).



## License

Snoopy is released under [GNU General Public License version 2](https://github.com/a2o/snoopy/blob/master/COPYING).



## Online resources

Snoopy development is located at the following URI:
- https://github.com/a2o/snoopy/

Additional git repository mirrors (read-only) are available here:
- https://gitlab.com/a2o/snoopy/
- https://bitbucket.org/snoopylogger/snoopy/



## Credits

Snoopy Logger was originally created and maintained by:
- Marius Aamodt Eriksen <marius@umich.edu>
- Mike Baker <mbm@linux.com>

Contribution acknowledgements are available at the following locations:
- In the [ChangeLog](ChangeLog),
- In [pull requests](https://github.com/a2o/snoopy/pulls),
- In [git history](https://github.com/a2o/snoopy/commits/master).

Snoopy is currently maintained by [Bostjan Skufca Jese](https://github.com/bostjan).
