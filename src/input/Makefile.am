### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all snoopy inputs collected in it
#
noinst_LTLIBRARIES = libsnoopy_inputs_all.la



### Input providers collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do
#
libsnoopy_inputs_all_la_SOURCES = \
	cmdline.c \
	cmdline.h \
	cwd.c \
	cwd.h \
	egid.c \
	egid.h \
	egroup.c \
	egroup.h \
	env.c \
	env.h \
	euid.c \
	euid.h \
	eusername.c \
	eusername.h \
	filename.c \
	filename.h \
	gid.c \
	gid.h \
	group.c \
	group.h \
	login.c \
	login.h \
	sid.c \
	sid.h \
	tty.c \
	tty.h \
	tty_uid.c \
	tty_uid.h \
	tty_username.c \
	tty_username.h \
	uid.c \
	uid.h \
	username.c \
	username.h
