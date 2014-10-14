AM_CFLAGS = -I$(top_srcdir)/src


### Test programs
#
# Some development/testing programs here
#
bin_PROGRAMS = \
	snoopy-test-output
snoopy_test_output_SOURCES = \
	snoopy-test-output.c
snoopy_test_output_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la


noinst_PROGRAMS = \
	snoopy-detect \
	snoopy-dev-helper

snoopy_detect_SOURCES = \
	snoopy-detect.c

snoopy_dev_helper_SOURCES = \
	snoopy-dev-helper.c
snoopy_dev_helper_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la
