#!/bin/sh

#installation script for snoopy (for system wide installs)
#Run as root!
#$Id: install.sh,v 1.3 2000/12/10 18:48:30 marius Exp $

[ -x /lib/snoopy.so ] && {
  touch /etc/ld.so.preload
  [ -w /etc/ld.so.preload ] && {
    [ `grep -c '/lib/snoopy.so'    /etc/ld.so.preload` == 0 ] && {
      echo     "/lib/snoopy.so" >> /etc/ld.so.preload
      echo "Installed"
    } || echo "snoopy already installed... updated to newest version"
  }   || echo "Failed, are you installing this as root?"
}     || echo "Failed, are you installing this as root?"
