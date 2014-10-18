### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all snoopy filters collected in it
#
noinst_LTLIBRARIES = libsnoopy_filters_all.la



### Filter collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do
#
libsnoopy_filters_all_la_SOURCES = \
	exclude_uid.c \
	exclude_uid.h \
	only_root.c \
	only_root.h \
	only_uid.c \
	only_uid.h
