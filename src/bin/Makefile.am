### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Init empty program install list, but always link with given library
#
noinst_PROGRAMS =
LDADD           = ../libsnoopy_no_execve.la



### Programs that are needed for testing suite
#
noinst_PROGRAMS += snoopy-test-datasource
if FILTERING_ENABLED
    noinst_PROGRAMS += snoopy-test-filter
endif
noinst_PROGRAMS += snoopy-test-output


snoopy_test_datasource_SOURCES = snoopy-test-datasource.c
#snoopy_test_datasource_LDADD   = ../libsnoopy_no_execve.la
if FILTERING_ENABLED
    snoopy_test_filter_SOURCES     = snoopy-test-filter.c
#    snoopy_test_filter_LDADD       = ../libsnoopy_no_execve.la
endif
snoopy_test_output_SOURCES     = snoopy-test-output.c
#snoopy_test_output_LDADD       = ../libsnoopy_no_execve.la
