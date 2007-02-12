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
AC_INIT(tuscany_sca_cpp, 1.0-incubator-M2)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(tuscany_sca_config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_PREFIX_DEFAULT(/usr/local/tuscany/sca)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Check for running on Darwin
AC_MSG_CHECKING([Checking if running on Darwin])
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

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd putenv strdup])
# AC_CONFIG_SUBDIRS([samples])

AC_MSG_CHECKING(whether to build all component type and binding extensions)
AC_ARG_ENABLE(all-extensions, [AS_HELP_STRING([--enable-all-extensions],[build all runtime extensions [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_all=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_all=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_all=false ]
)
AM_CONDITIONAL([WANT_ALL], [test x$want_all = xtrue])

AC_MSG_CHECKING(whether to build C++ component type extension)
AC_ARG_ENABLE(cpp, [AS_HELP_STRING([--enable-cpp],[build C++ component type extension [default=yes]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_cpp=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_cpp=true
    ;;
  esac ],
[ AC_MSG_RESULT(yes)
  want_cpp=true ]
)
AM_CONDITIONAL([WANT_CPP], [test x$want_cpp = xtrue])

AC_MSG_CHECKING(whether to build Web Service binding extension)
AC_ARG_ENABLE(wsbinding, [AS_HELP_STRING([--enable-wsbinding],[build Web Service binding extension [default=yes]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_wsbinding=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_wsbinding=true
    ;;
  esac ],
[ AC_MSG_RESULT(yes)
  want_wsbinding=true ]
)
AM_CONDITIONAL([WANT_WSBINDING], [test x$want_wsbinding = xtrue])

AC_MSG_CHECKING(whether to build Axis2C Web Service binding extension)
AC_ARG_WITH(axis2c, [AS_HELP_STRING([--with-axis2c],[build Axis2C Web Service binding extension [default=no]])],
[ case "${withval}" in
  yes)
    AC_MSG_RESULT(yes)
    with_axis2c=true
    ;;
  *)
    AC_MSG_RESULT(no)
    with_axis2c=false
    ;;
  esac ],
[ AC_MSG_RESULT(no)
    with_axis2c=false ]
)
AM_CONDITIONAL([WITH_AXIS2C], [test x$with_axis2c = xtrue])

AC_MSG_CHECKING(whether to build SCA default binding extension)
AC_ARG_ENABLE(scabinding, [AS_HELP_STRING([--enable-scabinding],[build SCA default binding extension [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_scabinding=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_scabinding=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_scabinding=false ]
)
AM_CONDITIONAL([WANT_SCABINDING], [test x$want_scabinding = xtrue])

AC_MSG_CHECKING(whether to build REST binding extension)
AC_ARG_ENABLE(restbinding, [AS_HELP_STRING([--enable-restbinding],[build REST binding extension [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_restbinding=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_restbinding=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_restbinding=false ]
)
AM_CONDITIONAL([WANT_RESTBINDING], [test x$want_restbinding = xtrue])

AC_MSG_CHECKING(whether to build Python component type extension)
AC_ARG_ENABLE(python, [AS_HELP_STRING([--enable-python],[build Python component type extension [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_python=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_python=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_python=false ]
)
AM_CONDITIONAL([WANT_PYTHON], [test x$want_python = xtrue])

AC_MSG_CHECKING(whether to build Ruby component type extension)
AC_ARG_ENABLE(ruby, [AS_HELP_STRING([--enable-ruby],[build Ruby component type extension [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_ruby=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_ruby=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_ruby=false ]
)
AM_CONDITIONAL([WANT_RUBY], [test x$want_ruby = xtrue])

AC_MSG_CHECKING(whether to build PHP component type extension)
AC_ARG_ENABLE(php, [AS_HELP_STRING([--enable-php],[build PHP component type extension [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_php=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_php=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_php=false ]
)
AM_CONDITIONAL([WANT_PHP], [test x$want_php = xtrue])

AC_MSG_CHECKING(whether to build Doxygen documentation)
AC_ARG_ENABLE(doxygen, [AS_HELP_STRING([--enable-doxygen],[build Doxygen documentation [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_doxygen=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_doxygen=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_doxygen=false ]
)
if test x$want_doxygen = xyes; then
  AC_PATH_PROG(DOXYGEN, doxygen, , $PATH)
  if test x$DOXYGEN = x; then
    AC_MSG_ERROR([could not find doxygen])
    want_doxygen=no
  fi
fi
AM_CONDITIONAL([WANT_DOXYGEN], [test x$want_doxygen = xtrue])

AC_CONFIG_FILES([Makefile
                 tools/Makefile
                 tools/scagen/Makefile
                 runtime/Makefile
                 runtime/core/Makefile
                 runtime/core/src/Makefile
                 runtime/extensions/Makefile
                 runtime/extensions/cpp/Makefile
                 runtime/extensions/cpp/src/Makefile
                 runtime/extensions/ws/Makefile
                 runtime/extensions/ws/reference/Makefile
                 runtime/extensions/ws/reference/axis2c/Makefile
                 runtime/extensions/ws/reference/axis2c/src/Makefile
                 runtime/extensions/ws/service/Makefile
                 runtime/extensions/ws/service/axis2c/Makefile
                 runtime/extensions/ws/service/axis2c/src/Makefile
                 runtime/extensions/sca/Makefile
                 runtime/extensions/sca/reference/Makefile
                 runtime/extensions/sca/reference/src/Makefile
                 runtime/extensions/sca/service/Makefile
                 runtime/extensions/sca/service/src/Makefile
                 runtime/extensions/rest/Makefile
                 runtime/extensions/rest/interface/Makefile
                 runtime/extensions/rest/interface/src/Makefile
                 runtime/extensions/rest/reference/Makefile
                 runtime/extensions/rest/reference/curl/Makefile
                 runtime/extensions/rest/reference/curl/src/Makefile
                 runtime/extensions/rest/service/Makefile
                 runtime/extensions/rest/service/httpd/Makefile
                 runtime/extensions/rest/service/httpd/src/Makefile
                 runtime/extensions/python/Makefile
                 runtime/extensions/python/src/Makefile
                 runtime/extensions/ruby/Makefile
                 runtime/extensions/ruby/src/Makefile
                 runtime/extensions/php/Makefile
                 runtime/extensions/php/src/Makefile
                 doc/Makefile
                 doc/Doxyfile
                 ])
AC_OUTPUT

