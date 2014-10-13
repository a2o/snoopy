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
