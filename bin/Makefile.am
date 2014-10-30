AM_CFLAGS = -I$(top_srcdir)/src



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
	$(top_srcdir)/src/libsnoopy_no_execve.la


noinst_PROGRAMS = \
	snoopy-detect \
	snoopy-dev-helper \
        snoopy-test-all-inputs

snoopy_detect_SOURCES = \
	snoopy-detect.c

snoopy_dev_helper_SOURCES = \
	snoopy-dev-helper.c
snoopy_dev_helper_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la

snoopy_test_all_inputs_SOURCES = \
	snoopy-test-all-inputs.c
snoopy_test_all_inputs_LDADD = \
	$(top_srcdir)/src/libsnoopy_no_execve.la
