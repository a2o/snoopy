# Snoopy Logger Internals



## How does Snoopy actually work?

Snoopy installs itself as a preloaded library that provides a wrapper
around execv() and execve() syscall. Logging is done via syslog.

Snoopy should be completely transparent to users and to applications.



## Architectural note

execv() calls are now explicitly logged.   Although,  according to the
man page for execv(),  it is supposed to call execve().   To this date
the reason why execv()  calls weren't being logged is unknown,  but we
are working to find out why.



## Further information

* Read the code.
* Start at [../../src/libsnoopy.c](../../src/libsnoopy.c). This is essential.
    Here the execv()/execve() syscalls are intercepted and forwarded to internal
    Snoopy functions. Once Snoopy is done, execution is forwarded back to
    real execv()/execve() syscall.
* Continue with [../../src/log.c](../../src/log.c). Here you can learn how
    Snoopy log message is generated (using data sources) and dispatched to
    configured output.
* Configuration file parsing code is here: [../../src/configfile.c](../../src/configfile.c).
* Data sources are located here: [../../src/datasource/](../../src/datasource/).
* Filters are located here: [../../src/filter/](../../src/filter/).
* Outputs are located here: [../../src/output/](../../src/output/).
