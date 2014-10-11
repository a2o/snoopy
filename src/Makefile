# Makefile for snoopy



CC=gcc
CFLAGS=-g -O2 -DHAVE_CONFIG_H
LIBS=-ldl
LDFLAGS= -ldl 
prefix=/usr/local
exec_prefix=${prefix}
LIBDIR=${exec_prefix}/lib



### Common CC arguments
#
#CC_ARGS=-O3 -fomit-frame-pointer -fPIC -I. $(CFLAGS) $(LDFLAGS)
#CC_ARGS=-O3 -fomit-frame-pointer -fPIC -Wall -Werror
CC_ARGS=-O3 -fomit-frame-pointer -fPIC -Wall


### Common dependencies for all targets
#
DEPS_HEADERS=snoopy.h ../config.h
DEPS=$(DEPS_HEADERS)



### Final build target
#
.PHONY: all
#all: snoopy.so detect snoopy-test snoopy-dev-helper
all: \
  snoopy.so \
  bin/snoopy-test-output



### Define targets for all snoopy inputs
#
SNOOPY_INPUTS_INDIVIDUAL     = $(shell ls input/*.c | sed -e "s/\.c$$//")
SNOOPY_INPUTS_INDIVIDUAL_SRC = $(addsuffix .c,${SNOOPY_INPUTS_INDIVIDUAL})
SNOOPY_INPUTS_INDIVIDUAL_OBJ = $(addsuffix .o,${SNOOPY_INPUTS_INDIVIDUAL})

inputs_data.o: $(DEPS) inputs_data.c inputs_data.h
	$(CC) $(CC_ARGS) -c inputs_data.c

input/%.o: $(DEPS) input/%.c input/%.h inputs_data.o
	$(eval SRC_FILENAME := $(shell echo "$@" | sed -e "s/\.o$$/.c/"))
	$(CC) $(CC_ARGS) -I. -c $(SRC_FILENAME) -o $@

inputs.o: $(DEPS) inputs.c inputs.h inputs_data.o ${SNOOPY_INPUTS_INDIVIDUAL_OBJ}
	$(CC) $(CC_ARGS) -c inputs.c



### Common snoopy logging routines
#
log.o: $(DEPS) log.c log.h inputs.o inputs_data.o
	$(eval SRC_FILENAME := $(shell echo "$@" | sed -e "s/\.o$$/.c/"))
	$(CC) $(CC_ARGS) -I.. -c $(SRC_FILENAME)



### Snoopy tester
#
bin/snoopy-test-output.o: $(DEPS) bin/snoopy-test-output.c log.o
	$(eval SRC_FILENAME := $(shell echo "$@" | sed -e "s/\.o$$/.c/"))
	$(CC) $(CC_ARGS) -I. -c $(SRC_FILENAME) -o $@
bin/snoopy-test-output: bin/snoopy-test-output.o
	$(CC) $(CC_ARGS) -o $@ $@.o log.o inputs.o inputs_data.o $(SNOOPY_INPUTS_INDIVIDUAL_OBJ)



### Development helper tool, for testing output
#
bin/snoopy-dev-helper.o: bin/snoopy-dev-helper.c inputs.o
	$(eval SRC_FILENAME := $(shell echo "$@" | sed -e "s/\.o$$/.c/"))
	$(CC) $(CC_ARGS) -I. -c $(SRC_FILENAME)
bin/snoopy-dev-helper: bin/snoopy-dev-helper.o
	$(CC) $(CC_ARGS) -o $@ $@.o log.o inputs.o inputs_data.o $(SNOOPY_INPUTS_INDIVIDUAL_OBJ)



### Main library object file and final library
#
snoopy.o: $(DEPS) snoopy.c snoopy.h log.o
	$(CC) $(CC_ARGS) -c snoopy.c

snoopy.so: $(DEPS) snoopy.o
	$(CC) $(CC_ARGS) -shared -ldl -o snoopy.so snoopy.o log.o inputs.o inputs_data.o $(SNOOPY_INPUTS_INDIVIDUAL_OBJ)



### Why is this here actually? To test whether snoopy is loaded?
#
detect: detect.c
	$(CC) $(CC_ARGS) $@.c -o $@



### Installation targets
#
install: all
	install -m 755 -d $(DESTDIR)$(LIBDIR)
	install -m 755 snoopy.so $(DESTDIR)$(LIBDIR)/snoopy.so
	@echo -e "\nSnoopy shared library installed in $(DESTDIR)$(LIBDIR)."
	@echo -e "Run 'make enable' to actually enable snoopy logging.\n"



clean:
	rm -f *.o */*.o
	rm -f detect snoopy.so
	rm -f contrib/sles/snoopy.spec
	rm -f bin/snoopy-test-output
	rm -f bin/snoopy-dev-helper

distclean: clean
	rm -f autoscan.log config.h config.log config.status
	rm -rf autom4te.cache
#	rm -f Makefile

realclean: distclean
	rm -f configure configure.scan
