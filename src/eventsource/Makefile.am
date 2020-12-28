### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Adjust cflags
#
# Exclude -Wpedantic from directory-level CFLAGS, because casting object pointer
# received from dlsym() to function pointer is not allowed by ISO C.
#
# Earlier we had this enabled on a per-make-target basis, like this:
#   libsnoopy_eventsource_execve_wrapper_la_CFLAGS = `echo $(AM_CFLAGS) | sed -e 's/-Wpedantic//'`
# Unfortunately, that produced prefixed .o|.gcna files, which were messing with
# our code coverage analysis.
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
