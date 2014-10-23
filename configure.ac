dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])dnl
AC_INIT([mdnsd],[0.8.0],[jer@jabber.org])dnl
AC_CONFIG_SRCDIR([1035.c])dnl
AC_CONFIG_HEADERS([config.h])dnl
AC_CONFIG_AUX_DIR([build-aux])dnl
AC_REQUIRE_AUX_FILE([gitlog-to-changelog])dnl
AC_CONFIG_MACRO_DIR([m4])dnl
AC_CONFIG_TESTDIR([tests])dnl
AC_CANONICAL_TARGET
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE([gnits])dnl
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AC_SEARCH_LIBS([strerror],[cposix]) dnl# Used to be AC\_ISC\_POSIX
AM_SET_LEADING_DOT
AC_EXEEXT
AC_OBJEXT
AC_SYS_LARGEFILE

# Checks for programs.
AC_DEFUN([AC_REQUIRE_COMPILER],[
  AC_REQUIRE([AC_PROG_CC])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_PROG_CXX])dnl
  AC_REQUIRE([AC_PROG_GCC_TRADITIONAL])dnl
  AC_SUBST([CFLAGS])dnl
])dnl
AC_REQUIRE_COMPILER
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AC_PATH_PROG([RM_PROG],[rm])dnl
AC_SUBST([RM_PROG])dnl

# all right, let us try using libtool:
LT_INIT([win32-dll])dnl
LT_LANG([C])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h errno.h fcntl.h netinet/in.h \
                  malloc.h malloc/malloc.h signal.h stdio.h \
                  sys/socket.h sys/time.h time.h xlocale.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_C_CONST
AC_C_PROTOTYPES
AC_CHECK_TYPES([in_addr_t])dnl
AC_TYPE_SIGNAL
AC_CHECK_TYPES([socklen_t],[],[
  unset ac_cv_type_socklen_t
  AC_CHECK_TYPES([socklen_t],[],[],[
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for socklen_t wants to include <sys/types.h>."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for socklen_t wants to include <sys/socket.h>."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
  ])dnl
])dnl

# Checks for library functions.
AC_FUNC_MEMCMP
AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([addr2ascii atoi bind bzero close exit errno free \
                gettimeofday htonl htons inet_addr inet_lnaof \
                inet_makeaddr inet_ntoa memcpy pipe printf read select \
                sendto setsockopt signal socket sprintf strchr strcmp \
                strdup strerror strlen time write])dnl

# Output.
AC_CONFIG_FILES([Makefile \
                 Makefile_orig \
                 tests/atlocal \
                 tests/Makefile])dnl
AC_OUTPUT
