# Originally taken from the autotest documentation.
# http://www.gnu.org/software/autoconf/manual/autoconf-2.69/html_node/Making-testsuite-Scripts.html#Making-testsuite-Scripts

# The `:;' works around a Bash 3.2 bug when the output is not writable.
$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	:;{ \
	  echo '# Signature of the current package.' && \
	  echo 'm4_define([AT_PACKAGE_NAME],' && \
	  echo '  [$(PACKAGE_NAME)])' && \
	  echo 'm4_define([AT_PACKAGE_TARNAME],' && \
	  echo '  [$(PACKAGE_TARNAME)])' && \
	  echo 'm4_define([AT_PACKAGE_VERSION],' && \
	  echo '  [$(PACKAGE_VERSION)])' && \
	  echo 'm4_define([AT_PACKAGE_STRING],' && \
	  echo '  [$(PACKAGE_STRING)])' && \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT],' && \
	  echo '  [$(PACKAGE_BUGREPORT)])'; \
	  echo 'm4_define([AT_PACKAGE_URL],' && \
	  echo '  [$(PACKAGE_URL)])'; \
	} >'$(srcdir)/package.m4'

EXTRA_DIST = testsuite.at $(srcdir)/package.m4 $(TESTSUITE) atlocal.in
TESTSUITE = $(srcdir)/testsuite

AUTOMAKE_OPTIONS = no-dependencies subdir-objects -Wno-override

check_PROGRAMS = $(top_builddir)/mhttp $(top_builddir)/mquery

__top_builddir__mhttp_SOURCES = $(top_builddir)/mhttp_main.c
__top_builddir__mhttp_LDADD = $(top_builddir)/libmdnsd.la
__top_builddir__mquery_SOURCES = $(top_builddir)/mquery_main.c
__top_builddir__mquery_LDADD = $(top_builddir)/libmdnsd.la

$(top_builddir)/mhttp$(EXEEXT): $(top_builddir)/Makefile
	$(MAKE) -C $(top_builddir) mhttp$(EXEEXT)

$(top_builddir)/mquery$(EXEEXT): $(top_builddir)/Makefile
	$(MAKE) -C $(top_builddir) mquery$(EXEEXT)

check-local: atconfig atlocal $(TESTSUITE) $(check_PROGRAMS)
	export top_builddir=$(top_builddir) && \
	  $(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)

##FIXME: fix this for VPATH builds:
installcheck-local: atconfig atlocal $(TESTSUITE) \
  $(top_builddir)/mhttp$(EXEEXT) $(top_builddir)/mquery$(EXEEXT)
	if test -d $(top_builddir); then \
	  abs_top_builddir=`cd $(top_builddir) && pwd`; \
	  echo "export top_builddir=$${abs_top_builddir}"; \
	  export top_builddir=$${abs_top_builddir}; \
	  $(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(bindir)' \
	    $(TESTSUITEFLAGS) $(AM_TESTSUITEFLAGS) \
	    top_builddir=$${abs_top_builddir}; \
	else \
	  echo "warning: '$(top_builddir)' does not exist!" >&2; \
	  exit 1; \
	fi

clean-local:
	test ! -f '$(TESTSUITE)' || \
	  export top_builddir=$(top_builddir) && $(SHELL) '$(TESTSUITE)' --clean

DISTCLEANFILES = .DS_Store

AUTOM4TE = $(SHELL) $(top_srcdir)/build-aux/missing --run autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest
$(TESTSUITE): $(srcdir)/testsuite.at $(srcdir)/package.m4
	export top_builddir=$(top_builddir) && \
	  $(AUTOTEST) -I '$(srcdir)' -o $@.tmp $@.at
	mv $@.tmp $@
