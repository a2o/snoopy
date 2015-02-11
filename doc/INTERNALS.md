# Snoopy Logger Internals



### How does Snoopy actually work?

Snoopy installs itself as a preloaded library that provides a wrapper around
execv() and execve() syscall. Logging is done via syslog.

Snoopy should be completely transparent to users and to applications.



### Architectural note

execv() calls are now explicitly logged.   Although,  according to the
man page for execv(),  it is supposed to call execve().   To this date
the reason why execv()  calls weren't being logged is unknown,  but we
are working to find out why.



### Further information

Read the code.
Start at [src/libsnoopy.c](src/libsnoopy.c).
Continue with [src/log.c](src/log.c).
