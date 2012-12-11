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

# Enable Javascript support.
AC_MSG_CHECKING([whether to enable Javascript support])
AC_ARG_ENABLE(js, [AS_HELP_STRING([--enable-js], [enable Javascript support [default=no]])],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_js=true
    ;;
  esac ],
[
  AC_MSG_RESULT(no)
])
if test "${want_js}" = "true"; then

  # Configure path to libmozjs includes and lib.
  AC_MSG_CHECKING([for js-include])
  AC_ARG_WITH([js-include], [AC_HELP_STRING([--with-js-include=PATH], [path to installed SpiderMonkey include dir
                [default=/usr/include]])], [
  JS_INCLUDE="${withval}"
  AC_MSG_RESULT("${withval}")
  ], [
  JS_INCLUDE="/usr/include"
  AC_MSG_RESULT(/usr/include)
  ])
  AC_MSG_CHECKING([for js-lib])
  AC_ARG_WITH([js-lib], [AC_HELP_STRING([--with-js-lib=PATH], [path to installed SpiderMonkey lib dir [default=/usr/lib]])], [
  JS_LIB="${withval}"
  AC_MSG_RESULT("${withval}")
  ], [
  JS_LIB="/usr/lib"
  AC_MSG_RESULT(/usr/lib)
  ])
  AC_SUBST(JS_INCLUDE)
  AC_SUBST(JS_LIB)
  LIBS="-L${JS_LIB} ${defaultlibs}"
  AC_CHECK_LIB([mozjs], [JS_NewContext], [], [AC_MSG_ERROR([couldn't find a suitable libmozjs, use --with-js-lib=PATH])])
  AM_CONDITIONAL([WANT_JS], true)
  AC_DEFINE([WANT_JS], 1, [enable Javascript support])

else
  AM_CONDITIONAL([WANT_JS], false)
fi

