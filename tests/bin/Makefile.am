### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Init empty program install list, but always link with given library
#
noinst_PROGRAMS =
LDADD           = ../../src/libsnoopy_no_execve.la



### Programs that are needed for testing suite
#
if CONFIGFILE_ENABLED
noinst_PROGRAMS                    += snoopy-test-configfile
snoopy_test_configfile_SOURCES      = snoopy-test-configfile.c
endif

noinst_PROGRAMS                    += snoopy-test-datasource
snoopy_test_datasource_SOURCES      = snoopy-test-datasource.c

if FILTERING_ENABLED
noinst_PROGRAMS                    += snoopy-test-filter
snoopy_test_filter_SOURCES          = snoopy-test-filter.c
endif

noinst_PROGRAMS                    += snoopy-test-message-format
snoopy_test_message_format_SOURCES  = snoopy-test-message-format.c

noinst_PROGRAMS                    += snoopy-test-output
snoopy_test_output_SOURCES          = snoopy-test-output.c

if THREAD_SAFETY_ENABLED
noinst_PROGRAMS                    += snoopy-test-exec-in-threads
snoopy_test_exec_in_threads_SOURCES = snoopy-test-exec-in-threads.c
noinst_PROGRAMS                    += snoopy-test-threads
snoopy_test_threads_SOURCES         = snoopy-test-threads.c
snoopy_test_threads_LDADD           = $(top_builddir)/src/libsnoopy-debug-addons.la
endif



### Create test library, whole
#
# Exclude -pedantic from CFLAGS because casing object pointer received from
# dlsym to function pointer is not allowed by ISO C.
#
# This library reads path to snoopy.ini from SNOOPY_INI environmental variable.
#
noinst_LTLIBRARIES        = libsnoopy-test.la
libsnoopy_test_la_SOURCES = libsnoopy-test.c
libsnoopy_test_la_CFLAGS  = `echo $(AM_CFLAGS) | sed -e 's/-pedantic//'`
libsnoopy_test_la_LIBADD  = $(top_builddir)/src/libsnoopy_no_execve.la
#
# This needs to be added as automake does not create shared library for
# things in noinst_LTLIBRARIES.
#
libsnoopy_test_la_LDFLAGS = -module -rpath /nowhere
