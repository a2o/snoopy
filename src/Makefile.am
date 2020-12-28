### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
SUBDIRS  =
SUBDIRS += eventsource
SUBDIRS += datasource
if FILTERING_ENABLED
SUBDIRS += filter
endif
SUBDIRS += output



### Create main library, but without execve() wrappers
#
noinst_LTLIBRARIES = \
	libsnoopy_no_execve.la

libsnoopy_no_execve_la_SOURCES = \
	configuration.c \
	configuration.h \
	error.c \
	error.h \
	genericregistry.c \
	genericregistry.h \
	inputdatastorage.c \
	inputdatastorage.h \
	datasourceregistry.c \
	datasourceregistry.h \
	log.c \
	log.h \
	message.c \
	message.h \
	misc.c \
	misc.h \
	outputregistry.c \
	outputregistry.h \
	parser.c \
	parser.h \
	snoopy.h
libsnoopy_no_execve_la_LDFLAGS = $(AM_LDFLAGS)

libsnoopy_no_execve_la_LIBADD = \
	datasource/libsnoopy_datasources_all.la \
	output/libsnoopy_outputs_all.la

# If config file is enabled, build and link these too
if CONFIGFILE_ENABLED
libsnoopy_no_execve_la_SOURCES += \
	configfile.c \
	configfile.h
# Add topdir into header search path, for lib/... headers
configfile.o:  CFLAGS += -I$(top_srcdir)
configfile.lo: CFLAGS += -I$(top_srcdir)
libsnoopy_no_execve_la_LIBADD += \
	../lib/inih/src/libinih.la
endif

# If thread safety is enabled, build and link these too
if THREAD_SAFETY_ENABLED
libsnoopy_no_execve_la_SOURCES += \
	tsrm.c \
	tsrm.h
# Add topdir into header search path, for lib/... headers
tsrm.o:  CFLAGS += -I$(top_srcdir)
tsrm.lo: CFLAGS += -I$(top_srcdir)
libsnoopy_no_execve_la_LIBADD += \
	../lib/liblcthw/src/liblist.la
libsnoopy_no_execve_la_LDFLAGS += -lpthread
endif

# If filtering is enabled, add these too
if FILTERING_ENABLED
libsnoopy_no_execve_la_SOURCES += \
	filtering.c \
	filtering.h \
	filterregistry.c \
	filterregistry.h
libsnoopy_no_execve_la_LIBADD += \
	filter/libsnoopy_filters_all.la
endif



### Create main library, whole
#
lib_LTLIBRARIES      = libsnoopy.la
libsnoopy_la_SOURCES =
libsnoopy_la_LIBADD  = \
	libsnoopy_no_execve.la \
	eventsource/libsnoopy_eventsource_execve_wrapper.la



### Create debugging helper library
#
noinst_LTLIBRARIES                += libsnoopy-debug-addons.la
libsnoopy_debug_addons_la_SOURCES  = libsnoopy-debug-addons.c \
                                     libsnoopy-debug-addons.h
libsnoopy_debug_addons_la_LIBADD   = libsnoopy_no_execve.la
