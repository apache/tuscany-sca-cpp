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

/**
 * A piece of C++ source code
 */
public class FilePart {
    public final static int UNKNOWN = 0;

    public final static int COMMENT = 1;

    public final static int METHOD = 2;

    public final static int FIELD = 3;

    public final static int BEGINSCOPE = 4;

    public final static int ENDSCOPE = 5;

    public final static int DIRECTIVE = 6;

    public final static int WHITESPACE = 7;

    public final static int MACRO = 8;

    public final static int CLASSATTRIBUTE = 9;

    public final static int ENUM = 10;

    public final static int PROTOTYPE = 11;

    public final static int TYPEDEF = 12;

    protected String cppsource;

    protected int type;

    FilePart(String s, int type) {
        cppsource = s;
        this.type = type;
    }

    public int getType() {
        return type;
    }

    int length() {
        return cppsource.length();
    }

    public String toString() {
        return cppsource;
    }
}