# Makefile for snoopy
# $Id: $

CC = gcc
LIBS  = -ldl

all: snoopy.so detect

snoopy.so:  snoopy.c snoopy.h
			$(CC) -shared -O3 snoopy.c -osnoopy.so $(LIBS)
detect: detect.c
			$(CC) detect.c -odetect $(LIBS)
install: all 
			install -m 755 snoopy.so /lib/snoopy.so; \
			./install.sh
clean:
			rm detect snoopy.so
