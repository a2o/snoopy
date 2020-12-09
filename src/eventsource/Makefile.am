### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Adjust cflags
#
# Exclude -pedantic from CFLAGS because casing object pointer received from
# dlsym to function pointer is not allowed by ISO C.
#
# Before we had this, but it produced a prefixed .o|.gcna files, which were
# messing with our code coverage analysis:
#   libsnoopy_eventsource_execve_wrapper_la_CFLAGS = `echo $(AM_CFLAGS) | sed -e 's/-pedantic//'`
#
AM_CFLAGS += -Wno-pedantic



### Create one library with all Snoopy data sources collected in it
#
noinst_LTLIBRARIES =



### Event sources
#
# Please maintain alphabetical order, equal to what `ls` would do.
#

# Production event source
libsnoopy_eventsource_execve_wrapper_la_SOURCES = \
	execve_wrapper.c \
	execve_wrapper.h
noinst_LTLIBRARIES += \
	libsnoopy_eventsource_execve_wrapper.la

# Test-related event source
libsnoopy_eventsource_execve_wrapper_test_configfile_env_la_SOURCES = \
	execve_wrapper_test_configfile_env.c \
	execve_wrapper_test_configfile_env.h
noinst_LTLIBRARIES += \
	libsnoopy_eventsource_execve_wrapper_test_configfile_env.la
