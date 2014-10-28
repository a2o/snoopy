### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### First process all required subdirs
#
SUBDIRS = lib filter input



### Create main library, but without execve() wrappers
#
noinst_LTLIBRARIES = \
	libsnoopy_no_execve.la

libsnoopy_no_execve_la_SOURCES = \
	configuration.c \
	configuration.h \
	error.c \
	error.h \
	filterregistry.c \
	filterregistry.h \
	inputdatastorage.c \
	inputdatastorage.h \
	inputregistry.c \
	inputregistry.h \
	log.c \
	log.h \
	misc.c \
	misc.h \
	snoopy.h

libsnoopy_no_execve_la_LIBADD = \
	lib/libiniparser.la \
	filter/libsnoopy_filters_all.la \
	input/libsnoopy_inputs_all.la



### Create main library, whole
#

lib_LTLIBRARIES = \
	libsnoopy.la

libsnoopy_la_SOURCES = \
	libsnoopy.c

libsnoopy_la_LIBADD = \
	libsnoopy_no_execve.la
