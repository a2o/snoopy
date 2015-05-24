include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I build/m4

if CONFIGFILE_ENABLED
install-exec-local:
	if [ ! -f $(DESTDIR)$(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "Installing configuration file: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	    if [ ! -e $(DESTDIR)$(sysconfdir) ]; then \
	        install -d $(DESTDIR)$(sysconfdir) ; \
	    fi ; \
	    install -m 644 snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini ; \
	else \
	    if diff -u snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini; then \
	        echo ; \
	        echo "No changes to configuration file are required: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	        echo ; \
	    else \
	        install -m 644 snoopy.ini $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW ; \
	        echo ; \
	        echo "Existing config file: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	        echo "     New config file: $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW" ; \
	        echo ; \
	        echo "WARNING: CONFIGURATION FILE CHANGES NEED MANUAL REVIEW" ; \
	        echo ; \
	    fi ; \
	fi ; \
	if [ -f $(DESTDIR)$(sysconfdir)/snoopy.ini ]; then \
	    if [ -x $(DESTDIR)$(sysconfdir)/snoopy.ini ]; then \
	        chmod 644 $(DESTDIR)$(sysconfdir)/snoopy.ini ; \
	    fi ; \
	fi ; \
	if [ -f $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW ]; then \
	    if [ -x $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW ]; then \
	        chmod 644 $(DESTDIR)$(sysconfdir)/snoopy.ini.NEW ; \
	    fi ; \
	fi ;

uninstall-local:
	if [ -f $(DESTDIR)$(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "Removing configuration file: $(DESTDIR)$(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	    rm -f $(DESTDIR)$(sysconfdir)/snoopy.ini ; \
	fi ;
endif
