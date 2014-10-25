include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src bin etc

EXTRA_DIST +=\
	autogen.sh \
	bin/snoopy-enable.sh

enable: install
	$(top_srcdir)/bin/snoopy-enable.sh $(libdir)
