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



### Variables to add final build targets to
#
noinst_LTLIBRARIES =



### Library-based entry points
#
# Maintain alphabetical order, equal to what `ls` would do.
#

# Production entry point
libsnoopy_entrypoint_execve_wrapper_la_SOURCES = \
	execve_wrapper.c \
	execve_wrapper.h
noinst_LTLIBRARIES += \
	libsnoopy_entrypoint_execve_wrapper.la

# Test-related entry point
libsnoopy_entrypoint_execve_wrapper_test_configfile_env_la_SOURCES = \
	execve_wrapper_test_configfile_env.c \
	execve_wrapper_test_configfile_env.h
noinst_LTLIBRARIES += \
	libsnoopy_entrypoint_execve_wrapper_test_configfile_env.la
