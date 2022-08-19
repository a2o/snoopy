### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
# `util` subdir must be the first to get processed, as `cli` depends on it
SUBDIRS  =
SUBDIRS += util
SUBDIRS += action
SUBDIRS += cli
SUBDIRS += entrypoint
SUBDIRS += datasource
if FILTERING_ENABLED
SUBDIRS += filter
endif
SUBDIRS += output



### Create the main library, but without an entrypoint (i.e. execve() wrapper)
#
noinst_LTLIBRARIES = \
	libsnoopy-no-entrypoint.la

libsnoopy_no_entrypoint_la_SOURCES = \
	configuration.c \
	configuration.h \
	error.c \
	error.h \
	genericregistry.c \
	genericregistry.h \
	init-deinit.c \
	init-deinit.h \
	inputdatastorage.c \
	inputdatastorage.h \
	datasourceregistry.c \
	datasourceregistry.h \
	message.c \
	message.h \
	outputregistry.c \
	outputregistry.h \
	snoopy.h
libsnoopy_no_entrypoint_la_LDFLAGS = $(AM_LDFLAGS)

libsnoopy_no_entrypoint_la_LIBADD = \
    action/libsnoopy-actions-all.la \
    datasource/libsnoopy_datasources_all.la \
    output/libsnoopy_outputs_all.la \
    util/libsnoopy-utils.la

# If config file is enabled, build and link these too
if CONFIGFILE_ENABLED
libsnoopy_no_entrypoint_la_SOURCES += \
	configfile.c \
	configfile.h
libsnoopy_no_entrypoint_la_LIBADD += \
	../lib/inih/src/libinih.la
endif

# If thread safety is enabled, build and link these too
if THREAD_SAFETY_ENABLED
libsnoopy_no_entrypoint_la_SOURCES += \
	tsrm.c \
	tsrm.h
libsnoopy_no_entrypoint_la_LDFLAGS += -lpthread
endif

# If filtering is enabled, add these too
if FILTERING_ENABLED
libsnoopy_no_entrypoint_la_SOURCES += \
	filtering.c \
	filtering.h \
	filterregistry.c \
	filterregistry.h
libsnoopy_no_entrypoint_la_LIBADD += \
	filter/libsnoopy_filters_all.la
endif



### Create the main library
#
lib_LTLIBRARIES = libsnoopy.la
libsnoopy_la_SOURCES =
libsnoopy_la_LIBADD  = \
    libsnoopy-no-entrypoint.la \
    entrypoint/libsnoopy-entrypoint-execve-wrapper.la \
    entrypoint/libsnoopy-entrypoint-cli.la



### Create a library to be included in `snoopy-test` CLI utility
#
noinst_LTLIBRARIES += libsnoopy-test-cli.la
libsnoopy_test_cli_la_SOURCES =
libsnoopy_test_cli_la_LIBADD  = \
    libsnoopy-no-entrypoint.la \
    cli/libsnoopy-cli-subroutines.la \
    entrypoint/libsnoopy-entrypoint-test-cli.la
