dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)
AC_INIT(tuscany_sca, 1.0.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

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

AC_CONFIG_FILES([Makefile
                 runtime/Makefile
                 runtime/core/Makefile
                 runtime/core/src/Makefile
                 runtime/axis_binding/Makefile
                 runtime/axis_binding/handler/Makefile
                 runtime/axis_binding/handler/src/Makefile
                 runtime/axis_binding/wrapper/Makefile
                 runtime/axis_binding/wrapper/src/Makefile
                 runtime/core/test/Makefile
                 runtime/core/test/CustomerInfo/Makefile
                 runtime/core/test/MyValue/Makefile
                 runtime/core/test/src/Makefile
                 samples/Makefile
                 samples/MyValue/Makefile
                 samples/MyValue/MyValueModule/Makefile
                 samples/MyValue/MyValueModule/CustomerInfo/Makefile
                 samples/MyValue/MyValueModule/MyValue/Makefile
                 samples/MyValue/Client/Makefile
                 ])
AC_OUTPUT

