# Automake Makefile for mdnsd

ACLOCAL_AMFLAGS = -I m4 --install

CC = @CC@
RM_PROG = @RM_PROG@
AM_CFLAGS = -g -Wall -Wimplicit -Wno-unused -Wno-pointer-sign
EXEEXT = @EXEEXT@

all: mquery$(EXEEXT) mhttp$(EXEEXT)

bin_PROGRAMS = mhttp mquery
mhttp_SOURCES = mhttp.c msock.c mdnsd.c 1035.c sdtxt.c xht.c
mhttp_CFLAGS = $(AM_CFLAGS)
mquery_SOURCES = mquery.c msock.c mdnsd.c 1035.c
mquery_CFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libmdnsd.la
libmdnsd_la_SOURCES = 1035.c mdnsd.c mhttp.c mquery.c msock.c sdtxt.c xht.c
include_HEADERS = 1035.h mdnsd.h msock.h sdtxt.h xht.h

clean-local:
	$(RM_PROG) -f mquery$(EXEEXT) mhttp$(EXEEXT)
	$(RM_PROG) -f *.o *~
	$(RM_PROG) -rf *.dSYM *.cache

CLEANFILES = *~ core

DISTCLEANFILES = autoscan.log
