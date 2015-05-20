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
