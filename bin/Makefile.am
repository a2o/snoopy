### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Use includes from src/ directory
#
AM_CFLAGS += -I$(top_srcdir)/src



### Scripts to enable/disable snoopy
#
sbin_SCRIPTS = \
	snoopy-enable \
	snoopy-disable



### Test programs
#
# Some development/testing programs here
#
bin_PROGRAMS = \
	snoopy-test-output
snoopy_test_output_SOURCES = \
	snoopy-test-output.c
snoopy_test_output_LDADD = \
	../src/libsnoopy_no_execve.la


noinst_PROGRAMS = \
	snoopy-detect \
	snoopy-dev-helper \
	snoopy-test-all-data-sources

snoopy_detect_SOURCES = \
	snoopy-detect.c

snoopy_dev_helper_SOURCES = \
	snoopy-dev-helper.c
snoopy_dev_helper_LDADD = \
	../src/libsnoopy_no_execve.la

snoopy_test_all_data_sources_SOURCES = \
	snoopy-test-all-data-sources.c
snoopy_test_all_data_sources_LDADD = \
	../src/libsnoopy_no_execve.la
