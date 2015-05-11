### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all snoopy outputs collected in it
#
noinst_LTLIBRARIES = libsnoopy_outputs_all.la



### Filter collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do.
#
libsnoopy_outputs_all_la_SOURCES =

# Output: devlog
#
if OUTPUT_ENABLED_devlog
libsnoopy_outputs_all_la_SOURCES += \
    devlogoutput.c \
    devlogoutput.h
endif

# Output: file
#
if OUTPUT_ENABLED_file
libsnoopy_outputs_all_la_SOURCES += \
    fileoutput.c \
    fileoutput.h
endif

# Output: socket
#
# This one is always enabled, unconditionally
#
libsnoopy_outputs_all_la_SOURCES += \
    socketoutput.c \
    socketoutput.h

# Output: syslog
#
if OUTPUT_ENABLED_syslog
libsnoopy_outputs_all_la_SOURCES += \
    syslogoutput.c \
    syslogoutput.h
endif
