# libcouchstore
# Copyright (C) 2010, 2011 Couchbase, Inc
# All rights reserved.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
AC_PREREQ(2.59)
m4_include([m4/version.m4])
AC_INIT(libcouchstore, VERSION_NUMBER, support@couchbase.com)
AC_CONFIG_SRCDIR([src/couch_db.cc])
AC_CONFIG_AUX_DIR(config)
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE(subdir-objects)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_DISABLE_STATIC
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LIBTOOL
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_HOST

# the malloc tests seems to be broken for cross compilation.. ignore them
ac_cv_func_malloc_0_nonnull=yes
ac_cv_func_realloc_0_nonnull=yes

AC_CHECK_FUNCS_ONCE([fdatasync])
if test $ac_cv_func_fdatasync = no; then
 AC_DEFINE([fdatasync], [fsync], [Define to fsync if you lack fdatasync])
fi

COUCHBASE_GENERIC_COMPILER(c89 cxx)

PANDORA_HAVE_LIBLUA

AC_CACHE_CHECK([for htonll], [ac_cv_have_htonll],
    [AC_TRY_LINK([
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
                 ], [
return htonll(0);
                 ],
                 [ ac_cv_have_htonll=yes ],
                 [ ac_cv_have_htonll=no ])
])
AS_IF([test "x$ac_cv_have_htonll" = "xyes"],[
       AC_DEFINE([HAVE_HTONLL], [1], [Have ntohll])])

AC_SEARCH_LIBS(floor, m)
AC_SEARCH_LIBS(pow, m)
AC_SEARCH_LIBS(fabs, m)

AC_ARG_WITH([docs],
  [AS_HELP_STRING([--with-docs],
    [Generate documentation (yes|no) @<:@default=yes@:>@])],
  [with_docs=$withval],
  [with_docs=yes])

AC_PATH_PROG(POD2MAN, pod2man, missing)
AS_IF([test "$with_docs" = "yes" -a "$POD2MAN" = "missing" ],
      [ AC_MSG_ERROR([pod2man is required for building docs])])
AM_CONDITIONAL(BUILD_DOCS, test "$with_docs" = "yes")

AS_CASE([$host_os],
  [mingw*], [
    IS_WINDOWS="TRUE"
  ])

AM_CONDITIONAL([WINDOWS], [test x$IS_WINDOWS = xTRUE])
AM_CONDITIONAL([GCC], [test x$GCC = xyes])

AC_CHECK_HEADERS_ONCE([snappy-c.h netinet/in.h inttypes.h])

dnl Check that we're able to find a usable libsnappy
AC_CACHE_CHECK([for libsnappy], [ac_cv_have_libsnappy],
  [ saved_libs="$LIBS"
    LIBS="-lsnappy"
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    AC_TRY_LINK([
      #include <snappy-c.h>
      #include <stddef.h>
            ],[
      size_t sizetest = snappy_max_compressed_length(10);
            ],[
      ac_cv_have_libsnappy="yes"
            ], [
      ac_cv_have_libsnappy="no"
      ])
    AC_LANG_RESTORE
    LIBS="$saved_libs"
  ])

AS_IF([test "x$ac_cv_header_snappy_c_h" != "xyes"],
      [AC_MSG_ERROR(Failed to locate snappy-c.h)])
AS_IF([test "x${ac_cv_have_libsnappy}" != "xyes"],
      [AC_MSG_ERROR(Failed to locate libsnappy)])

AC_ARG_WITH([win32-icu-binaries], [AC_HELP_STRING([--with-win32-icu-binaries=PATH],
    [set PATH to the Win32 native ICU binaries directory])], [
    ICU_CONFIG="" # supposed to be a command to query options...
    ICU_INCLUDE="$withval/include"
    ICU_LOCAL_CFLAGS="-I$ICU_INCLUDE"
    ICU_LIB_DIR="$withval/lib"
    ICU_LOCAL_LDFLAGS="-L$ICU_LIB_DIR"
    ICU_LOCAL_BIN=$withval/bin
    AC_SUBST(ICU_INCLUDE)
    AC_SUBST(ICU_LIB_DIR)
], [
    AC_CHECK_ICU([3.4.1])
    ICU_LOCAL_CFLAGS=`$ICU_CONFIG --cppflags-searchpath`
    ICU_LOCAL_LDFLAGS=`$ICU_CONFIG --ldflags-searchpath`
    ICU_LOCAL_BIN=
])

