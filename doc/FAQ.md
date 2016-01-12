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





### 4. Was legacy Snoopy faster than current implementation?

This was tested with Snoopy 2.3.0.

We compared the following three implementations:
    - current implementation, version 2.3.0, with enabled config file (missing), filtering, and with dynamic log message generation
    - legacy Snoopy, version 1.9.0, where message is generated in single pass (no function calling)
    - legacy Snoopy with output directly to console (to avoid syslog())

Results (loop with 2000 iterations, on Macbook Air 6.2 with i5-4250U):
    - 2.3.0: 3.6s (with config file and filtering enabled)
    - 2.3.0: 3.5s
    - 1.9.0: 3.2s
    - 1.9.0: 3.0s (output to console)

This 10% difference is insignificant. It seems forking is the most expensive
operation, which is not unexpected.

This is the reason why we removed implementation of data source called 'legacy'
from Snoopy, as maintaining redundant code seems pointless for 10% performance
gain.





### 5. I see no Snoopy output after initial user login

Your user probably has LD_PRELOAD environmental variable set to something non-
empty in their profile script. Remove it, or set libsnoopy.so as part of that
preload environment variable, like this (UNTESTED):

LD_PRELOAD="/path/to/libsnoopy.so /path/to/otherlib.so"





### 6. How do I go about developing new data source/filter/output?

Please see [HACKING.md](HACKING.md) for more information.



### 7. I want to contribute - how should I do it?

Contributions are welcome. Please see [../CONTRIBUGING.md](CONTRIBUTING.md) for
additional details about how to properly submit patches so they are merged as
fast as possible.
If your contribution is refactoring Snoopy's internal structure, it might take
a while to review it. Adding new data source, filter or output does not count
as refactoring.



### 8. I do not see any non-root Snoopy entries in my logs

If you have configured `file` output provider, make sure it has proper permissions
set for writing into it. This usually means chmodding it to 0666 which is quite
insecure setting. You may want to reconfigure Snoopy to use `devlog` output.
