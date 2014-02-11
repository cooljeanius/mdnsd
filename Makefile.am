CC = @CC@
RM_PROG = @RM_PROG@
AM_CFLAGS = -g -Wall -Wimplicit -Wno-unused -Wno-pointer-sign
EXEEXT = @EXEEXT@

all: mquery$(EXEEXT) mhttp$(EXEEXT)

bin_PROGRAMS = mhttp mquery
mhttp_SOURCES = mhttp.c mdnsd.c 1035.c sdtxt.c xht.c
mquery_SOURCES = mquery.c mdnsd.c 1035.c
noinst_LIBRARIES = libmdnsdcommon.a
libmdnsdcommon_a_SOURCES = mdnsd.c 1035.c
lib_LIBRARIES = libmdnsd.a
libmdnsd_a_SOURCES = 1035.c mdnsd.c mhttp.c mquery.c sdtxt.c xht.c
include_HEADERS = 1035.h mdnsd.h sdtxt.h xht.h

my_mhttp$(EXEEXT): mhttp.c mdnsd.c 1035.c sdtxt.c xht.c
	$(CC) $(CFLAGS) $(AM_CFLAGS) -o mhttp$(EXEEXT) mhttp.c mdnsd.c 1035.c sdtxt.c xht.c
.PHONY: my_mhttp$(EXEEXT)

my_mquery$(EXEEXT): mquery.c mdnsd.c 1035.c
	$(CC) $(CFLAGS) $(AM_CFLAGS) -o mquery$(EXEEXT) mquery.c mdnsd.c 1035.c
.PHONY: my_mquery$(EXEEXT)

clean-local:
	$(RM_PROG) -f mquery$(EXEEXT) mhttp$(EXEEXT)
	$(RM_PROG) -f *.o *~
	$(RM_PROG) -rf *.dSYM *.cache

CLEANFILES = *~ core

DISTCLEANFILES = autoscan.log
