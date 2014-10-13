AM_CFLAGS = -I$(top_srcdir)/src


### Test program
#
# A program which takes input parameters to produce a log entry as if
#
bin_PROGRAMS = \
	snoopy-test-output
snoopy_test_output_SOURCES = \
	snoopy-test-output.c
snoopy_test_output_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la


noinst_PROGRAMS = \
	snoopy-dev-helper
snoopy_dev_helper_SOURCES = \
	snoopy-dev-helper.c
snoopy_dev_helper_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la

