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

# Check for required programs.
AC_MSG_NOTICE([checking for programs])
if test "${CXX}" = ""; then
  AC_MSG_CHECKING([for clang++])
  if test -x "/usr/bin/clang++"; then
    # Use CLang++/LLVM if available
    CXX=/usr/bin/clang++
    CC=/usr/bin/clang
    AC_MSG_RESULT(/usr/bin/clang++)
    AM_CONDITIONAL([WANT_LLVM], true)
    AC_DEFINE([WANT_LLVM], 1, [compile with clang++/llvm])
  else
    AM_CONDITIONAL([WANT_LLVM], false)
  fi
else
  AM_CONDITIONAL([WANT_LLVM], false)
fi
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Initialize default GCC C++ and LD options.
cxxname=`basename ${CXX}`
if test "${cxxname}" = "clang++"; then
  cxxtype="clang"
else
  cxxtype="gcc"
fi
#cxxflags="${CXXFLAGS}"
cxxflags=""
ldflags="${LDFLAGS}"
defaultlibs="${LIBS}"

# Configure TUSCANY_SCACPP path variable.
TUSCANY_SCACPP=`echo "${TUSCANY_SCACPP}"`
if test "${TUSCANY_SCACPP}" = ""; then
  pwd=`pwd`
  AC_SUBST([TUSCANY_SCACPP], ["${pwd}"])
fi

# Check for required header files.
AC_MSG_NOTICE([checking for header files])
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h sys/time.h])

# Check for typedefs, structures, and compiler characteristics.
AC_MSG_NOTICE([checking for typedefs, structures, and compiler characteristics])
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Check for required library functions.
AC_MSG_NOTICE([checking for library functions])
AC_CHECK_FUNCS([gettimeofday select])

# Check for running on Darwin.
AC_MSG_CHECKING([if running on Darwin])
UNAME=`uname -s`
if test "${UNAME}" = "Darwin"; then
  AC_DEFINE([IS_DARWIN], 1, [running on Darwin])
  AC_MSG_RESULT(yes)
  libsuffix=".dylib"
  AC_SUBST([libsuffix],[".dylib"])
  AM_CONDITIONAL([DARWIN], true)
else
  AC_MSG_RESULT(no)
  libsuffix=".so"
  AC_SUBST([libsuffix],[".so"])
  AM_CONDITIONAL([DARWIN], false)
fi

