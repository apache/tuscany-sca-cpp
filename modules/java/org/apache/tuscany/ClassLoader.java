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

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;

/**
 * Class loader used to load SCA component implementation classes.
 */
class ClassLoader extends URLClassLoader {

    ClassLoader(final URL... urls) {
        super(urls);
    }

    /**
     * Create a class loader for an SCA contribution path.
     */
    static java.lang.ClassLoader valueOf(final String path) throws MalformedURLException {
        return new ClassLoader(new File(path).toURI().toURL());
    }

    /**
     * Load a class.
     */
    static Class<?> forName(final String name, final boolean resolve, final java.lang.ClassLoader loader) throws ClassNotFoundException {
        return Class.forName(name, resolve, loader);
    }

    /**
     * Test the class loader.
     */
    static class Test {
        Boolean testClassLoader() {
            try {
                final Class<?> clazz = ClassLoader.forName("test.CalcImpl", true, ClassLoader.valueOf("."));
                assert clazz != null;
            } catch(final MalformedURLException e) {
                throw new RuntimeException(e);
            } catch(final ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
            return true;
        }
    }

    public static void main(final String[] args) {
        System.out.println("Testing...");

        Test.class.getClassLoader().setDefaultAssertionStatus(true);
        new Test().testClassLoader();

        System.out.println("OK");
    }

}
