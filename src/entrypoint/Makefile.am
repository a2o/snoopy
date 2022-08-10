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



### Library entry points
#
# Maintain alphabetical order, equal to what `ls` would do.
#

# Production entry point (for loaded library)
noinst_LTLIBRARIES += libsnoopy-entrypoint-execve-wrapper.la
libsnoopy_entrypoint_execve_wrapper_la_SOURCES = \
    execve-wrapper.c \
    execve-wrapper.h

# Production entry point (for `snoopyctl` CLI utility)
noinst_LTLIBRARIES += libsnoopy-entrypoint-cli.la
libsnoopy_entrypoint_cli_la_SOURCES = \
    cli.c \
    cli.h

# Test-related entry point (for loaded library)
noinst_LTLIBRARIES += libsnoopy-entrypoint-execve-wrapper-test-configfile-env.la
libsnoopy_entrypoint_execve_wrapper_test_configfile_env_la_SOURCES = \
    execve-wrapper-test-configfile-env.c \
    execve-wrapper-test-configfile-env.h

# Test-related entry point (for `snoopy-test` CLI utility)
noinst_LTLIBRARIES += libsnoopy-entrypoint-test-cli.la
libsnoopy_entrypoint_test_cli_la_SOURCES = \
    test-cli.c \
    test-cli.h