AC_SUBST(ICU_CONFIG)
AC_SUBST(ICU_LOCAL_CFLAGS)
AC_SUBST(ICU_LOCAL_LDFLAGS)
AC_SUBST(ICU_LOCAL_BIN)

AC_LANG_PUSH([C++])

AC_ARG_WITH([v8-lib], [AC_HELP_STRING([--with-v8-lib=PATH],
    [set PATH to the V8 library directory])],
    [
        V8_LIB_DIR="$withval"
        V8_LIB_FLAGS="-L$V8_LIB_DIR -lv8"
    ], [
        V8_LIB_FLAGS="-lv8"
    ])
AC_SUBST(V8_LIB_FLAGS)

AC_ARG_WITH([v8-include], [AC_HELP_STRING([--with-v8-include=PATH],
    [set PATH to the V8 include directory])], [
        V8_FLAGS="-I$withval"
    ], [
        V8_FLAGS="-I/usr/include/v8"
        V8_FLAGS="$V8_FLAGS -I/opt/local/include"
        V8_FLAGS="$V8_FLAGS -I/usr/local/include/v8"
        V8_FLAGS="$V8_FLAGS -I/opt/local/include/v8"
])
AC_SUBST(V8_FLAGS)
CPPFLAGS="$V8_FLAGS $CPPFLAGS"

SAVED_LDFLAGS=$LDFLAGS
if test "x$V8_LIB_DIR" != "x"; then
    LDFLAGS="-L$V8_LIB_DIR -Wl,-rpath,$V8_LIB_DIR"
fi

SAVED_LIBS="$LIBS"
LIBS="-lv8"
AC_CHECK_HEADER([v8.h], [
  AC_TRY_LINK([
    #include <v8.h>
  ],
  [v8::V8::GetVersion();],
  [], [AC_MSG_ERROR([Failed to link with libv8.])])

  AC_RUN_IFELSE([AC_LANG_SOURCE([[
    #include <v8.h>
    #include <stdio.h>
    int main() {
      int major, minor;
      sscanf(v8::V8::GetVersion(), "%d.%d", &major, &minor);
      if (major >= 3 && minor > 19) {
        return 0;
      }
      else {
        return 1;
      }
    }
  ]])],
  [
  AC_MSG_NOTICE([Found v8 version > 3.19.])
  V8_POST_3_19_FLAG="-DV8_POST_3_19_API"],
  [
  AC_MSG_NOTICE([Found v8 version <= 3.19.])
  ])
], [AC_MSG_ERROR([Could not find the V8 JavaScript engine library.])])

LIBS="$SAVED_LIBS"
LDFLAGS="$V8_LIB_FLAGS $SAVED_LDFLAGS"
CPPFLAGS="$CPPFLAGS $V8_POST_3_19_FLAG"

AC_LANG_POP([C++])

dnl Check whether the user's system supports pthread
AC_SEARCH_LIBS(pthread_create, pthread)
if test "x$ac_cv_search_pthread_create" = "xno"; then
  AC_MSG_ERROR([Can't enable threads without the POSIX thread library.])
fi

AH_TOP([
#ifndef CONFIG_H
#define CONFIG_H
/*
 * This file is generated by running configure. Any changes you make to this
 * file will be overwritten the next time you run configure. If you want to
 * make permanent changes to the file you should edit configure.ac instead.
 * All platform-specific includes should be placed inside config_static.h
 * to keep the config.h as small as possible. That allows us for easily
 * use another build systems with a poor support for automake (like Windows)
 */
])

AH_BOTTOM([
#include "config_static.h"
#endif
])

#
# To ensure that we don't break applications in mysterious ways we need
# to version our library. For a detailed description, check out:
# http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
#
LIBCOUCHSTORE_API_CURRENT=1
LIBCOUCHSTORE_API_REVISION=0
LIBCOUCHSTORE_API_AGE=0
AC_SUBST(LIBCOUCHSTORE_API_CURRENT)
AC_SUBST(LIBCOUCHSTORE_API_REVISION)
AC_SUBST(LIBCOUCHSTORE_API_AGE)
dnl ----------------------------------------------------------------------------

AC_CONFIG_FILES(Makefile libcouchstore.pc)
AC_OUTPUT
AS_IF(test "$with_docs" = "no",
  [
    echo "*****"
    echo "*"
    echo "*  WARNING: You are not generating any documentation."
    echo "*           Please don't ship libcouchstore to an end user"
    echo "*           without documentation..."
    echo "*"
    echo "*****"
  ])
