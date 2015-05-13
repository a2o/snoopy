# Makefile for snoopy
# $Id: Makefile,v 1.3 2000/12/10 08:50:49 marius Exp $

CC = gcc
LIBS  = -ldl

all: snoopy.so detect

snoopy.so:  snoopy.c snoopy.h
			$(CC) -shared -O3 -fomit-frame-pointer snoopy.c -osnoopy.so $(LIBS)
detect: detect.c
			$(CC) detect.c -odetect $(LIBS)
install: all 
			install -m 755 snoopy.so /lib/snoopy.so; \
			./install.sh
clean:
			rm detect snoopy.so
