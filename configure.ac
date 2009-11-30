#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)
AC_INIT(tuscany-sca, 1.0, dev@tuscany.apache.org)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/tuscany/sca)

# Checks for programs.
AC_MSG_NOTICE([checking for programs])
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Check for running on Darwin
AC_MSG_CHECKING([if running on Darwin])
UNAME=`uname -s`
if test "x$UNAME" = "xDarwin"; then
  AC_DEFINE([IS_DARWIN], [1], [Set to 1 when running on Darwin - Mac OSX])
  AC_MSG_RESULT(yes)
  AC_SUBST([libsuffix],[".dylib"])
  is_darwin=true
else
  AC_MSG_RESULT(no)
  AC_SUBST([libsuffix],[".so"])
  is_darwin=false
fi
AM_CONDITIONAL([DARWIN], [test x$is_darmin = xtrue])

# Configure CURL_INCLUDE and CURL_LIB
AC_MSG_CHECKING([for curl])
AC_ARG_WITH([curl], [AC_HELP_STRING([--with-curl=PATH], [path to installed curl [default=/usr]])], [
  CURL_INCLUDE="$withval/include"
  CURL_LIB="$withval/lib"
  AC_MSG_RESULT("$withval")
], [
  CURL_INCLUDE="/usr/include"
  CURL_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(CURL_INCLUDE)
AC_SUBST(CURL_LIB)

# Configure LIBXML2_INCLUDE and LIBXML2_LIB
AC_MSG_CHECKING([for libxml2])
AC_ARG_WITH([libxml2], [AC_HELP_STRING([--with-libxml2=PATH], [path to installed libxml2 [default=/usr]])], [
  LIBXML2_INCLUDE="$withval/include/libxml2"
  LIBXML2_LIB="$withval/lib"
  AC_MSG_RESULT("$withval")
], [
  LIBXML2_INCLUDE="/usr/include/libxml2"
  LIBXML2_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(LIBXML2_INCLUDE)
AC_SUBST(LIBXML2_LIB)

# Configure LIBMOZJS_INCLUDE and LIBMOZJS_LIB
AC_MSG_CHECKING([for js-include])
AC_ARG_WITH([js-include], [AC_HELP_STRING([--with-js-include=PATH], [path to installed SpiderMonkey include dir
             [default=/usr/include/xulrunner-1.9.1.5/unstable]])], [
  JS_INCLUDE="$withval"
  AC_MSG_RESULT("$withval")
], [
  JS_INCLUDE="/usr/include/xulrunner-1.9.1.5/unstable"
  AC_MSG_RESULT(/usr/include/xulrunner-1.9.1.5/unstable)
])
AC_MSG_CHECKING([for js-lib])
AC_ARG_WITH([js-lib], [AC_HELP_STRING([--with-js-lib=PATH], [path to installed SpiderMonkey lib dir [default=/usr/lib/xulrunner-1.9.1.5]])], [
  JS_LIB="$withval"
  AC_MSG_RESULT("$withval")
], [
  JS_LIB="/usr/lib/xulrunner-1.9.1.5"
  AC_MSG_RESULT(/usr/lib/xulrunner-1.9.1.5)
])
AC_SUBST(JS_INCLUDE)
AC_SUBST(JS_LIB)

# Configure APR and HTTPD
AC_MSG_CHECKING([for apr])
AC_ARG_WITH([apr], [AC_HELP_STRING([--with-apr=PATH], [path to installed apr [default=/usr/local/apache2]])], [
  APR_INCLUDE="$withval/include"
  APR_LIB="$withval/lib"
  AC_MSG_RESULT("$withval")
], [
  APR_INCLUDE="/usr/local/apache2/include"
  APR_LIB="/usr/local/apache2/lib"
  AC_MSG_RESULT(/usr/local/apache2/lib)
])
AC_SUBST(APR_INCLUDE)
AC_SUBST(APR_LIB)

AC_MSG_CHECKING([for httpd])
AC_ARG_WITH([httpd], [AC_HELP_STRING([--with-httpd=PATH], [path to installed httpd [default=/usr/local/apache2]])], [
  HTTPD_INCLUDE="$withval/include"
  AC_MSG_RESULT("$withval")
], [
  HTTPD_INCLUDE="/usr/local/apache2/include"
  AC_MSG_RESULT(/usr/local/apache2/lib)
])
AC_SUBST(HTTPD_INCLUDE)

# Checks for libraries.
original_LIBS="$LIBS"
AC_MSG_NOTICE([checking for libraries])
LIBS="-L$CURL_LIB $original_LIBS"
AC_CHECK_LIB([curl], [curl_global_init], [], [AC_MSG_ERROR([couldn't find a suitable libcurl, use --with-curl=PATH])])
LIBS="-L$JS_LIB $original_LIBS"
AC_CHECK_LIB([mozjs], [JS_NewContext], [], [AC_MSG_ERROR([couldn't find a suitable libmozjs, use --with-js-lib=PATH])])
LIBS="$original_LIBS"
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([couldn't find a suitable libpthread])])
LIBS="-L$LIBXML2_LIB $original_LIBS"
AC_CHECK_LIB([xml2], [xmlInitParser], [], [AC_MSG_ERROR([couldn't find a suitable libxml2, use --with-libxml2=PATH])])
LIBS="-L$APR_LIB $original_LIBS"
AC_CHECK_LIB([apr-1], [apr_pool_initialize], [], [AC_MSG_ERROR([couldn't find a suitable libapr-1, use --with-apr=PATH])])
AC_CHECK_LIB([aprutil-1], [apr_memcache_replace], [], [AC_MSG_ERROR([couldn't find a suitable libaprutil-1, use --with-apr=PATH])])
LIBS="$original_LIBS"

# Checks for header files.
AC_MSG_NOTICE([checking for header files])
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_NOTICE([checking for typedefs, structures, and compiler characteristics])
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_MSG_NOTICE([checking for library functions])
AC_CHECK_FUNCS([gettimeofday select])

# Configure GCC C++ compile options
# Configure debugging, profiling and compile-time warnings
AC_MSG_CHECKING([whether to compile for debugging and profiling])
AC_ARG_ENABLE(maintainer-mode, [AS_HELP_STRING([--enable-maintainer-mode], [compile for debugging and profiling [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_maintainer_mode=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "$want_maintainer_mode" = "true"; then
  AC_SUBST([CXXFLAGS], ["-D_DEBUG -O0 -g3 -pg -Wall -Wextra -Wno-ignored-qualifiers -Winit-self -Wmissing-include-dirs -Wcast-qual -Wcast-align -Wwrite-strings -Wpointer-arith -Wconversion -Waddress -Wlogical-op -Wredundant-decls -std=c++0x -fmessage-length=0"])
  AC_SUBST([LDFLAGS], ["${LDFLAGS} -pg"])
else
  AC_SUBST([CXXFLAGS], ["-O3 -std=c++0x -fmessage-length=0"])
fi

# Configure exuberant ctags
TAGSFILE="`pwd`/tags"
AC_SUBST([CTAGSFLAGS], ["${CTAGSFLAGS} --c++-kinds=+p --fields=+iaS --extra=+q --append --tag-relative=yes -f $TAGSFILE"])

# Configure TUSCANY_SCACPP
TUSCANY_SCACPP=`echo "$TUSCANY_SCACPP"`
if test "$TUSCANY_SCACPP" = ""; then
  SCAPWD=`pwd`
  AC_SUBST([TUSCANY_SCACPP], ["$SCAPWD"])
fi

# Configure Doxygen build
AC_MSG_CHECKING([whether to build Doxygen documentation])
AC_ARG_ENABLE(doxygen, [AS_HELP_STRING([--enable-doxygen], [build Doxygen documentation [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    AC_PATH_PROG(DOXYGEN, doxygen, , $PATH)
    if test "$DOXYGEN" = ""; then
      AC_MSG_ERROR([could not find doxygen])
    fi
    want_doxygen=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
AM_CONDITIONAL([WANT_DOXYGEN], [test "$want_doxygen" = "true"])

AC_CONFIG_FILES([Makefile
                 kernel/Makefile
                 modules/Makefile
                 modules/atom/Makefile
                 modules/eval/Makefile
                 modules/http/Makefile
                 modules/json/Makefile
                 modules/scdl/Makefile
                 modules/server/Makefile
                 components/Makefile
                 components/cache/Makefile
                 components/store/Makefile
                 samples/Makefile
                 test/Makefile
                 test/store-object/Makefile
                 test/store-function/Makefile
                 test/store-script/Makefile
                 doc/Makefile
                 doc/Doxyfile
                 ])
AC_OUTPUT

