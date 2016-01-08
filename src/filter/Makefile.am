### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all Snoopy filters collected in it
#
noinst_LTLIBRARIES = libsnoopy_filters_all.la



### Filter collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do
#
libsnoopy_filters_all_la_SOURCES = \
	noop.c \
	noop.h



### Filter: exclude_spawns_of
#
if FILTER_ENABLED_exclude_spawns_of
libsnoopy_filters_all_la_SOURCES += \
	exclude_spawns_of.c \
	exclude_spawns_of.h
endif

### Filter: exclude_uid
#
if FILTER_ENABLED_exclude_uid
libsnoopy_filters_all_la_SOURCES += \
	exclude_uid.c \
	exclude_uid.h
endif

### Filter: only_root
#
if FILTER_ENABLED_only_root
libsnoopy_filters_all_la_SOURCES += \
	only_root.c \
	only_root.h
endif

### Filter: only_tty
#
if FILTER_ENABLED_only_tty
libsnoopy_filters_all_la_SOURCES += \
	only_tty.c \
	only_tty.h
endif

### Filter: only_uid
#
if FILTER_ENABLED_only_uid
libsnoopy_filters_all_la_SOURCES += \
	only_uid.c \
	only_uid.h
endif
