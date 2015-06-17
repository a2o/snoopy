### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Use includes from src/ directory
#
AM_CFLAGS += -I$(top_srcdir)/src

bin_PROGRAMS    =
sbin_SCRIPTS    =
noinst_LTLIBRARIES =
noinst_PROGRAMS =



### Scripts to enable/disable Snoopy
#
sbin_SCRIPTS += snoopy-enable
sbin_SCRIPTS += snoopy-disable



### Test programs
#
# Some development/testing programs here
#

if DEV_TOOLS_ENABLED

bin_PROGRAMS                         += snoopy-test-output
snoopy_test_output_SOURCES            = snoopy-test-output.c
snoopy_test_output_LDADD              = ../src/libsnoopy_no_execve.la

noinst_PROGRAMS                      += snoopy-detect
snoopy_detect_SOURCES                 = snoopy-detect.c

noinst_PROGRAMS                      += snoopy-test-all-data-sources
snoopy_test_all_data_sources_SOURCES  = snoopy-test-all-data-sources.c
snoopy_test_all_data_sources_LDADD    = ../src/libsnoopy_no_execve.la \
                                        ../src/libsnoopy-debug-addons.la

noinst_PROGRAMS                      += snoopy-run-everything
snoopy_run_everything_SOURCES         = snoopy-run-everything.c
snoopy_run_everything_LDADD           = ../src/libsnoopy_no_execve.la \
                                        ../src/libsnoopy-debug-addons.la

noinst_PROGRAMS                      += snoopy-dev-helper
snoopy_dev_helper_SOURCES             = snoopy-dev-helper.c
snoopy_dev_helper_LDADD               = ../src/libsnoopy_no_execve.la

endif
