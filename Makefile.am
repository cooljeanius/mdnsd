# Automake Makefile for mdnsd

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = tests

CC = @CC@
RM_PROG = @RM_PROG@
AM_CFLAGS = -g -Wall -Wimplicit -Wno-unused -Wno-pointer-sign
EXEEXT = @EXEEXT@

all: mquery$(EXEEXT) mhttp$(EXEEXT)

bin_PROGRAMS = mhttp mquery
mhttp_SOURCES = mhttp_main.c
mhttp_LDADD = libmdnsd.la
mquery_SOURCES = mquery_main.c
mquery_LDADD = libmdnsd.la

lib_LTLIBRARIES = libmdnsd.la
libmdnsd_la_SOURCES = 1035.c mdnsd.c mhttp.c mquery.c msock.c sdtxt.c xht.c
include_HEADERS = 1035.h mdnsd.h mhttp.h mquery.h msock.h sdtxt.h xht.h

clean-local:
	$(RM_PROG) -f mquery$(EXEEXT) mhttp$(EXEEXT)
	$(RM_PROG) -f *.o *~
	$(RM_PROG) -rf *.dSYM *.cache

CLEANFILES = *~ core

DISTCLEANFILES = autoscan.log

PHONY: update-ChangeLog
update-ChangeLog:
	    if test -d $(srcdir)/.git; then                         \
	       $(srcdir)/build-aux/gitlog-to-changelog              \
	          --format='%s%n%n%b%n' --no-cluster                \
	          --strip-tab --strip-cherry-pick                   \
	          -- $$(cat $(srcdir)/.last-cl-gen)..               \
	        >ChangeLog.tmp                                      \
	      && git rev-list -n 1 HEAD >.last-cl-gen.tmp           \
	      && (echo; cat $(srcdir)/ChangeLog) >>ChangeLog.tmp    \
	      && mv -f ChangeLog.tmp $(srcdir)/ChangeLog            \
	      && mv -f .last-cl-gen.tmp $(srcdir)/.last-cl-gen      \
	      && rm -f ChangeLog.tmp;                               \
	    fi

EXTRA_DIST = .last-cl-gen
