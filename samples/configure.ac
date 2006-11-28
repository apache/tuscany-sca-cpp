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

# Check for dynamic library suffixes
AC_MSG_CHECKING([checking for dynamic library suffix])
AC_CHECK_HEADER([sys/appleapiopts.h],
  [AC_SUBST([libsuffix],[".dylib"]) AC_MSG_RESULT([using .dylib])],
  [AC_SUBST([libsuffix],[".so"]) AC_MSG_RESULT([using .so])]
)

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
                 CppCalculator/sample.calculator.wsclient/Makefile
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
