### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Init empty program install list, but always link with given library
#
noinst_PROGRAMS =



### Programs required by the test suite
#
noinst_PROGRAMS     += snoopy-test
snoopy_test_SOURCES  = \
    action-common.h \
    action-run.c \
    action-run.h \
    action-run-datasource.c \
    action-run-datasource.h \
    action-run-everything.c \
    action-run-everything.h \
    action-run-messageformat.c \
    action-run-messageformat.h \
    action-run-output.c \
    action-run-output.h \
    action-stress.c \
    action-stress.h \
    action-unit.c \
    action-unit.h \
    action-unit-action.c \
    action-unit-action.h \
    action-unit-action-log-syscall-exec.c \
    action-unit-action-log-syscall-exec.h \
    action-unit-action-log-message-dispatch.c \
    action-unit-action-log-message-dispatch.h \
    action-unit-error.c \
    action-unit-error.h \
    action-unit-outputregistry.c \
    action-unit-outputregistry.h \
    action-unit-util.c \
    action-unit-util.h \
    action-unit-util-syslog.c \
    action-unit-util-syslog.h \
    snoopy-test.c \
    $(top_builddir)/src/cli/cli-subroutines.c \
    $(top_builddir)/src/cli/cli-subroutines.h
snoopy_test_LDADD  = \
    $(top_builddir)/src/libsnoopy-test-cli.la

if CONFIGFILE_ENABLED
snoopy_test_SOURCES += \
    action-run-configfile.c \
    action-run-configfile.h \
    action-unit-ext-ini.c \
    action-unit-ext-ini.h
endif

if FILTERING_ENABLED
snoopy_test_SOURCES += \
    action-run-filter.c \
    action-run-filter.h \
    action-run-filterchain.c \
    action-run-filterchain.h \
    action-unit-filterregistry.c \
    action-unit-filterregistry.h
endif

if THREAD_SAFETY_ENABLED
snoopy_test_SOURCES += \
    action-stress-threads.c \
    action-stress-threads.h \
    action-stress-threadsexec.c \
    action-stress-threadsexec.h \
    action-unit-ext-list.c \
    action-unit-ext-list.h
endif

if DATASOURCE_ENABLED_systemd_unit_name
snoopy_test_SOURCES += \
    action-unit-util-systemd.c \
    action-unit-util-systemd.h
endif

noinst_PROGRAMS                    += spaceparent
spaceparent_SOURCES                 = spaceparent.c



### Create a program called "space parent", for testing filter exclude_spawns_of
#
all-local: spaceparent
	@cp spaceparent "space parent"

clean-local:
	rm -f "space parent"



### Create test library, whole
#
# This library reads path to snoopy.ini from SNOOPY_INI environmental variable.
#
noinst_LTLIBRARIES        = libsnoopy-test-execve-wrapper-env-ini-path.la
libsnoopy_test_execve_wrapper_env_ini_path_la_SOURCES =
libsnoopy_test_execve_wrapper_env_ini_path_la_LIBADD  = \
	$(top_builddir)/src/libsnoopy-no-entrypoint.la \
	$(top_builddir)/src/entrypoint/libsnoopy-entrypoint-execve-wrapper-test-configfile-env.la
#
# This needs to be added as automake does not create shared library for
# things in noinst_LTLIBRARIES.
#
libsnoopy_test_execve_wrapper_env_ini_path_la_LDFLAGS = -module -rpath /nowhere
