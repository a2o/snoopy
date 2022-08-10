### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Variables to add final targets to
#
noinst_LTLIBRARIES =
sbin_PROGRAMS =



### Program-based entry points
#
sbin_PROGRAMS += snoopyctl
snoopyctl_SOURCES = \
	action-conf.c \
	action-conf.h \
	action-disable.c \
	action-disable.h \
	action-enable.c \
	action-enable.h \
	action-status.c \
	action-status.h \
	action-version.c \
	action-version.h \
	cli-subroutines.c \
	cli-subroutines.h \
	snoopyctl.c
snoopyctl_LDADD = \
	../util/libsnoopy-utils.la
