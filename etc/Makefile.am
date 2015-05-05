include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I build/m4

EXTRA_DIST +=\
    snoopy.ini

if CONFIG_FILE_ENABLED
install-exec-local:
	if [ ! -f $(DESTDIR)$(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "Installing configuration file: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	    if [ ! -e $(DESTDIR)$(sysconfdir) ]; then \
	        install -d $(DESTDIR)$(sysconfdir) ; \
	    fi ; \
	    install snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini ; \
	else \
	    if diff snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini; then \
	        echo ; \
	        echo "No changes to configuration file are required: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	        echo ; \
	    else \
	        install snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW ; \
	        echo ; \
	        echo "Existing config file: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	        echo "     New config file: $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW" ; \
	        echo ; \
	        echo "WARNING: CONFIGURATION FILE CHANGES NEED MANUAL REVIEW" ; \
	        echo ; \
	    fi \
	fi
endif
