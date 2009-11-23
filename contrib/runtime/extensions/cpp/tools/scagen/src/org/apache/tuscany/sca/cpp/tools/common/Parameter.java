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
 *  
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * A parameter from a method signature. The parameter is the datatype plus its
 * name but not its value.
 */
public class Parameter {
    private ArrayList type = new ArrayList();

    private String name = null;

    private boolean failed = false;

    /**
     * Defaults to a parameter in a parameter list so it HAS a parameter name in
     * it
     */
    Parameter(List parts) {
        this(parts, false);
    }

    /**
     * A parameter which is a return type does not have a parameter name.
     * Parameters in a parameter list do have parameter names embedded in them
     */
    Parameter(List parts, boolean isRetType) {
        if (parts == null || parts.size() == 0)
            return;

        // Tuscany: The original code below from apache axis blocks int
        // getCustomer(long)
        // i.e. no spaces in parameter list.
        // We do not need to deal with "..." as parameters to SCS methods.
        // 
        //        if (!isRetType && parts.size() == 1) {
        //            if ("...".equals(parts.get(0))) {
        //                type.add("...");
        //                name = "";
        //            } else if (!"void".equals(parts.get(0)))
        //                failed = true; // Seems like bad C++ code here
        //            return;
        //        }

        if (isRetType) {
            Iterator it = parts.iterator();
            while (it.hasNext())
                type.add(it.next());

            // Some methods return have void on their signature and others
            // have nothing. So to make them both the same, if a method
            // doesn't return anything make type null.
            // TODO: This assumption is wrong - methods that return nothing
            // default to returning an int!
            if (1 == type.size() && "void".equals(type.get(0)))
                type = new ArrayList();

        } else {
            // Cope with array subscripts [] after the name
            int arrIdx = -1;
            for (int i = 0; i < parts.size(); i++) {
                String tok = (String) parts.get(i);
                if ("[".equals(tok)) {
                    arrIdx = i;
                    break;
                }
            }

            // Find the name
            int nameIdx = parts.size() - 1;
            if (-1 != arrIdx)
                nameIdx = arrIdx - 1;

            // Even in real method declarations, parameters may not have a name
            boolean noName = false;
            name = (String) parts.get(nameIdx);
            // Tuscany: The original code below from apache axis
            // was updated to work with signatures of
            // the form fn(int) a non-named, no-space, parameter list.
            // if (Utils.cPrimitives.contains(name) ||
            // Utils.cTypeQualifiers.contains(name) )
            //
            if (Utils.cPrimitives.contains(name)
                    || Utils.cTypeQualifiers.contains(name)
                    || parts.size() == 1)
                noName = true;

            if (noName) {
                name = null;
                for (int i = 0; i < parts.size(); i++)
                    type.add(parts.get(i));
            } else {
                // Construct the type
                for (int i = 0; i < nameIdx; i++)
                    type.add(parts.get(i));

                if (-1 != arrIdx)
                    for (int i = arrIdx; i < parts.size(); i++)
                        type.add(parts.get(i));
            }
        }
    }

    public boolean failed() {
        return failed;
    }

    public String getType() {
        String s = null;
        Iterator it = type.iterator();
        while (it.hasNext()) {
            String next = (String) it.next();
            if (null == s)
                s = next;
            else if ("*".equals(next) || "&".equals(next))
                s += next;
            else
                s += " " + next;
        }
        return s;
    }

    public String getTypeWithoutConst() {
        String s = null;
        Iterator it = type.iterator();
        while (it.hasNext()) {
            String next = (String) it.next();
            if ("const".equals(next))
                continue;
            else if (null == s)
                s = next;
            else if ("*".equals(next) || "&".equals(next))
                s += next;
            else
                s += " " + next;
        }
        return s;
    }

    public String getName() {
        return name;
    }

    public boolean isVoid() {
        return 0 == type.size();
    }

    public boolean isDotDotDot() {
        return 1 == type.size() && "...".equals(type.get(0));
    }

    /**
     * For two parameters to match their types must match or both be null, but
     * the parameters names don't have to match. Just because a parameter is
     * called something different in a header file as in the the source file
     * doesn't mean it's a different parameter.
     */
    public boolean equals(Object o) {
        if (null == o || !(o instanceof Parameter))
            return false;
        Parameter that = (Parameter) o;
        if (type.size() != that.type.size())
            return false;
        for (int i = 0; i < type.size(); i++) {
            String s1 = (String) type.get(i);
            String s2 = (String) that.type.get(i);
            if (!Utils.safeEquals(s1, s2))
                return false;
        }
        return true;
    }

    public String toString() {
        if (0 == type.size())
            return "void";
        if (null == name)
            return getType();
        return getType() + " " + name;
    }

    public Iterator iterator() {
        if (null == type)
            return null;
        return type.iterator();
    }
}