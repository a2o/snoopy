#!/bin/sh
[ -x snoopy.so ] && {
  touch /etc/ld.so.preload
  [ -w /etc/ld.so.preload ] && {
    [ `grep -c '/lib/snoopy.so'    /etc/ld.so.preload` == 0 ] && {
      echo     "/lib/snoopy.so" >> /etc/ld.so.preload
      echo "Installed"
    } || echo "snoopy already installed... updated to newest version"
  }   || echo "Failed, are you installing this as root?"
}     || echo "Failed, are you installing this as root?"
