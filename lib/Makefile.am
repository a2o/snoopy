### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### INI file parser
#
if CONFIGFILE_ENABLED
noinst_LTLIBRARIES = \
    libiniparser.la

libiniparser_la_SOURCES = \
    iniparser/src/dictionary.c \
    iniparser/src/dictionary.h \
    iniparser/src/iniparser.c \
    iniparser/src/iniparser.h
endif
