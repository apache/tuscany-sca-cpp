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
 * Branched from the original class that was also contributed to the
 * org.apache.axis.tools.common package.
 */

/*
 * A C or C++ macro as it is used in the source code
 */
package org.apache.tuscany.sca.cpp.tools.common;

class MacroPart extends FilePart {
    /**
     * Factory method to create a MacroPart.
     * 
     * @param s
     *            unparsed source code which may start with a define or macro.
     */
    static MacroPart create(String s) {
        String orig = getOriginalText(s);
        if (null == orig)
            return null;
        return new MacroPart(orig);
    }

    MacroPart(String s) {
        super(s, FilePart.MACRO);
    }

    /**
     * @param s
     *            unparsed source code which may start with a define or macro.
     * @return all of s up to the end of the define or macro.
     */
    private static String getOriginalText(String s) {
        String name = getName(s);
        int len = name.length();
        if (null == name)
            return null;
        else if (Configuration.isDefine(name)) {
            return s.substring(0, len);
        } else if (Configuration.isMacro(name)) {
            String rest = s.substring(len);
            len += Utils.findMatching(rest, '(', ')');
            return s.substring(0, len + 1);
        } else
            return null;
    }

    static boolean isAMacro(String s) {
        if (s == null || 0 == s.length())
            return false;
        String name = getName(s);
        return Configuration.isMacro(name) || Configuration.isDefine(name);
    }

    private static String getName(String s) {
        int i;
        for (i = 0; i < s.length(); i++)
            if (!Character.isLetterOrDigit(s.charAt(i)) && '_' != s.charAt(i))
                break;
        if (s.length() == i)
            return null;
        return s.substring(0, i);
    }
}