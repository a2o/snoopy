### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all Snoopy data sources collected in it
#
noinst_LTLIBRARIES = libsnoopy_eventsource_execve_wrapper.la



### Event sources
#
# Please maintain alphabetical order, equal to what `ls` would do.
#
libsnoopy_eventsource_execve_wrapper_la_SOURCES = \
	execve_wrapper.c \
	execve_wrapper.h
libsnoopy_eventsource_execve_wrapper_la_LDFLAGS =
#
# Exclude -pedantic from CFLAGS because casing object pointer received from
# dlsym to function pointer is not allowed by ISO C.
#
libsnoopy_eventsource_execve_wrapper_la_CFLAGS = `echo $(AM_CFLAGS) | sed -e 's/-pedantic//'`
