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
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_PATH_PROG(MEMCACHED, memcached, , ${PATH})
if test "${MEMCACHED}" = ""; then
  AC_MSG_ERROR([could not find memcached])
fi

# Check for running on Darwin.
AC_MSG_CHECKING([if running on Darwin])
UNAME=`uname -s`
if test "${UNAME}" = "Darwin"; then
  AC_DEFINE([IS_DARWIN], 1, [running on Darwin])
  AC_MSG_RESULT(yes)
  AC_SUBST([libsuffix],[".dylib"])
  AM_CONDITIONAL([DARWIN], true)
else
  AC_MSG_RESULT(no)
  AC_SUBST([libsuffix],[".so"])
  AM_CONDITIONAL([DARWIN], false)
fi

# Configure path to CURL includes and lib.
AC_MSG_CHECKING([for curl])
AC_ARG_WITH([curl], [AC_HELP_STRING([--with-curl=PATH], [path to installed curl [default=/usr]])], [
  CURL_INCLUDE="${withval}/include"
  CURL_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  CURL_INCLUDE="/usr/include"
  CURL_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(CURL_INCLUDE)
AC_SUBST(CURL_LIB)

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

# Configure path to libmozjs includes and lib.
AC_MSG_CHECKING([for js-include])
AC_ARG_WITH([js-include], [AC_HELP_STRING([--with-js-include=PATH], [path to installed SpiderMonkey include dir
             [default=/usr/include/xulrunner-1.9.1.7/unstable]])], [
  JS_INCLUDE="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  JS_INCLUDE="/usr/include/xulrunner-1.9.1.7/unstable"
  AC_MSG_RESULT(/usr/include/xulrunner-1.9.1.7/unstable)
])
AC_MSG_CHECKING([for js-lib])
AC_ARG_WITH([js-lib], [AC_HELP_STRING([--with-js-lib=PATH], [path to installed SpiderMonkey lib dir [default=/usr/lib/xulrunner-1.9.1.7]])], [
  JS_LIB="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  JS_LIB="/usr/lib/xulrunner-1.9.1.7"
  AC_MSG_RESULT(/usr/lib/xulrunner-1.9.1.7)
])
AC_SUBST(JS_INCLUDE)
AC_SUBST(JS_LIB)

# Configure path to APR and HTTPD includes and libs.
AC_MSG_CHECKING([for apr])
AC_ARG_WITH([apr], [AC_HELP_STRING([--with-apr=PATH], [path to installed APR [default=/usr/local/apache2]])], [
  APR_INCLUDE="${withval}/include"
  APR_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  APR_INCLUDE="/usr/local/apache2/include"
  APR_LIB="/usr/local/apache2/lib"
  AC_MSG_RESULT(/usr/local/apache2/lib)
])
AC_SUBST(APR_INCLUDE)
AC_SUBST(APR_LIB)

AC_MSG_CHECKING([for httpd])
AC_ARG_WITH([httpd], [AC_HELP_STRING([--with-httpd=PATH], [path to installed HTTPD [default=/usr/local/apache2]])], [
  HTTPD_INCLUDE="${withval}/include"
  AC_MSG_RESULT("${withval}")
], [
  HTTPD_INCLUDE="/usr/local/apache2/include"
  AC_MSG_RESULT(/usr/local/apache2/lib)
])
AC_SUBST(HTTPD_INCLUDE)

# Configure TUSCANY_SCACPP path variable.
TUSCANY_SCACPP=`echo "${TUSCANY_SCACPP}"`
if test "${TUSCANY_SCACPP}" = ""; then
  pwd=`pwd`
  AC_SUBST([TUSCANY_SCACPP], ["${pwd}"])
fi

# Initialize default GCC C++ and LD options.
cxxflags=""
ldflags="${LDFLAGS}"
defaultlibs="${LIBS}"

# Configure default includes.
cxxflags="${cxxflags} ${INCLUDES} -I. -I${TUSCANY_SCACPP}/kernel -I${APR_INCLUDE} -I${LIBXML2_INCLUDE} -I${JS_INCLUDE} -I${CURL_INCLUDE}"

# Check for libraries required by all modules and add them to LD options.
AC_MSG_NOTICE([checking for required libraries])
LIBS="-L${APR_LIB} ${defaultlibs}"
AC_CHECK_LIB([apr-1], [apr_pool_initialize], [AC_MSG_NOTICE([found])], [AC_MSG_ERROR([couldn't find a suitable libapr-1, use --with-apr=PATH])])
ldflags="${ldflags} -L${APR_LIB} -R${APR_LIB} -lapr-1 -laprutil-1"

# Check for libraries only required by some modules and add their search path to LD options.  
LIBS="-L${CURL_LIB} ${defaultlibs}"
AC_CHECK_LIB([curl], [curl_global_init], [], [AC_MSG_ERROR([couldn't find a suitable libcurl, use --with-curl=PATH])])
LIBS="-L${JS_LIB} ${defaultlibs}"
AC_CHECK_LIB([mozjs], [JS_NewContext], [], [AC_MSG_ERROR([couldn't find a suitable libmozjs, use --with-js-lib=PATH])])
LIBS="-L${LIBXML2_LIB} ${defaultlibs}"
AC_CHECK_LIB([xml2], [xmlInitParser], [], [AC_MSG_ERROR([couldn't find a suitable libxml2, use --with-libxml2=PATH])])
ldflags="${ldflags} -L${CURL_LIB} -R${CURL_LIB} -L${JS_LIB} -R${JS_LIB} -L${LIBXML2_LIB} -R${LIBXML2_LIB}"

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
  cxxflags="${cxxflags} -D_DEBUG -O0 -ggdb -g3 -Werror -Wall -Wextra -Wno-ignored-qualifiers -Winit-self -Wmissing-include-dirs -Wcast-qual -Wcast-align -Wwrite-strings -Wpointer-arith -Wconversion -Waddress -Wlogical-op -Wredundant-decls -std=c++0x -fmessage-length=0"
  ldflags="${ldflags} -pg"
  AM_CONDITIONAL([WANT_MAINTAINER_MODE], true)
  AC_DEFINE([WANT_MAINTAINER_MODE], 1, [compile with debugging and compile-time warnings])
else
  cxxflags="${cxxflags} -O3 -std=c++0x -fmessage-length=0"
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

# Configure path to Python 2.6 includes and lib.
AC_MSG_CHECKING([for python])
AC_ARG_WITH([python], [AC_HELP_STRING([--with-python=PATH], [path to installed Python 2.6 [default=/usr]])], [
  PYTHON_INCLUDE="${withval}/include"
  PYTHON_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  PYTHON_INCLUDE="/usr/include"
  PYTHON_LIB="/usr/lib"
  AC_MSG_RESULT(/usr)
])
AC_SUBST(PYTHON_INCLUDE)
AC_SUBST(PYTHON_LIB)

# Enable Python 2.6 support.
AC_MSG_CHECKING([whether to enable Python support])
AC_ARG_ENABLE(python, [AS_HELP_STRING([--enable-python], [enable Python support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_python=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_python}" = "true"; then
  LIBS="-L${PYTHON_LIB} ${default_LIBS}"
  AC_CHECK_LIB([python2.6], [Py_Initialize], [], [AC_MSG_ERROR([couldn't find a suitable libpython2.6, use --with-python=PATH])])
  AM_CONDITIONAL([WANT_PYTHON], true)
  AC_DEFINE([WANT_PYTHON], 1, [enable Python support])
else
  AM_CONDITIONAL([WANT_PYTHON], false)
fi

# Configure path to Java includes and lib.
AC_MSG_CHECKING([for java])
AC_ARG_WITH([java], [AC_HELP_STRING([--with-java=PATH], [path to installed Java [default=/usr/lib/jvm/default-java]])], [
  JAVA_INCLUDE="${withval}/include"
  JAVA_LIB="${withval}/jre/lib/i386"
  AC_MSG_RESULT("${withval}")
], [
  JAVA_INCLUDE="/usr/lib/jvm/default-java/include"
  JAVA_LIB="/usr/lib/jvm/default-java/jre/lib/i386"
  AC_MSG_RESULT(/usr/lib/jvm/default-java)
])
AC_SUBST(JAVA_INCLUDE)
AC_SUBST(JAVA_LIB)

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
  LIBS="-L${JAVA_LIB} ${default_LIBS}"
  #AC_CHECK_LIB([java], [JNI_OnLoad], [], [AC_MSG_ERROR([couldn't find a suitable libjava, use --with-java=PATH])])
  AC_PROG_JAVAC
  AC_PROG_JAR
  AM_CONDITIONAL([WANT_JAVA], true)
  AC_DEFINE([WANT_JAVA], 1, [enable Java support])
else
  AM_CONDITIONAL([WANT_JAVA], false)
fi

# Configure path to Axis2C includes and lib.
AC_MSG_CHECKING([for axis2c])
AC_ARG_WITH([axis2c], [AC_HELP_STRING([--with-axis2c=PATH], [path to installed Axis2C [default=/usr/local/axis2c]])], [
  AXIS2C_INCLUDE="${withval}/include/axis2-1.6.0"
  AXIS2C_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  AXIS2C_INCLUDE="/usr/local/axis2c/include/axis2-1.6.0"
  AXIS2C_LIB="/usr/local/axis2c/lib"
  AC_MSG_RESULT(/usr/local/axis2c)
])
AC_SUBST(AXIS2C_INCLUDE)
AC_SUBST(AXIS2C_LIB)

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
  LIBS="-L${AXIS2C_LIB} ${default_LIBS}"
  AC_CHECK_LIB([axis2_engine], [axis2_svc_client_create], [], [AC_MSG_ERROR([couldn't find a suitable libaxis2_engine, use --with-axis2c=PATH])])
  AM_CONDITIONAL([WANT_WEBSERVICE], true)
  AC_DEFINE([WANT_WEBSERVICE], 1, [enable Web service component])
else
  AM_CONDITIONAL([WANT_WEBSERVICE], false)
fi

# Enable Store component.
AC_MSG_CHECKING([whether to enable the Store component])
AC_ARG_ENABLE(store, [AS_HELP_STRING([--enable-store], [enable Store component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_store=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_store}" = "true"; then
  AM_CONDITIONAL([WANT_STORE], true)
  AC_DEFINE([WANT_STORE], 1, [enable Store component])
else
  AM_CONDITIONAL([WANT_STORE], false)
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
  AM_CONDITIONAL([WANT_CHAT], true)
  AC_DEFINE([WANT_CHAT], 1, [enable Chat component])
else
  AM_CONDITIONAL([WANT_CHAT], false)
fi

# Configure GCC C++ and LD options.
AC_SUBST([CXXFLAGS], ["${cxxflags}"])
AC_SUBST([LDFLAGS], ["${ldflags}"])
LIBS="${defaultlibs}"

AC_CONFIG_FILES([Makefile
                 kernel/Makefile
                 modules/Makefile
                 modules/scheme/Makefile
                 modules/atom/Makefile
                 modules/json/Makefile
                 modules/scdl/Makefile
                 modules/http/Makefile
                 modules/python/Makefile
                 modules/java/Makefile
                 modules/server/Makefile
                 components/Makefile
                 components/cache/Makefile
                 components/log/Makefile
                 components/chat/Makefile
                 components/queue/Makefile
                 components/store/Makefile
                 components/webservice/Makefile
                 samples/Makefile
                 test/Makefile
                 test/store-scheme/Makefile
                 test/store-cpp/Makefile
                 test/store-python/Makefile
                 test/store-java/Makefile
                 doc/Makefile
                 doc/Doxyfile
                 ])
AC_OUTPUT

