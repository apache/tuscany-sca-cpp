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
                 ])
AC_OUTPUT

