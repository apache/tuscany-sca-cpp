/**
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

/* @version $Rev$ $Date$ */


/*
 *  Branched from the original class that was also contributed to the 
 *  org.apache.axis.tools.common package.
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Encapsulates the tool's configuration file
 */
public class Configuration {
    private static Set files = new HashSet();

    private static Set classes = new HashSet();

    private static Set methods = new HashSet();

    private static Set macros = new HashSet();

    private static Set defines = new HashSet();

    private static Set attributes = new HashSet();

    private static Map others = new HashMap();

    /**
     * No one creates an instance of this class.
     */
    private Configuration() {
    }

    /**
     * Reads in the configuration file
     */
    public static void initialise(String filename) throws Exception {
        File file = new File(filename);
        FileReader fr = new FileReader(file);
        BufferedReader br = new BufferedReader(fr);
        String line = br.readLine();
        for (int lineno = 1; null != line; lineno++, line = br.readLine()) {
            // Ignore lines starting with a # (comments) and blank lines
            if (line.startsWith("#"))
                continue;
            boolean blank = true;
            for (int i = 0; i < line.length() && blank; i++)
                if (!Character.isWhitespace(line.charAt(i)))
                    blank = false;
            if (blank)
                continue;

            int equals = line.indexOf("=");
            if (-1 == equals)
                Utils.rude("Bad line in configuration file " + filename
                        + " lineno " + lineno);
            String key = line.substring(0, equals).trim();
            String value = line.substring(equals + 1).trim();
            if ("excludefile".equals(key)) {
                files.add(value);
            } else if ("excludeclass".equals(key)) {
                classes.add(value);
            } else if ("excludemethod".equals(key)) {
                methods.add(value);
            } else if ("macro".equals(key)) {
                macros.add(value);
            } else if ("define".equals(key)) {
                defines.add(value);
            } else if ("attribute".equals(key)) {
                attributes.add(value);
            } else {
                others.put(key, value);
            }
        }
    }

    public static boolean fileExcluded(String s) {
        return files.contains(s);
    }

    public static boolean classExcluded(String s) {
        return classes.contains(s);
    }

    public static boolean methodExcluded(String className, String method) {
        return methods.contains(className + "::" + method);
    }

    public static boolean isMacro(String s) {
        return macros.contains(s);
    }

    public static boolean isDefine(String s) {
        return defines.contains(s);
    }

    public static boolean isAttribute(String s) {
        return attributes.contains(s);
    }

    public static String getConfigured(String key) {
        return (String) others.get(key);
    }
}