#!/bin/sh

#installation script for snoopy (for system wide installs)
#Run as root!
#$Id: install.sh,v 1.2 2000/08/19 02:25:31 marius Exp $

[ -x snoopy.so ] && {
  touch /etc/ld.so.preload
  [ -w /etc/ld.so.preload ] && {
    [ `grep -c '/lib/snoopy.so'    /etc/ld.so.preload` == 0 ] && {
      echo     "/lib/snoopy.so" >> /etc/ld.so.preload
      echo "Installed"
    } || echo "snoopy already installed... updated to newest version"
  }   || echo "Failed, are you installing this as root?"
}     || echo "Failed, are you installing this as root?"
