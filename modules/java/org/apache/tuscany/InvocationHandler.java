/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.tuscany;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

/**
 * Proxy Invocation handler used to represent SCA component references.
 */
class InvocationHandler implements java.lang.reflect.InvocationHandler {
    final long lambda;

    InvocationHandler(final long lambda) {
        this.lambda = lambda;
    }

    /**
     * Create a proxy for an interface and the lambda function representing
     * an SCA component reference.
     */
    static Object valueOf(final Class<?> iface, final long lambda) {
        return Proxy.newProxyInstance(iface.getClassLoader(), new Class[]{iface}, new InvocationHandler(lambda));
    }

    /**
     * Proxy invocation of a C++ function.
     */
    public native Object invoke(final Object proxy, final Method method, final Object[] args) throws Throwable;

    /**
     * Return the stack trace of an exception.
     */
    static String stackTrace(final Throwable e) {
        final StringWriter sw = new StringWriter();
        final PrintWriter pw = new PrintWriter(sw);
        e.printStackTrace(pw);
        return sw.toString();
    }
}
