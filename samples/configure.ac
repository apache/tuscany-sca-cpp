dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)
AC_INIT(tuscany_sca_samples, 0.1.incubating-M1)
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
                 Calculator/Makefile
                 Calculator/sample.calculator/Makefile
                 Calculator/sample.calculator.client/Makefile
                 Calculator/sample.calculator.wsclient/Makefile
                 RubyCalculator/Makefile
                 RubyCalculator/sample.calculator/Makefile
                 RubyCalculator/sample.calculator.client/Makefile
                 RubyCalculator/sample.calculator.wsclient/Makefile
                 PythonCalculator/Makefile
                 PythonCalculator/sample.calculator/Makefile
                 PythonCalculator/sample.calculator.client/Makefile
                 PythonCalculator/sample.calculator.wsclient/Makefile
                 BigBank/Makefile
                 BigBank/bigbank.account/Makefile
                 BigBank/bigbank.client/Makefile
                 BigBank/bigbank.wsclient/Makefile
                 RubyBank/Makefile
                 RubyBank/bigbank.account/Makefile
                 RubyBank/bigbank.client/Makefile
                 ])
AC_OUTPUT