# Enable debugging and compile-time warnings.
AC_MSG_CHECKING([whether to compile with debugging and compile-time warnings])
AC_ARG_ENABLE(maintainer-mode, [AS_HELP_STRING([--enable-maintainer-mode], [compile with debugging and compile-time warnings [default=no]])],
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
if test "${want_maintainer_mode}" = "true"; then
  cxxflags="${cxxflags} -ggdb -g3 -Werror -Wall -Wextra -Wno-ignored-qualifiers -Wno-strict-aliasing -Winit-self -Wmissing-include-dirs -Wcast-qual -Wcast-align -Wwrite-strings -Wpointer-arith -Waddress -Wredundant-decls -std=c++0x -fmessage-length=0"
  if test "${cxxtype}" = "clang"; then
    cxxflags="${cxxflags} -O1 -stdlib=libc++ -Qunused-arguments -Wno-return-type-c-linkage"
  else
    cxxflags="${cxxflags} -O2 -Wlogical-op -Wconversion -Wno-attributes"
  fi
  AM_CONDITIONAL([WANT_MAINTAINER_MODE], true)
  AC_DEFINE([WANT_MAINTAINER_MODE], 1, [compile with debugging and compile-time warnings])
else
  cxxflags="${cxxflags} -g -std=c++0x -fmessage-length=0"
  if test "${cxxtype}" = "clang"; then
    cxxflags="${cxxflags} -O1 -stdlib=libc++ -Qunused-arguments"
  else
    cxxflags="${cxxflags} -O2"
  fi
  AM_CONDITIONAL([WANT_MAINTAINER_MODE], false)
fi

# Enable profiling with gprof.
AC_MSG_CHECKING([whether to compile with profiling])
AC_ARG_ENABLE(profiling, [AS_HELP_STRING([--enable-profiling], [compile with profiling [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_profiling=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_profiling}" = "true"; then
  cxxflags="${cxxflags} -pg"
  ldflags="${ldflags} -pg"
  AM_CONDITIONAL([WANT_PROFILING], true)
  AC_DEFINE([WANT_PROFILING], 1, [compile with profiling])
else
  AM_CONDITIONAL([WANT_PROFILING], false)
fi

# Enable memory checking with electric fence.
AC_MSG_CHECKING([whether to enable electric fence])
AC_ARG_ENABLE(efence, [AS_HELP_STRING([--enable-efence], [link with electric fence [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_efence=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_efence}" = "true"; then
  ldflags="${ldflags} -lefence"
  AM_CONDITIONAL([WANT_EFENCE], true)
  AC_DEFINE([WANT_EFENCE], 1, [link with electric fence])
else
  AM_CONDITIONAL([WANT_EFENCE], false)
fi

# Enable usage of mmap for memory allocation.
AC_MSG_CHECKING([whether to use mmap for memory allocation])
AC_ARG_ENABLE(malloc-mmap, [AS_HELP_STRING([--enable-malloc-mmap], [use mmap for memory allocation [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_malloc_mmap=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_malloc_mmap}" = "true"; then
  AM_CONDITIONAL([WANT_MALLOC_MMAP], true)
  AC_DEFINE([WANT_MALLOC_MMAP], 1, [use mmap for memory allocation])
else
  AM_CONDITIONAL([WANT_MALLOC_MMAP], false)
fi

# Enable multi-threading support.
AC_MSG_CHECKING([whether to compile for multi-threaded execution])
AC_ARG_ENABLE(threads, [AS_HELP_STRING([--enable-threads], [compile for multi-threaded execution [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_threads=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_threads}" = "true"; then
  AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([couldn't find a suitable libpthread])])
  LIBS="${defaultlibs}"
  cxxflags="${cxxflags} -D_REENTRANT"
  ldflags="${ldflags} -lpthread"
  AM_CONDITIONAL([WANT_THREADS], true)
  AC_DEFINE([WANT_THREADS], 1, [compile for multi-threaded execution])
else
  AM_CONDITIONAL([WANT_THREADS], false)
fi

# Configure exuberant ctags.
TAGSFILE="`pwd`/tags"
AC_SUBST([CTAGSFLAGS], ["${CTAGSFLAGS} --c++-kinds=+p --fields=+iaS --extra=+q --append --tag-relative=yes -f ${TAGSFILE}"])

# Enable Doxygen documentation.
AC_MSG_CHECKING([whether to build Doxygen documentation])
AC_ARG_ENABLE(doxygen, [AS_HELP_STRING([--enable-doxygen], [build Doxygen documentation [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_doxygen=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_doxygen}" = "true"; then
    AC_PATH_PROG(DOXYGEN, doxygen, , ${PATH})
    if test "${DOXYGEN}" = ""; then
      AC_MSG_ERROR([could not find doxygen])
    fi
  AM_CONDITIONAL([WANT_DOXYGEN], true)
  AC_DEFINE([WANT_DOXYGEN], 1, [build Doxygen documentation])
else
  AM_CONDITIONAL([WANT_DOXYGEN], false)
fi

# Configure path to CURL.
AC_MSG_CHECKING([for curl])
AC_ARG_WITH([curl], [AC_HELP_STRING([--with-curl=PATH], [path to installed curl [default=/usr]])], [
  CURL_PREFIX="${withval}"
  LIBCURL_INCLUDE="${withval}/include"
  LIBCURL_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  CURL_PREFIX="/usr"
  LIBCURL_INCLUDE="/usr/include"
  LIBCURL_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(CURL_PREFIX)
AC_DEFINE_UNQUOTED([CURL_PREFIX], "${CURL_PREFIX}", [path to installed curl])
AC_SUBST(LIBCURL_INCLUDE)
AC_SUBST(LIBCURL_LIB)
LIBS="-L${LIBCURL_LIB} ${defaultlibs}"
AC_CHECK_LIB([curl], [curl_global_init], [], [AC_MSG_ERROR([couldn't find a suitable libcurl, use --with-libcurl=PATH])])

# Configure path to libxml2 includes and lib.
AC_MSG_CHECKING([for libxml2])
AC_ARG_WITH([libxml2], [AC_HELP_STRING([--with-libxml2=PATH], [path to installed libxml2 [default=/usr]])], [
  LIBXML2_INCLUDE="${withval}/include/libxml2"
  LIBXML2_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  LIBXML2_INCLUDE="/usr/include/libxml2"
  LIBXML2_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(LIBXML2_INCLUDE)
AC_SUBST(LIBXML2_LIB)
LIBS="-L${LIBXML2_LIB} ${defaultlibs}"
AC_CHECK_LIB([xml2], [xmlInitParser], [], [AC_MSG_ERROR([couldn't find a suitable libxml2, use --with-libxml2=PATH])])

# Configure path to jansson includes and lib.
AC_MSG_CHECKING([for jansson])
AC_ARG_WITH([jansson], [AC_HELP_STRING([--with-jansson=PATH], [path to installed jansson [default=/usr]])], [
  JANSSON_INCLUDE="${withval}/include"
  JANSSON_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  JANSSON_INCLUDE="/usr/include"
  JANSSON_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(JANSSON_INCLUDE)
AC_SUBST(JANSSON_LIB)
LIBS="-L${JANSSON_LIB} ${defaultlibs}"
AC_CHECK_LIB([jansson], [json_loads], [], [AC_MSG_ERROR([couldn't find a suitable libjansson, use --with-jansson=PATH])])

# Configure path to Apache APR and HTTPD includes and libs.
AC_MSG_CHECKING([for apr])
AC_ARG_WITH([apr], [AC_HELP_STRING([--with-apr=PATH], [path to installed Apache APR [default=/usr]])], [
  APR_INCLUDE="${withval}/include/apr-1"
  APR_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  APR_INCLUDE="/usr/include/apr-1"
  APR_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(APR_INCLUDE)
AC_SUBST(APR_LIB)
LIBS="-L${APR_LIB} ${defaultlibs}"
AC_CHECK_LIB([apr-1], [apr_pool_initialize], [], [AC_MSG_ERROR([couldn't find a suitable libapr-1, use --with-apr=PATH])])

AC_MSG_CHECKING([for apr-util])
AC_ARG_WITH([apr-util], [AC_HELP_STRING([--with-apr-util=PATH], [path to installed Apache APR util [default=/usr]])], [
  APRUTIL_INCLUDE="${withval}/include/apr-1"
  APRUTIL_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  APRUTIL_INCLUDE="/usr/include/apr-1"
  APRUTIL_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(APRUTIL_INCLUDE)
AC_SUBST(APRUTIL_LIB)
LIBS="-L${APRUTIL_LIB} ${defaultlibs}"
AC_CHECK_LIB([aprutil-1], [apr_memcache_server_create], [], [AC_MSG_ERROR([couldn't find a suitable libaprutil-1, use --with-apr-util=PATH])])

AC_MSG_CHECKING([for httpd])
AC_ARG_WITH([httpd], [AC_HELP_STRING([--with-httpd=PATH], [path to installed Apache HTTPD [default=/usr]])], [
  HTTPD_PREFIX="${withval}"
  HTTPD_APACHECTL_PREFIX="${withval}/bin/apachectl"
  HTTPD_MODULES_PREFIX="${withval}"
  HTTPD_INCLUDE="${withval}/include"
  AC_MSG_RESULT("${withval}")
], [
  HTTPD_PREFIX="/usr"
  HTTPD_APACHECTL_PREFIX="/usr/sbin/apache2ctl"
  HTTPD_MODULES_PREFIX="/usr/lib/apache2"
  HTTPD_INCLUDE="/usr/include/apache2"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(HTTPD_PREFIX)
AC_DEFINE_UNQUOTED([HTTPD_PREFIX], "${HTTPD_PREFIX}", [path to installed Apache HTTPD])
AC_SUBST(HTTPD_APACHECTL_PREFIX)
AC_SUBST(HTTPD_MODULES_PREFIX)
AC_SUBST(HTTPD_INCLUDE)
AC_MSG_CHECKING([for ${APACHECTL_PREFIX}])
if test -x "${HTTPD_APACHECTL_PREFIX}"; then
    AC_MSG_RESULT(found)
else
    AC_MSG_ERROR([couldn't find apachectl, use --with-httpd=PATH])
fi
AC_MSG_CHECKING([for ${HTTPD_PREFIX}/bin/htpasswd])
if test -x "${HTTPD_PREFIX}/bin/htpasswd"; then
    AC_MSG_RESULT(found)
else
    AC_MSG_ERROR([couldn't find htpasswd, use --with-httpd=PATH])
fi

# Configure path to memcached.
AC_MSG_CHECKING([for memcached])
AC_ARG_WITH([memcached], [AC_HELP_STRING([--with-memcached=PATH], [path to installed memcached [default=/usr]])], [
  MEMCACHED_PREFIX="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  MEMCACHED_PREFIX="/usr"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(MEMCACHED_PREFIX)
AC_DEFINE_UNQUOTED([MEMCACHED_PREFIX], "${MEMCACHED_PREFIX}", [path to installed memcached])
AC_MSG_CHECKING([for ${MEMCACHED_PREFIX}/bin/memcached])
if test -x "${MEMCACHED_PREFIX}/bin/memcached"; then
    AC_MSG_RESULT(found)
else
    AC_MSG_ERROR([couldn't find memcached, use --with-memcached=PATH])
fi

# Configure path to tinycdb.
AC_MSG_CHECKING([for tinycdb])
AC_ARG_WITH([tinycdb], [AC_HELP_STRING([--with-tinycdb=PATH], [path to installed tinycdb [default=/usr]])], [
  TINYCDB_PREFIX="${withval}"
  TINYCDB_INCLUDE="${withval}/include"
  TINYCDB_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  TINYCDB_PREFIX="/usr"
  TINYCDB_INCLUDE="/usr/include"
  TINYCDB_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(TINYCDB_PREFIX)
AC_DEFINE_UNQUOTED([TINYCDB_PREFIX], "${TINYCDB_PREFIX}", [path to installed tinycdb])
AC_SUBST(TINYCDB_INCLUDE)
AC_SUBST(TINYCDB_LIB)
LIBS="-L${TINYCDB_LIB} ${defaultlibs}"
AC_CHECK_LIB([cdb], [cdb_make_start], [], [AC_MSG_ERROR([couldn't find a suitable libcdb, use --with-tinycdb=PATH])])

# Configure default includes and ldflags
cxxflags="${cxxflags} ${INCLUDES} -I. -I${TUSCANY_SCACPP}/kernel -I${APR_INCLUDE} -I${APRUTIL_INCLUDE} -I${HTTPD_INCLUDE} -I${LIBXML2_INCLUDE} -I${JANSSON_INCLUDE} -I${LIBCURL_INCLUDE}"
ldflags="${ldflags} -ldl -L${APR_LIB} -R${APR_LIB} -lapr-1 -L${APRUTIL_LIB} -R${APRUTIL_LIB} -laprutil-1"
ldflags="${ldflags} -L${LIBCURL_LIB} -R${LIBCURL_LIB} -L${JANSSON_LIB} -R${JANSSON_LIB} -L${LIBXML2_LIB} -R${LIBXML2_LIB}"

# Enable Python support.
AC_MSG_CHECKING([whether to enable Python support])
AC_ARG_ENABLE(python, [AS_HELP_STRING([--enable-python], [enable Python support [default=yes]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_python=true
    ;;
  esac ],
[
  AC_MSG_RESULT(yes)
  want_python=true
])
if test "${want_python}" = "true"; then

  # Configure path to Python includes and lib.
  AC_MSG_CHECKING([for python])
  AC_ARG_WITH([python], [AC_HELP_STRING([--with-python=PATH], [path to installed Python [default=/usr]])], [
    PYTHON_PREFIX="${withval}"
    PYTHON_INCLUDE="${withval}/include"
    PYTHON_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    PYTHON_PREFIX="/usr"
    PYTHON_INCLUDE="/usr/include"
    PYTHON_LIB="/usr/lib"
    AC_MSG_RESULT(/usr)
  ])
  AC_SUBST(PYTHON_PREFIX)
  AC_DEFINE_UNQUOTED([PYTHON_PREFIX], "${PYTHON_PREFIX}", [path to installed Python])
  AC_SUBST(PYTHON_INCLUDE)
  AC_SUBST(PYTHON_LIB)
  LIBS="-L${PYTHON_LIB} ${defaultlibs}"
  if test -f "${PYTHON_LIB}/libpython2.7${libsuffix}"; then
      PYTHON_VERSION="2.7"
      AC_DEFINE([PYTHON_VERSION], 27, [Python version])
  else
      PYTHON_VERSION="2.6"
      AC_DEFINE([PYTHON_VERSION], 26, [Python version])
  fi
  AC_SUBST(PYTHON_VERSION)
  AC_CHECK_LIB([python${PYTHON_VERSION}], [Py_Initialize], [], [AC_MSG_ERROR([couldn't find a suitable libpython, use --with-python=PATH])])
  AM_CONDITIONAL([WANT_PYTHON], true)
  AC_DEFINE([WANT_PYTHON], 1, [enable Python support])

else
  AM_CONDITIONAL([WANT_PYTHON], false)
fi

# Configure path to Page Speed.
AC_MSG_CHECKING([for pagespeed])
AC_ARG_WITH([pagespeed], [AC_HELP_STRING([--with-pagespeed=PATH], [path to installed Page Speed [default=/usr/bin]])], [
  PAGESPEED_PREFIX="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  PAGESPEED_PREFIX="/usr/bin"
  AC_MSG_RESULT(/usr/bin)
])
AC_SUBST(PAGESPEED_PREFIX)

# Enable Page Speed optimizations.
AC_MSG_CHECKING([whether to run Page Speed optimizations])
AC_ARG_ENABLE(pagespeed, [AS_HELP_STRING([--enable-pagespeed], [run Page Speed optimizations [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_pagespeed=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_pagespeed}" = "true"; then
    AC_PATH_PROG(MINIFY_HTML, minify_html_bin, , ${PAGESPEED_PREFIX})
    if test "${MINIFY_HTML}" = ""; then
      AC_MSG_ERROR([could not find Page Speed minify_html_bin])
    fi
    AC_PATH_PROG(MINIFY_JS, jsmin_bin, , ${PAGESPEED_PREFIX})
    if test "${MINIFY_JS}" = ""; then
      AC_MSG_ERROR([could not find Page Speed jsmin_bin])
    fi
  AM_CONDITIONAL([WANT_PAGESPEED], true)
  AC_DEFINE([WANT_PAGESPEED], 1, [run Page Speed optimizations])
  AC_DEFINE_UNQUOTED([PAGESPEED_PREFIX], "${PAGESPEED_PREFIX}", [path to installed Page Speed])
else
  AM_CONDITIONAL([WANT_PAGESPEED], false)
fi

# Enable OpenCL support.
AC_MSG_CHECKING([whether to enable OpenCL support])
AC_ARG_ENABLE(opencl, [AS_HELP_STRING([--enable-opencl], [enable OpenCL support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_opencl=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_opencl}" = "true"; then

  # Configure path to OpenCL includes and lib.
  AC_MSG_CHECKING([for opencl])
  AC_ARG_WITH([opencl-include], [AC_HELP_STRING([--with-opencl-include=PATH], [path to installed OpenCL 1.1 include dir [default=/usr/include]])], [
    OPENCL_INCLUDE="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    OPENCL_INCLUDE="/usr/include"
    AC_MSG_RESULT(/usr/include)
  ])
  AC_SUBST(OPENCL_INCLUDE)
  AC_ARG_WITH([opencl-lib], [AC_HELP_STRING([--with-opencl-lib=PATH], [path to installed OpenCL 1.1 lib dir [default=/usr/lib]])], [
    OPENCL_LIB="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    OPENCL_LIB="/usr/lib"
    AC_MSG_RESULT(/usr/lib)
  ])
  AC_SUBST(OPENCL_LIB)
  if test "${UNAME}" != "Darwin"; then
    LIBS="-L${OPENCL_LIB} ${defaultlibs}"
    AC_CHECK_LIB([OpenCL], [clGetDeviceIDs], [], [AC_MSG_ERROR([couldn't find a suitable libOpenCL, use --with-opencl=PATH])])
  fi
  AM_CONDITIONAL([WANT_OPENCL], true)
  AC_DEFINE([WANT_OPENCL], 1, [enable OpenCL support])

else
  AM_CONDITIONAL([WANT_OPENCL], false)
fi

# Enable Java support.
AC_MSG_CHECKING([whether to enable Java support])
AC_ARG_ENABLE(java, [AS_HELP_STRING([--enable-java], [enable Java support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_java=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_java}" = "true"; then

  # Configure path to Java includes and lib.
  AC_MSG_CHECKING([for java])
  AC_ARG_WITH([java], [AC_HELP_STRING([--with-java=PATH], [path to installed Java [default=/usr/lib/jvm/default-java]])], [
    JAVA_PREFIX="${withval}"
    JAVA_INCLUDE="${withval}/include"
    JAVAC="${withval}/bin/javac"
    JAR="${withval}/bin/jar" 
    AC_MSG_RESULT("${withval}")
  ], [
    JAVA_PREFIX="/usr/lib/jvm/default-java"
    JAVA_INCLUDE="/usr/lib/jvm/default-java/include"
    JAVAC="/usr/lib/jvm/default-java/bin/javac"
    JAR="/usr/lib/jvm/default-java/bin/jar" 
    AC_MSG_RESULT(/usr/lib/jvm/default-java)
  ])
  AC_SUBST(JAVA_PREFIX)
  AC_DEFINE_UNQUOTED([JAVA_PREFIX], "${JAVA_PREFIX}", [path to installed Java])
  AC_SUBST(JAVA_INCLUDE)
  AC_SUBST(JAVAC)
  AC_SUBST(JAR)

  # Detect most common Java VMs
  JAVA_LIBJAVA_SO=`find ${JAVA_PREFIX}/jre/lib -name libjava.so`
  if test "${JAVA_LIBJAVA_SO}" != ""; then
    JAVA_LIBJAVA=`dirname "${JAVA_LIBJAVA_SO}"`
    JAVA_LIBJVM_SO=`find ${JAVA_PREFIX}/jre/lib -name libjvm.so`
    JAVA_J9_VM=`echo "${JAVA_LIBJVM}" | grep "j9vm"`
    if test "${JAVA_J9_VM}" != ""; then
      # IBM J9 VM
      AC_MSG_NOTICE([checking for J9 Java VM])
      JAVA_LIBJVM=`dirname "${JAVA_LIBJVM_SO}"`
      JAVA_CHECK_LIB="-L${JAVA_LIBJAVA} -Wl,-R${JAVA_LIBJAVA} -L${JAVA_LIBJVM} -Wl,-R${JAVA_LIBJVM}"
      LIBS="${JAVA_CHECK_LIB} ${defaultlibs}"
      AC_CHECK_LIB([java], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -ljava -ljvm -ljsig"], [], [-ljvm -ljsig])
      if test "${JAVA_LDFLAGS}" != ""; then
        AC_DEFINE([JAVA_J9_VM], 1, [J9 Java VM])
      fi
    else
      # SUN JDK or OpenJDK VM
      AC_MSG_NOTICE([checking for OpenJDK Java VM])
      JAVA_LIBJVM_SO=`find ${JAVA_PREFIX}/jre/lib -name libjvm.so | grep server`
      JAVA_LIBJVM=`dirname "${JAVA_LIBJVM_SO}"`
      AC_MSG_NOTICE([libjava ${JAVA_LIBJAVA} libjvm ${LIBJVM}])
      JAVA_CHECK_LIB="-L${JAVA_LIBJAVA} -Wl,-R${JAVA_LIBJAVA} -L${JAVA_LIBJVM} -Wl,-R${JAVA_LIBJVM}"
      LIBS="${JAVA_CHECK_LIB} ${defaultlibs}"
      AC_CHECK_LIB([java], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -ljava -ljvm -lverify"], [], [-ljvm -lverify])
      if test "${JAVA_LDFLAGS}" != ""; then
        AC_DEFINE([JAVA_OPENJDK_VM], 1, [OpenJDK Java VM])
      fi
    fi
  else
    JAVA_LIBHARMONYVM_SO=`find ${JAVA_PREFIX}/jre/bin -name libharmonyvm.so`
    if test "${JAVA_LIBHARMONYVM_SO}" != ""; then
      # Apache Harmony VM
      JAVA_LIBHARMONYVM=`dirname "${JAVA_LIBHARMONYVM_SO}"`
      JAVA_LIBJAVA=`dirname "${JAVA_LIBHARMONYVM}"`
      AC_MSG_NOTICE([checking for Apache Harmony Java VM])
      JAVA_CHECK_LIB="-L${JAVA_LIBJAVA} -Wl,-R${JAVA_LIBJAVA} -L${JAVA_LIBHARMONYVM} -Wl,-R${JAVA_LIBHARMONYVM}"
      LIBS="${JAVA_CHECK_LIB} ${defaultlibs}"
      AC_CHECK_LIB([harmonyvm], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -lharmonyvm -lhythr -licuuc -lch ${JAVA_LIBHARMONYVM}/libicudata.so.34"], [], [-lhythr -licuuc -lch ${JAVA_LIBHARMONYVM}/libicudata.so.34])
      if test "${JAVA_LDFLAGS}" != ""; then
        AC_DEFINE([JAVA_HARMONY_VM], 1, [Apache Harmony Java VM])
      fi
    fi
  fi
  if test "${JAVA_LDFLAGS}" = ""; then
    AC_MSG_ERROR([couldn't find a suitable Java JNI library, use --with-java=PATH])
  fi
  AC_MSG_CHECKING([for javac])
  if test -x "${JAVAC}"; then
    AC_MSG_RESULT("${JAVAC}")
  else
    AC_MSG_ERROR([couldn't find a suitable javac tool, use --with-java=PATH])
  fi
  AC_MSG_CHECKING([for jar])
  if test -x "${JAR}"; then
    AC_MSG_RESULT("${JAR}")
  else
    AC_MSG_ERROR([couldn't find a suitable jar tool, use --with-java=PATH])
  fi 
  AM_CONDITIONAL([WANT_JAVA], true)
  AC_DEFINE([WANT_JAVA], 1, [enable Java support])
else
  AM_CONDITIONAL([WANT_JAVA], false)
  JAVA_LDFLAGS=""
fi
AC_SUBST(JAVA_LDFLAGS)

# Enable support for OpenID.
AC_MSG_CHECKING([whether to enable OpenID support])
AC_ARG_ENABLE(openid, [AS_HELP_STRING([--enable-openid], [enable OpenID support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_openid=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_openid}" = "true"; then

  # Configure path to mod-auth-openid
  AC_MSG_CHECKING([for mod-auth-openid])
  AC_ARG_WITH([mod-auth-openid], [AC_HELP_STRING([--with-mod-auth-openid=PATH], [path to installed mod-auth-openid [default=/usr/local]])], [
    MODAUTHOPENID_PREFIX="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    MODAUTHOPENID_PREFIX="/usr/local/"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(MODAUTHOPENID_PREFIX)
  AC_DEFINE_UNQUOTED([MODAUTHOPENID_PREFIX], "${MODAUTHOPENID_PREFIX}", [path to installed mod-auth-openid])

  AM_CONDITIONAL([WANT_OPENID], true)
  AC_DEFINE([WANT_OPENID], 1, [enable OpenID support])

else
  AM_CONDITIONAL([WANT_OPENID], false)
fi

# Enable support for OAuth.
AC_MSG_CHECKING([whether to enable OAuth support])
AC_ARG_ENABLE(oauth, [AS_HELP_STRING([--enable-oauth], [enable OAuth support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_oauth=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_oauth}" = "true"; then

  # Configure path to Liboauth includes and lib.
  AC_MSG_CHECKING([for liboauth])
  AC_ARG_WITH([liboauth], [AC_HELP_STRING([--with-liboauth=PATH], [path to liboauth [default=/usr/local]])], [
    LIBOAUTH_INCLUDE="${withval}/include"
    LIBOAUTH_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    LIBOAUTH_INCLUDE="/usr/local/include"
    LIBOAUTH_LIB="/usr/local/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(LIBOAUTH_INCLUDE)
  AC_SUBST(LIBOAUTH_LIB)
  LIBS="-L${LIBOAUTH_LIB} ${defaultlibs}"
  AC_CHECK_LIB([oauth], [oauth_sign_url2], [], [AC_MSG_ERROR([couldn't find a suitable liboauth, use --with-liboauth=PATH])], [-lssl])
  
  AM_CONDITIONAL([WANT_OAUTH], true)
  AC_DEFINE([WANT_OAUTH], 1, [enable OAuth support])

else
  AM_CONDITIONAL([WANT_OAUTH], false)
fi

# Enable support for modsecurity.
AC_MSG_CHECKING([whether to enable mod-security support])
AC_ARG_ENABLE(mod-security, [AS_HELP_STRING([--enable-mod-security], [enable mod-security support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_modsecurity=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_modsecurity}" = "true"; then

  # Configure path to mod-security
  AC_MSG_CHECKING([for mod-security])
  AC_ARG_WITH([mod-security], [AC_HELP_STRING([--with-mod-security=PATH], [path to installed mod-security [default=/usr/local]])], [
    MODSECURITY_PREFIX="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    MODSECURITY_PREFIX="/usr/local/"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(MODSECURITY_PREFIX)
  AC_DEFINE_UNQUOTED([MODSECURITY_PREFIX], "${MODSECURITY_PREFIX}", [path to installed mod-security])

  AM_CONDITIONAL([WANT_MODSECURITY], true)
  AC_DEFINE([WANT_MODSECURITY], 1, [enable mod-security support])

else
  AM_CONDITIONAL([WANT_MODSECURITY], false)
fi

# Enable support for Google AppEngine.
AC_MSG_CHECKING([whether to enable Google AppEngine support])
AC_ARG_ENABLE(gae, [AS_HELP_STRING([--enable-gae], [enable Google AppEngine support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_gae=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_gae}" = "true"; then

  # Configure path to Google AppEngine SDK.
  AC_MSG_CHECKING([for gae])
  AC_ARG_WITH([gae], [AC_HELP_STRING([--with-gae=PATH], [path to installed Google AppEngine 1.3.2 [default=${HOME}/google_appengine]])], [
    GAE_PREFIX="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    GAE_PREFIX="${HOME}/google_appengine"
    AC_MSG_RESULT(${HOME}/google_appengine)
  ])
  AC_SUBST(GAE_PREFIX)

  AM_CONDITIONAL([WANT_GAE], true)
  AC_DEFINE([WANT_GAE], 1, [enable Google AppEngine support])
else
  AM_CONDITIONAL([WANT_GAE], false)
fi

# Enable support for Libcloud.
AC_MSG_CHECKING([whether to enable libcloud support])
AC_ARG_ENABLE(libcloud, [AS_HELP_STRING([--enable-libcloud], [enable libcloud support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_libcloud=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_libcloud}" = "true"; then

  # Configure path to Apache Libcloud.
  AC_MSG_CHECKING([for libcloud])
  AC_ARG_WITH([libcloud], [AC_HELP_STRING([--with-libcloud=PATH], [path to installed Apache libcloud [default=/usr/local]])], [
    LIBCLOUD_LIB="${withval}/lib/python"
    AC_MSG_RESULT("${withval}")
  ], [
    LIBCLOUD_LIB="/usr/local/lib/python${PYTHON_VERSION}/site-packages"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(LIBCLOUD_LIB)

  AM_CONDITIONAL([WANT_LIBCLOUD], true)
  AC_DEFINE([WANT_LIBCLOUD], 1, [enable libcloud support])
else
  AM_CONDITIONAL([WANT_LIBCLOUD], false)
fi

# Enable Web service component.
AC_MSG_CHECKING([whether to enable the Web service component])
AC_ARG_ENABLE(webservice, [AS_HELP_STRING([--enable-webservice], [enable Web service component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_webservice=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_webservice}" = "true"; then

  # Configure path to Apache Axis2C includes and lib.
  AC_MSG_CHECKING([for axis2c])
  AC_ARG_WITH([axis2c], [AC_HELP_STRING([--with-axis2c=PATH], [path to installed Apache Axis2C [default=/usr/local/axis2c]])], [
    AXIS2C_PREFIX="${withval}"
    AXIS2C_INCLUDE="${withval}/include/axis2-1.6.0"
    AXIS2C_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    AXIS2C_PREFIX="/usr/local/axis2c"
    AXIS2C_INCLUDE="/usr/local/axis2c/include/axis2-1.6.0"
    AXIS2C_LIB="/usr/local/axis2c/lib"
    AC_MSG_RESULT(/usr/local/axis2c)
  ])
  AC_SUBST(AXIS2C_PREFIX)
  AC_DEFINE_UNQUOTED([AXIS2C_PREFIX], "${AXIS2C_PREFIX}", [path to installed Axis2C])
  AC_SUBST(AXIS2C_INCLUDE)
  AC_SUBST(AXIS2C_LIB)
  LIBS="-L${AXIS2C_LIB} ${defaultlibs}"
  AC_CHECK_LIB([axis2_engine], [axis2_svc_client_create], [], [AC_MSG_ERROR([couldn't find a suitable libaxis2_engine, use --with-axis2c=PATH])])

  AM_CONDITIONAL([WANT_WEBSERVICE], true)
  AC_DEFINE([WANT_WEBSERVICE], 1, [enable Web service component])

else
  AM_CONDITIONAL([WANT_WEBSERVICE], false)
fi

# Enable SQL Database component.
AC_MSG_CHECKING([whether to enable the SQL Database component])
AC_ARG_ENABLE(sqldb, [AS_HELP_STRING([--enable-sqldb], [enable SQL Database component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_sqldb=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_sqldb}" = "true"; then

  # Configure path to PostgreSQL.
  AC_MSG_CHECKING([for pgsql])
  AC_ARG_WITH([pgsql], [AC_HELP_STRING([--with-pgsql=PATH], [path to installed PostgreSQL [default=/usr/local/pgsql]])], [
    PGSQL_PREFIX="${withval}"
    PGSQL_INCLUDE="${withval}/include"
    PGSQL_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    PGSQL_PREFIX="/usr/local/pgsql"
    PGSQL_INCLUDE="/usr/local/pgsql/include"
    PGSQL_LIB="/usr/local/pgsql/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(PGSQL_PREFIX)
  AC_DEFINE_UNQUOTED([PGSQL_PREFIX], "${PGSQL_PREFIX}", [path to installed PostgreSQL])
  AC_SUBST(PGSQL_INCLUDE)
  AC_SUBST(PGSQL_LIB)

  # Configure path to PgBouncer.
  AC_MSG_CHECKING([for pgbouncer])
  AC_ARG_WITH([pgbouncer], [AC_HELP_STRING([--with-pgbouncer=PATH], [path to installed PgBouncer [default=/usr/local]])], [
    PGBOUNCER_PREFIX="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    PGBOUNCER_PREFIX="/usr/local"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(PGBOUNCER_PREFIX)
  AC_DEFINE_UNQUOTED([PGBOUNCER_PREFIX], "${PGBOUNCER_PREFIX}", [path to installed PgBouncer])

  AM_CONDITIONAL([WANT_SQLDB], true)
  AC_DEFINE([WANT_SQLDB], 1, [enable SQL Database component])
else
  AM_CONDITIONAL([WANT_SQLDB], false)
fi

# Enable Queue component.
AC_MSG_CHECKING([whether to enable the Queue component])
AC_ARG_ENABLE(queue, [AS_HELP_STRING([--enable-queue], [enable Queue component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_queue=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_queue}" = "true"; then

  # Configure path to Apache Qpid/C++.
  AC_MSG_CHECKING([for qpidc])
  AC_ARG_WITH([qpidc], [AC_HELP_STRING([--with-qpidc=PATH], [path to installed Apache Qpid/C++ [default=/usr/local]])], [
    QPIDC_PREFIX="${withval}"
    QPIDC_INCLUDE="${withval}/include"
    QPIDC_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    QPIDC_PREFIX="/usr/local"
    QPIDC_INCLUDE="/usr/local/include"
    QPIDC_LIB="/usr/local/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(QPIDC_PREFIX)
  AC_DEFINE_UNQUOTED([QPIDC_PREFIX], "${QPIDC_PREFIX}", [path to installed Qpid/C++])
  AC_SUBST(QPIDC_INCLUDE)
  AC_SUBST(QPIDC_LIB)

  if test "${want_threads}" != "true"; then
    AC_MSG_ERROR([--enable-queue requires multi-threading, use --enable-threads])
  fi
  LIBS="-L${QPIDC_LIB} ${defaultlibs}"
  AC_CHECK_LIB([qpidclient], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libqpidclient, use --with-qpidc=PATH])])

  AM_CONDITIONAL([WANT_QUEUE], true)
  AC_DEFINE([WANT_QUEUE], 1, [enable Queue component])
else
  AM_CONDITIONAL([WANT_QUEUE], false)
fi

# Enable Chat component.
AC_MSG_CHECKING([whether to enable the Chat component])
AC_ARG_ENABLE(chat, [AS_HELP_STRING([--enable-chat], [enable Chat component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_chat=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_chat}" = "true"; then

  # Configure path to Libstrophe includes and lib.
  AC_MSG_CHECKING([for libstrophe])
  AC_ARG_WITH([libstrophe], [AC_HELP_STRING([--with-libstrophe=PATH], [path to libstrophe [default=/usr/local]])], [
    LIBSTROPHE_INCLUDE="${withval}/include"
    LIBSTROPHE_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    LIBSTROPHE_INCLUDE="/usr/local/include"
    LIBSTROPHE_LIB="/usr/local/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(LIBSTROPHE_INCLUDE)
  AC_SUBST(LIBSTROPHE_LIB)
  if test "${want_threads}" != "true"; then
    AC_MSG_ERROR([--enable-chat requires multi-threading, use --enable-threads])
  fi
  LIBS="-L${LIBSTROPHE_LIB} ${defaultlibs}"
  AC_CHECK_LIB([strophe], [xmpp_initialize], [], [AC_MSG_ERROR([couldn't find a suitable libstrophe, use --with-libstrophe=PATH])], [-lssl -lresolv])
  
  # Configure path to Vysper
  AC_MSG_CHECKING([for vysper])
  AC_ARG_WITH([vysper], [AC_HELP_STRING([--with-vysper=PATH], [path to Apache Vysper [default=${HOME}/vysper-1.0.0]])], [
    VYSPER_PREFIX="${withval}"
    AC_MSG_RESULT("${withval}")
  ], [
    VYSPER_PREFIX="${HOME}/vysper-1.0.0"
    AC_MSG_RESULT(${HOME}/vysper-1.0.0)
  ])
  AC_SUBST(VYSPER_PREFIX)
  AC_CHECK_FILE([${VYSPER_PREFIX}/lib/vysper-core-0.6.jar], [want_vysper=true], [])
  if test "${want_vysper}" = "true"; then
    AM_CONDITIONAL([WANT_VYSPER], true)
  else
    AM_CONDITIONAL([WANT_VYSPER], false)
  fi

  AM_CONDITIONAL([WANT_CHAT], true)
  AC_DEFINE([WANT_CHAT], 1, [enable Chat component])

else
  AM_CONDITIONAL([WANT_CHAT], false)
  AM_CONDITIONAL([WANT_VYSPER], false)
fi

# Enable Log component.
AC_MSG_CHECKING([whether to enable the Log component])
AC_ARG_ENABLE(log, [AS_HELP_STRING([--enable-log], [enable Log component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_log=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_log}" = "true"; then

  # Configure path to Apache Thrift (and Facebook fb303).
  AC_MSG_CHECKING([for thrift])
  AC_ARG_WITH([thrift], [AC_HELP_STRING([--with-thrift=PATH], [path to installed Apache Thrift [default=/usr/local]])], [
    THRIFT_PREFIX="${withval}"
    THRIFT_INCLUDE="${withval}/include/thrift"
    THRIFT_LIB="${withval}/lib"
    FB303_PREFIX="${withval}/contrib/fb303"
    FB303_INCLUDE="${withval}/contrib/fb303/include/thrift"
    FB303_LIB="${withval}/contrib/fb303/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    THRIFT_PREFIX="/usr/local"
    THRIFT_INCLUDE="/usr/local/include"
    THRIFT_LIB="/usr/local/lib"
    FB303_PREFIX="/usr/local"
    FB303_INCLUDE="/usr/local/include"
    FB303_LIB="/usr/local/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(THRIFT_PREFIX)
  AC_DEFINE_UNQUOTED([THRIFT_PREFIX], "${THRIFT_PREFIX}", [path to installed Apache Thrift])
  AC_SUBST(THRIFT_INCLUDE)
  AC_SUBST(THRIFT_LIB)
  AC_SUBST(FB303_PREFIX)
  AC_SUBST(FB303_INCLUDE)
  AC_SUBST(FB303_LIB)
  
  # Configure path to Facebook Scribe.
  AC_MSG_CHECKING([for scribe])
  AC_ARG_WITH([scribe], [AC_HELP_STRING([--with-scribe=PATH], [path to installed Facebook Scribe [default=/usr/local]])], [
    SCRIBE_PREFIX="${withval}"
    SCRIBE_INCLUDE="${withval}/include"
    SCRIBE_LIB="${withval}/lib"
    AC_MSG_RESULT("${withval}")
  ], [
    SCRIBE_PREFIX="/usr/local"
    SCRIBE_INCLUDE="/usr/local/include"
    SCRIBE_LIB="/usr/local/lib"
    AC_MSG_RESULT(/usr/local)
  ])
  AC_SUBST(SCRIBE_PREFIX)
  AC_DEFINE_UNQUOTED([SCRIBE_PREFIX], "${SCRIBE_PREFIX}", [path to installed Facebook Scribe])
  AC_SUBST(SCRIBE_INCLUDE)
  AC_SUBST(SCRIBE_LIB)

  LIBS="-L${THRIFT_LIB} -L${FB303_LIB} -L${SCRIBE_LIB} ${defaultlibs}"
  AC_CHECK_LIB([thrift], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libthrift, use --with-thrift=PATH])])
  AC_CHECK_LIB([fb303], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libfb303, use --with-thrift=PATH])])
  AC_CHECK_LIB([scribe], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libscribe, use --with-scribe=PATH])])

  AM_CONDITIONAL([WANT_LOG], true)
  AC_DEFINE([WANT_LOG], 1, [enable Log component])
else
  AM_CONDITIONAL([WANT_LOG], false)
fi

# Configure GCC C++ and LD options.
AC_SUBST([DEFAULT_CXXFLAGS], ["${cxxflags}"])
AC_SUBST([CXXFLAGS], ["${cxxflags}"])
AC_SUBST([LDFLAGS], ["${ldflags}"])
AC_SUBST([DEFAULT_LDFLAGS], ["${ldflags}"])
LIBS="${defaultlibs}"

AC_CONFIG_FILES([Makefile
                 etc/Makefile
                 kernel/Makefile
                 modules/Makefile
                 modules/scheme/Makefile
                 modules/atom/Makefile
                 modules/rss/Makefile
                 modules/js/Makefile
                 modules/json/Makefile
                 modules/scdl/Makefile
                 modules/http/Makefile
                 modules/server/Makefile
                 modules/python/Makefile
                 modules/opencl/Makefile
                 modules/java/Makefile
                 modules/openid/Makefile
                 modules/oauth/Makefile
                 modules/wsgi/Makefile
                 modules/xml/Makefile
                 components/Makefile
                 components/cache/Makefile
                 components/log/Makefile
                 components/chat/Makefile
                 components/constdb/Makefile
                 components/filedb/Makefile
                 components/http/Makefile
                 components/smtp/Makefile
                 components/queue/Makefile
                 components/sqldb/Makefile
                 components/webservice/Makefile
                 hosting/Makefile
                 hosting/server/Makefile
                 samples/Makefile
                 samples/store-scheme/Makefile
                 samples/store-cpp/Makefile
                 samples/store-python/Makefile
                 samples/store-java/Makefile
                 samples/store-gae/Makefile
                 samples/store-sql/Makefile
                 samples/store-constdb/Makefile
                 samples/store-vhost/Makefile
                 samples/store-cluster/Makefile
                 samples/relay-python/Makefile
                 samples/relay-gae/Makefile
                 doc/Makefile
                 doc/Doxyfile
                 macos/Makefile
                 ubuntu/Makefile
                 ])
AC_OUTPUT

