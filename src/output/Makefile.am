### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all snoopy inputs collected in it
#
noinst_LTLIBRARIES = libsnoopy_outputs_all.la



### Input providers collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do
#
# TODO:
#	consoleoutput.c \
#	consoleoutput.h \
#	fileoutput.c \
#	fileoutput.h \
#	journaldoutput.c \
#	journaldout.h \
#
libsnoopy_outputs_all_la_SOURCES = \
	devlog.c \
	devlog.h \
	socketoutput.c \
	socketoutput.h \
	syslogoutput.c \
	syslogoutput.h
