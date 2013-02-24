#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mdnsd], [0.7G], [jer@jabber.org])
AC_CONFIG_SRCDIR([1035.c])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AC_ISC_POSIX
AM_SET_LEADING_DOT
AC_EXEEXT

# Checks for programs.
AC_PROG_CC
AC_PATH_PROG([RM],[rm])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h fcntl.h netinet/in.h signal.h stdio.h string.h strings.h sys/socket.h sys/time.h sys/types.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_TYPES([socklen_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atoi bzero close exit free gettimeofday inet_addr inet_ntoa memcpy pipe printf read select socket sprintf strchr strdup strerror strlen write])

AC_CONFIG_FILES([Makefile Makefile.orig])
AC_OUTPUT
