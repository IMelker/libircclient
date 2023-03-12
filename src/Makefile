# $Id: Makefile.in 137 2018-08-27 01:11:08Z gyunaev $
CC = gcc
CFLAGS = -Wall -Wno-misleading-indentation -Wno-unused-function -Wno-nonnull -DIN_BUILDING_LIBIRC -g -O2 -O3 -DENABLE_THREADS -D_REENTRANT -DENABLE_TWITCH
AR=ar cr
RANLIB=ranlib
INCLUDES=-I../include
DESTDIR=
APIVERSION = 1
STATIC=true

OBJS = libircclient.o

all:	static

static:	libircclient.a
shared: libircclient.so
shared_mingw: libircclient.dll

install:
	-mkdir -p $(DESTDIR)/usr/local/include
	-mkdir -p $(DESTDIR)${exec_prefix}/lib
	cp ../include/libircclient.h $(DESTDIR)/usr/local/include/libircclient.h
	cp ../include/libirc_errors.h $(DESTDIR)/usr/local/include/libirc_errors.h
	cp ../include/libirc_events.h  $(DESTDIR)/usr/local/include/libirc_events.h 
	cp ../include/libirc_options.h  $(DESTDIR)/usr/local/include/libirc_options.h 
	cp ../include/libirc_rfcnumeric.h  $(DESTDIR)/usr/local/include/libirc_rfcnumeric.h
ifneq (,$(wildcard ./libircclient.a))
	cp libircclient.a $(DESTDIR)${exec_prefix}/lib/libircclient.a.$(APIVERSION)
	ln -fs libircclient.a.$(APIVERSION) $(DESTDIR)${exec_prefix}/lib/libircclient.a
endif
ifneq (,$(wildcard ./libircclient.so))
	cp libircclient.so $(DESTDIR)${exec_prefix}/lib/libircclient.so.$(APIVERSION)
	ln -fs libircclient.so.$(APIVERSION) $(DESTDIR)${exec_prefix}/lib/libircclient.so
endif
ifneq (,$(wildcard ./libircclient.dll))
	cp libircclient.dll $(DESTDIR)${exec_prefix}/lib/libircclient.dll
endif

libircclient.so: libircclient.o
	$(CC) -shared -s -Wl,-soname,libircclient.so.$(APIVERSION) -o libircclient.so libircclient.o   -lnsl

libircclient.dll: libircclient.o
	$(CC) -shared -s -o libircclient.dll -Wl,-out-implib,libircclient.lib libircclient.o libircclient.def -lkernel32 -lwsock32  -lnsl

libircclient.a: libircclient.o
	$(AR) libircclient.a libircclient.o
	$(RANLIB) libircclient.a

clean:
	rm -f libircclient.a libircclient.o

distclean: clean
	-rm -f Makefile

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
