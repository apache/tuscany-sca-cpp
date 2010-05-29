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
AC_MSG_CHECKING([for gcc-4.5])
if test -x "/usr/bin/g++-4.5"; then
    # Use GCC 4.5 if available
    CXX=/usr/bin/g++-4.5
    CPP=/usr/bin/cpp-4.5
    CC=/usr/bin/gcc-4.5
    AC_MSG_RESULT(/usr/bin/gcc-4.5)
    AM_CONDITIONAL([WANT_GCC45], true)
    AC_DEFINE([WANT_GCC45], 1, [compile with gcc-4.5])
else
    AM_CONDITIONAL([WANT_GCC45], false)
fi
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

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
AC_SUBST(LIBCURL_INCLUDE)
AC_SUBST(LIBCURL_LIB)

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
xulrunner=`ls /usr/include | grep "xulrunner" | tail -1`
if test "$xulrunner" = ""; then
  xulrunner="xulrunner-1.9.1.8"
fi
AC_ARG_WITH([js-include], [AC_HELP_STRING([--with-js-include=PATH], [path to installed SpiderMonkey include dir
             [default=/usr/include/$xulrunner/unstable]])], [
  JS_INCLUDE="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  JS_INCLUDE="/usr/include/$xulrunner/unstable"
  AC_MSG_RESULT(/usr/include/$xulrunner/unstable)
])
AC_MSG_CHECKING([for js-lib])
AC_ARG_WITH([js-lib], [AC_HELP_STRING([--with-js-lib=PATH], [path to installed SpiderMonkey lib dir [default=/usr/lib/$xulrunner]])], [
  JS_LIB="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  JS_LIB="/usr/lib/$xulrunner"
  AC_MSG_RESULT(/usr/lib/$xulrunner)
])
AC_SUBST(JS_INCLUDE)
AC_SUBST(JS_LIB)

