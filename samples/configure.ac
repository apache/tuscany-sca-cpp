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
AC_INIT(tuscany_sca_samples, 1.0-incubator-M2)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_PREFIX_DEFAULT(/usr/local/tuscany/sca/samples)

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

AC_MSG_CHECKING(whether to build all samples)
AC_ARG_ENABLE(all-samples, [AS_HELP_STRING([--enable-all-samples],[build all samples [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_all_samples=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_all_samples=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_all_samples=false ]
)
AM_CONDITIONAL([WANT_ALL_SAMPLES], [test x$want_all_samples = xtrue])

AC_MSG_CHECKING(whether to build WS samples)
AC_ARG_ENABLE(wsbinding, [AS_HELP_STRING([--enable-wsbinding],[build Web Service binding samples [default=yes]])],
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

AC_MSG_CHECKING(whether to build Axis2C Web Service samples)
AC_ARG_WITH(axis2c, [AS_HELP_STRING([--with-axis2c],[build Axis2C Web Service samples [default=no]])],
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

AC_MSG_CHECKING(whether to build C++ samples)
AC_ARG_ENABLE(cpp, [AS_HELP_STRING([--enable-cpp],[build C++ samples [default=yes]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_cpp_samples=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_cpp_samples=true
    ;;
  esac ],
[ AC_MSG_RESULT(yes)
  want_cpp_samples=true ]
)
AM_CONDITIONAL([WANT_CPP_SAMPLES], [test x$want_cpp_samples = xtrue])

AC_MSG_CHECKING(whether to build Python samples)
AC_ARG_ENABLE(python, [AS_HELP_STRING([--enable-python],[build Python samples [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_python_samples=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_python_samples=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_python_samples=false ]
)
AM_CONDITIONAL([WANT_PYTHON_SAMPLES], [test x$want_python_samples = xtrue])

AC_MSG_CHECKING(whether to build Ruby samples)
AC_ARG_ENABLE(ruby, [AS_HELP_STRING([--enable-ruby],[build Ruby samples [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_ruby_samples=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_ruby_samples=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_ruby_samples=false ]
)
AM_CONDITIONAL([WANT_RUBY_SAMPLES], [test x$want_ruby_samples = xtrue])

AC_MSG_CHECKING(whether to build PHP samples)
AC_ARG_ENABLE(php, [AS_HELP_STRING([--enable-php],[build PHP samples [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_php_samples=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_php_samples=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)
  want_php_samples=false ]
)
AM_CONDITIONAL([WANT_PHP_SAMPLES], [test x$want_php_samples = xtrue])

AC_CONFIG_FILES([Makefile
                 CppCalculator/Makefile
                 CppCalculator/sample.calculator/Makefile
                 CppCalculator/sample.calculator.client/Makefile
                 RubyCalculator/Makefile
                 RubyCalculator/sample.calculator/Makefile
                 RubyCalculator/sample.calculator.client/Makefile
                 RubyCalculator/sample.calculator.wsclient/Makefile
                 PythonCalculator/Makefile
                 PythonCalculator/sample.calculator/Makefile
                 PythonCalculator/sample.calculator.client/Makefile
                 PythonCalculator/sample.calculator.wsclient/Makefile
                 PythonWeatherForecast/Makefile
                 PythonWeatherForecast/sample.weather/Makefile
                 PythonWeatherForecast/sample.weather.client/Makefile
                 RestCalculator/Makefile
                 RestCalculator/sample.calculator/Makefile
                 RestCalculator/sample.calculator.restclient/Makefile
                 RestCalculator/httpserver/Makefile
                 RestCustomer/Makefile
                 RestCustomer/sample.customer/Makefile
                 RestCustomer/sample.customer.restclient/Makefile
                 RestCustomer/httpserver/Makefile
                 RestYahoo/Makefile
                 RestYahoo/sample.yahoo/Makefile
                 RestYahoo/sample.yahoo.client/Makefile
                 CppBigBank/Makefile
                 CppBigBank/bigbank.account/Makefile
                 CppBigBank/bigbank.client/Makefile
                 CppBigBank/bigbank.wsclient/Makefile
                 CppBigBank/bigbank.phpwsclient/Makefile
                 RubyBigBank/Makefile
                 RubyBigBank/bigbank.account/Makefile
                 RubyBigBank/bigbank.client/Makefile
                 RubyBigBank/bigbank.wsclient/Makefile
                 HttpdBigBank/Makefile
                 HttpdBigBank/bigbank.account/Makefile
                 HttpdBigBank/bigbank.accountdata/Makefile
                 HttpdBigBank/bigbank.wsclient/Makefile
                 HttpdBigBank/httpserver/Makefile
                 ])
AC_OUTPUT
