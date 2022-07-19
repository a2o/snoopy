### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Create one library with all Snoopy utils collected in it
#
# Maintain alphabetical order, equal to what `ls` would do.
#
noinst_LTLIBRARIES = libsnoopy-utils.la
libsnoopy_utils_la_SOURCES = \
    file.c \
    file.h \
    pwd.c \
    pwd-snoopy.h \
    string.c \
    string-header.h


if DATASOURCE_ENABLED_systemd_unit_name
libsnoopy_utils_la_SOURCES += \
    systemd.c \
    systemd-snoopy.h
endif