# Configure path to Apache APR and HTTPD includes and libs.
AC_MSG_CHECKING([for apr])
AC_ARG_WITH([apr], [AC_HELP_STRING([--with-apr=PATH], [path to installed Apache APR [default=/usr/local/apache2]])], [
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
AC_ARG_WITH([httpd], [AC_HELP_STRING([--with-httpd=PATH], [path to installed Apache HTTPD [default=/usr/local/apache2]])], [
  HTTPD_PREFIX="${withval}"
  HTTPD_INCLUDE="${withval}/include"
  AC_MSG_RESULT("${withval}")
], [
  HTTPD_PREFIX="/usr/local/apache2"
  HTTPD_INCLUDE="/usr/local/apache2/include"
  AC_MSG_RESULT(/usr/local/apache2/lib)
])
AC_SUBST(HTTPD_PREFIX)
AC_SUBST(HTTPD_INCLUDE)

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
AC_SUBST(TINYCDB_INCLUDE)
AC_SUBST(TINYCDB_LIB)

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
cxxflags="${cxxflags} ${INCLUDES} -I. -I${TUSCANY_SCACPP}/kernel -I${APR_INCLUDE} -I${LIBXML2_INCLUDE} -I${JS_INCLUDE} -I${LIBCURL_INCLUDE}"

# Check for libraries required by all modules and add them to LD options.
AC_MSG_NOTICE([checking for required libraries])
LIBS="-L${APR_LIB} ${defaultlibs}"
AC_CHECK_LIB([apr-1], [apr_pool_initialize], [AC_MSG_NOTICE([found])], [AC_MSG_ERROR([couldn't find a suitable libapr-1, use --with-apr=PATH])])
ldflags="${ldflags} -ldl -L${APR_LIB} -R${APR_LIB} -lapr-1 -laprutil-1"

# Check for libraries only required by some modules and add their search path to LD options.  
LIBS="-L${LIBCURL_LIB} ${defaultlibs}"
AC_CHECK_LIB([curl], [curl_global_init], [], [AC_MSG_ERROR([couldn't find a suitable libcurl, use --with-libcurl=PATH])])
LIBS="-L${JS_LIB} ${defaultlibs}"
AC_CHECK_LIB([mozjs], [JS_NewContext], [], [AC_MSG_ERROR([couldn't find a suitable libmozjs, use --with-js-lib=PATH])])
LIBS="-L${LIBXML2_LIB} ${defaultlibs}"
AC_CHECK_LIB([xml2], [xmlInitParser], [], [AC_MSG_ERROR([couldn't find a suitable libxml2, use --with-libxml2=PATH])])
ldflags="${ldflags} -L${LIBCURL_LIB} -R${LIBCURL_LIB} -L${JS_LIB} -R${JS_LIB} -L${LIBXML2_LIB} -R${LIBXML2_LIB}"

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
AC_SUBST(JAVA_INCLUDE)
AC_SUBST(JAVAC)
AC_SUBST(JAR)

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
  # Detect most common Java VMs
  if test -f "${JAVA_PREFIX}/jre/lib/i386/libjava.so"; then
    if test -f "${JAVA_PREFIX}/jre/lib/i386/server/libjvm.so"; then
      # Server VM
      AC_MSG_NOTICE([checking for server Java VM])
      JAVA_CHECK_LIB="-L${JAVA_PREFIX}/jre/lib/i386 -R${JAVA_PREFIX}/jre/lib/i386 -L${JAVA_PREFIX}/jre/lib/i386/server -R${JAVA_PREFIX}/jre/lib/i386/server"
      LIBS="${JAVA_CHECK_LIB} ${default_LIBS}"
      AC_CHECK_LIB([java], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -ljava -ljvm -lverify"], [], [-ljvm -lverify])
      if test "${JAVA_LDFLAGS}" != ""; then
        AC_DEFINE([JAVA_SERVER_VM], 1, [Server Java VM])
      fi
    else
      if test -f "${JAVA_PREFIX}/jre/lib/i386/j9vm/libjvm.so"; then
        # J9 VM
        AC_MSG_NOTICE([checking for J9 Java VM])
        JAVA_CHECK_LIB="-L${JAVA_PREFIX}/jre/lib/i386 -R${JAVA_PREFIX}/jre/lib/i386 -L${JAVA_PREFIX}/jre/lib/i386/j9vm -R${JAVA_PREFIX}/jre/lib/i386/j9vm"
        LIBS="${JAVA_CHECK_LIB} ${default_LIBS}"
        AC_CHECK_LIB([java], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -ljava -ljvm -ljsig"], [], [-ljvm -ljsig])
        if test "${JAVA_LDFLAGS}" != ""; then
          AC_DEFINE([JAVA_J9_VM], 1, [J9 Java VM])
        fi
      fi
    fi
  else
    if test -f "${JAVA_PREFIX}/jre/bin/default/libharmonyvm.so"; then
      # Apache Harmony VM
      AC_MSG_NOTICE([checking for Apache Harmony Java VM])
      JAVA_CHECK_LIB="-L${JAVA_PREFIX}/jre/bin -R${JAVA_PREFIX}/jre/bin -L${JAVA_PREFIX}/jre/bin/default -R${JAVA_PREFIX}/jre/bin/default"
      LIBS="${JAVA_CHECK_LIB} ${default_LIBS}"
      AC_CHECK_LIB([harmonyvm], [JNI_CreateJavaVM], [JAVA_LDFLAGS="${JAVA_CHECK_LIB} -lharmonyvm -lhythr -licuuc -lch ${JAVA_PREFIX}/jre/bin/default/libicudata.so.34"], [], [-lhythr -licuuc -lch ${JAVA_PREFIX}/jre/bin/default/libicudata.so.34])
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

# Configure path to Google AppEngine SDK.
AC_MSG_CHECKING([for gae])
AC_ARG_WITH([gae], [AC_HELP_STRING([--with-gae=PATH], [path to installed Google AppEngine 1.3.2 [default=$HOME/google_appengine]])], [
  GAE_PREFIX="${withval}"
  AC_MSG_RESULT("${withval}")
], [
  GAE_PREFIX="$HOME/google_appengine"
  AC_MSG_RESULT($HOME/google_appengine)
])
AC_SUBST(GAE_PREFIX)

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
  AM_CONDITIONAL([WANT_GAE], true)
  AC_DEFINE([WANT_GAE], 1, [enable Google AppEngine support])
else
  AM_CONDITIONAL([WANT_GAE], false)
fi

# Configure path to Apache Libcloud.
AC_MSG_CHECKING([for libcloud])
AC_ARG_WITH([libcloud], [AC_HELP_STRING([--with-libcloud=PATH], [path to installed Apache Libcloud [default=/usr/local]])], [
  LIBCLOUD_LIB="${withval}/lib/python"
  AC_MSG_RESULT("${withval}")
], [
  LIBCLOUD_LIB="/usr/local/lib/python2.6/site-packages"
  AC_MSG_RESULT(/usr/local)
])
AC_SUBST(LIBCLOUD_LIB)

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

# Enable NoSQL database component.
AC_MSG_CHECKING([whether to enable the NoSQL database component])
AC_ARG_ENABLE(nosqldb, [AS_HELP_STRING([--enable-nosqldb], [enable NoSQL database component [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_nosqldb=true
    ;;
  esac ],
[ AC_MSG_RESULT(no)])
if test "${want_nosqldb}" = "true"; then
  AM_CONDITIONAL([WANT_NOSQLDB], true)
  AC_DEFINE([WANT_NOSQLDB], 1, [enable NoSQL database component])
else
  AM_CONDITIONAL([WANT_NOSQLDB], false)
fi

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
AC_SUBST(PGSQL_INCLUDE)
AC_SUBST(PGSQL_LIB)

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
  AM_CONDITIONAL([WANT_SQLDB], true)
  AC_DEFINE([WANT_SQLDB], 1, [enable SQL Database component])
else
  AM_CONDITIONAL([WANT_SQLDB], false)
fi

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
AC_SUBST(QPIDC_INCLUDE)
AC_SUBST(QPIDC_LIB)

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
  if test "${want_threads}" != "true"; then
    AC_MSG_ERROR([--enable-queue requires multi-threading, use --enable-threads])
  fi
  LIBS="-L${QPIDC_LIB} ${default_LIBS}"
  AC_CHECK_LIB([qpidclient], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libqpidclient, use --with-qpidc=PATH])])
  AM_CONDITIONAL([WANT_QUEUE], true)
  AC_DEFINE([WANT_QUEUE], 1, [enable Queue component])
else
  AM_CONDITIONAL([WANT_QUEUE], false)
fi

# Configure path to Libstrophe includes and lib.
AC_MSG_CHECKING([for libstrophe])
AC_ARG_WITH([libstrophe], [AC_HELP_STRING([--with-libstrophe=PATH], [path to libstrophe [default=${HOME}/libstrophe-bin]])], [
  LIBSTROPHE_INCLUDE="${withval}/include"
  LIBSTROPHE_LIB="${withval}/lib"
  AC_MSG_RESULT("${withval}")
], [
  LIBSTROPHE_INCLUDE="${HOME}/libstrophe-bin/include"
  LIBSTROPHE_LIB="${HOME}/libstrophe-bin/lib"
  AC_MSG_RESULT(${HOME}/libstrophe-bin)
])
AC_SUBST(LIBSTROPHE_INCLUDE)
AC_SUBST(LIBSTROPHE_LIB)

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
  if test "${want_threads}" != "true"; then
    AC_MSG_ERROR([--enable-chat requires multi-threading, use --enable-threads])
  fi
  LIBS="-L${LIBSTROPHE_LIB} ${default_LIBS}"
  AC_CHECK_LIB([strophe], [xmpp_initialize], [], [AC_MSG_ERROR([couldn't find a suitable libstrophe, use --with-libstrophe=PATH])], [-lexpat -lssl -lresolv])
  AM_CONDITIONAL([WANT_CHAT], true)
  AC_DEFINE([WANT_CHAT], 1, [enable Chat component])

  # Check for Vysper
  AC_CHECK_FILE([${VYSPER_PREFIX}/lib/vysper-core-0.5.jar], [want_vysper=true], [])
  if test "${want_vysper}" = "true"; then
    AM_CONDITIONAL([WANT_VYSPER], true)
  else
    AM_CONDITIONAL([WANT_VYSPER], false)
  fi
else
  AM_CONDITIONAL([WANT_CHAT], false)
  AM_CONDITIONAL([WANT_VYSPER], false)
fi

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
AC_SUBST(SCRIBE_INCLUDE)
AC_SUBST(SCRIBE_LIB)

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
  LIBS="-L${THRIFT_LIB} -L${FB303_LIB} -L${SCRIBE_LIB} ${default_LIBS}"
  AC_CHECK_LIB([thrift], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libthrift, use --with-thrift=PATH])])
  AC_CHECK_LIB([fb303], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libfb303, use --with-thrift=PATH])])
  AC_CHECK_LIB([scribe], [_init], [], [AC_MSG_ERROR([couldn't find a suitable libscribe, use --with-scribe=PATH])])
  AM_CONDITIONAL([WANT_LOG], true)
  AC_DEFINE([WANT_LOG], 1, [enable Log component])
else
  AM_CONDITIONAL([WANT_LOG], false)
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
                 modules/wsgi/Makefile
                 components/Makefile
                 components/cache/Makefile
                 components/log/Makefile
                 components/chat/Makefile
                 components/nosqldb/Makefile
                 components/queue/Makefile
                 components/sqldb/Makefile
                 components/webservice/Makefile
                 samples/Makefile
                 samples/store-scheme/Makefile
                 samples/store-cpp/Makefile
                 samples/store-python/Makefile
                 samples/store-java/Makefile
                 samples/store-gae/Makefile
                 samples/store-sql/Makefile
                 samples/store-nosql/Makefile
                 doc/Makefile
                 doc/Doxyfile
                 ])
AC_OUTPUT

