CC = @CC@
RM_PROG = @RM_PROG@
AM_CFLAGS = -g -Wall -Wimplicit
EXEEXT = @EXEEXT@

all: mquery$(EXEEXT) mhttp$(EXEEXT)

bin_PROGRAMS = mhttp mquery
mhttp_SOURCES = mhttp.c mdnsd.c 1035.c sdtxt.c xht.c
mquery_SOURCES = mquery.c mdnsd.c 1035.c

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
