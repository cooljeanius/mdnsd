## Automake Makefile for mdnsd

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = tests

CC = @CC@
RM_PROG = @RM_PROG@
AM_CFLAGS = -g -O1 -Wall -Wextra -Wimplicit -Wformat=2 -Wswitch \
	-Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
	-Wuninitialized -Winit-self -Wparentheses -Wwrite-strings \
	-fstack-protector -Wstack-protector \
	-fstrict-aliasing -Wstrict-aliasing \
	-std=gnu99 \
	-Wno-unused -Wno-unused-parameter \
	-Wno-pointer-sign -Wno-sign-compare
EXEEXT = @EXEEXT@

CLANG_ANALYZER = @CLANG_ANALYZER@
RUN_CLANG_ANALYZER = $(CLANG_ANALYZER) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) \
	--analyze

analysisdir = $(builddir)/clang_static_analysis

all: mquery$(EXEEXT) mhttp$(EXEEXT)

bin_PROGRAMS = mhttp mquery
mhttp_SOURCES = mhttp_main.c
mhttp_LDADD = libmdnsd.la
mquery_SOURCES = mquery_main.c
mquery_LDADD = libmdnsd.la

lib_LTLIBRARIES = libmdnsd.la
libmdnsd_la_SOURCES = 1035.c mdnsd.c mhttp.c mquery.c msock.c sdtxt.c xht.c
include_HEADERS = 1035.h mdnsd.h mhttp.h mquery.h msock.h sdtxt.h xht.h

ALL_PLISTS = 1035.plist mdnsd.plist mhttp_main.plist mhttp.plist \
	mquery_main.plist mquery.plist msock.plist sdtxt.plist xht.plist

SUFFIXES = .c .o .plist

.c.plist:
	@if test ! -d $(analysisdir); then $(MKDIR_P) $(analysisdir); fi
	$(RUN_CLANG_ANALYZER) -o $(analysisdir)/$@ $< 

MOSTLYCLEANFILES = config.h.in~

clean-local:
	$(RM_PROG) -f mquery$(EXEEXT) mhttp$(EXEEXT)
	$(RM_PROG) -f *.o *~
	$(RM_PROG) -rf *.dSYM *.cache

CLEANFILES = *~ core

distclean-local:
	$(RM_PROG) -rf autom4te.cache || rmdir autom4te.cache

DISTCLEANFILES = .DS_Store $(analysisdir)/.DS_Store autoscan.log

maintainer-clean-local:
	for plist in $(ALL_PLISTS); do \
	  test -e $(analysisdir)/$${plist} && $(RM_PROG) -f $(analysisdir)/$${plist}; \
	done
	$(RM_PROG) -rf $(analysisdir) || rmdir $(analysisdir)
	$(RM_PROG) -f $(DIST_ARCHIVES)

MAINTAINERCLEANFILES = configure.scan

nodist_noinst_DATA = @MAINT@ configure.scan

configure.scan:
	autoscan || touch $@

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

dist-hook: clean @MAINT@ update-ChangeLog
	ls

## using dependencies for the plists could go wrong, so call them as part
## of the rule instead:
check-local: all
	$(MKDIR_P) $(analysisdir)
	$(MAKE) $(ALL_PLISTS)
