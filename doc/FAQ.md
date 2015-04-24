# Snoopy Logger FAQ





### 1. Snoopy hangs my RedHat/Fedora/CentOS/other system

**OBSOLETE, FIXED IN SNOOPY 1.9.0**

Due to bug described at the URIs below, **please make sure you test your Snoopy
installation before you deploy it to production**. The bug manifests itself at
shutdown, when system hangs instead of finishing the whole shutdown procedure.

So far this has only been reproduced on Fedora, and there is a reference to
Hyper-Threading, but no one has come up with a valid solution, only workarounds.


Bug descriptions:

* https://bugzilla.redhat.com/show_bug.cgi?id=745603
* https://github.com/a2o/snoopy/issues/1


How to test:

* Configure, build, install, enable
* Make sure it is working by tailing log files
* Reboot #1
* Reboot #2. If second reboot is successful, you are on the safe side


Distributions known to work OK (all 64-bit, all tested with 2.0.0+ versions):

* CentOS 6.5
* Debian 7
* Ubuntu Server 12.04.01, 14.04
* Slackware 12.2+





### 2. Snoopy hangs my RH/CentOS/(other-systemd-based) system

**HAPPY ANNOUNCEMENT: Snoopy version 2.2.0 resolved boot problems on RHEL/CentOS 7.**
More info: https://github.com/a2o/snoopy/issues/28

~DO NOT INSTALL Snoopy on CentOS 7 just yet!~~
~For currently unknown reason, when Snoopy is installed, CentOS 7 does not boot.~~
~Here is the issue tracker entry:~~
~https://github.com/a2o/snoopy/issues/28~~





### 3. PulseAudio fills logs when Snoopy is installed

Issue was originally reported here: https://github.com/a2o/snoopy/issues/29

Transcript of original content (a bit condensed):

    If, like us, you have users running desktop environments, you'll find pulseaudio
    filling your logs because of Snoopy.

    Workaround:
    * edit /etc/pulse/client.conf
    * if exists, uncomment line that starts with "autospawn" and change value to "no"
    * if not, add a line "autospawn = no"

    That should fix your issue.

(Thanks to https://github.com/sykosoft for reporting this and providing a solution.)
