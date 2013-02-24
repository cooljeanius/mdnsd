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
AC_CHECK_HEADERS([errno.h fcntl.h netinet/in.h signal.h string.h sys/socket.h sys/time.h sys/types.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero exit gettimeofday inet_ntoa memcpy printf select socket sprintf strchr strdup strerror strlen])

AC_CONFIG_FILES([Makefile Makefile.orig])
AC_OUTPUT
