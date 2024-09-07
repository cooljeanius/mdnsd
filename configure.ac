dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])dnl
AC_INIT([mdnsd],[0.8.1],[jer@jabber.org])dnl
AC_CONFIG_SRCDIR([1035.c])dnl
AC_CONFIG_HEADERS([config.h])dnl
AC_CONFIG_AUX_DIR([build-aux])dnl
AC_REQUIRE_AUX_FILE([gitlog-to-changelog])dnl
AC_CONFIG_MACRO_DIR([m4])dnl
AC_CONFIG_TESTDIR([tests])dnl
AC_CANONICAL_TARGET
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE([dist-bzip2 dist-zip gnits])dnl
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
  AC_SUBST([CFLAGS])dnl
])dnl
AC_REQUIRE_COMPILER([])dnl
AC_ARG_VAR([CLANG_ANALYZER],[Path to the clang static analyzer])dnl
AC_CACHE_CHECK([for the clang static analyzer],[ac_cv_path_CLANG_ANALYZER],
  [AC_PATH_PROGS_FEATURE_CHECK([CLANG_ANALYZER],
    [clang clang++ clang-mp-14 clang-mp-12 clang-mp-11 clang-mp-10 clang-mp-9.0],
    [[${ac_path_CLANG_ANALYZER} --analyze /dev/null > /dev/null 2>&1 && \
      ac_cv_path_CLANG_ANALYZER=${ac_path_CLANG_ANALYZER}
      ac_path_CLANG_ANALYZER_found=:]],
    [AC_MSG_WARN([we will not be able to do static analysis with clang])],
    [${PATH}])dnl# end program check
  ])dnl# end cache check
  ## (need this extra line here)
AC_SUBST([CLANG_ANALYZER],[${ac_cv_path_CLANG_ANALYZER}])dnl
AM_PROG_AR
m4_ifdef([AC_PROG_AR],[
AC_PROG_AR
])dnl
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
  AC_REQUIRE([AC_CHECK_INCLUDES_DEFAULT])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS_ONCE([sys/time.h])
# Obsolete code to be removed.
if test "x${ac_cv_header_sys_time_h}" = "xyes"; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],
            [Define to 1 if you can safely include both <sys/time.h>
             and <time.h>.  This macro is obsolete.])
fi
# End of obsolete code.

AC_CHECK_HEADERS([arpa/inet.h errno.h fcntl.h netinet/in.h \
                  malloc.h malloc/malloc.h signal.h time.h xlocale.h])dnl

if test "x${ac_cv_header_inttypes_h}" = "x"; then
  test -z "${ac_cv_header_inttypes_h}" || unset ac_cv_header_inttypes_h
  AC_CHECK_HEADERS([inttypes.h])
fi
if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}" || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_stdio_h}" = "x"; then
  test -z "${ac_cv_header_stdio_h}" || unset ac_cv_header_stdio_h
  AC_CHECK_HEADERS([stdio.h])
fi
if test "x${ac_cv_header_strings_h}" = "x"; then
  test -z "${ac_cv_header_strings_h}" || unset ac_cv_header_strings_h
  AC_CHECK_HEADERS([strings.h])
fi
if test "x${ac_cv_header_sys_socket_h}" = "x"; then
  test -z "${ac_cv_header_sys_socket_h}" || unset ac_cv_header_sys_socket_h
  AC_CHECK_HEADERS([sys/socket.h])
fi
if test "x${ac_cv_header_sys_time_h}" = "x"; then
  test -z "${ac_cv_header_sys_time_h}" || unset ac_cv_header_sys_time_h
  AC_CHECK_HEADERS([sys/time.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_C_CONST
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_CHECK_TYPES([in_addr_t])dnl
dnl# we provide a replacement for this macro in the m4/ directory;
dnl# make sure to prevent autoupdate from replacing it:
AC_TYPE_SIGNAL
dnl# back to normal...
AC_TYPE_SIZE_T
AC_CHECK_TYPES([socklen_t],[],[
  AC_MSG_NOTICE([conftest for socklen_t may have been missing some includes; trying again])
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
