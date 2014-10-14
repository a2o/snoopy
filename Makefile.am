include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src bin

EXTRA_DIST +=\
	autogen.sh \
	enable.sh

enable: install
	$(top_srcdir)/enable.sh $(libdir)
