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
AC_INIT(tuscany_sca_php, 1.0-incubator-M3)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(tuscany_sca_php_config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/tuscany/sca/extensions/php)

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


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 samples/Makefile
                 samples/PHPCalculator/Makefile
                 samples/PHPCalculator/sample.calculator/Makefile
                 samples/PHPCalculator/sample.calculator.client/Makefile
                 ])
AC_OUTPUT

