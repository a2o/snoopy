### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Override certain inih defaults
#
AM_CFLAGS += -DINI_MAX_LINE=1024 -DINI_INITIAL_ALLOC=1024



### Create library "libinih"
#
noinst_LTLIBRARIES = \
    libinih.la

libinih_la_SOURCES = \
    ini.c \
    ini.h
