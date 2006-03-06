/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  Branched from the original class that was also contributed to the 
 *  org.apache.axis.tools.common package.
 *  
 */
package org.apache.tuscany.sca.cpp.tools.common;

/**
 * A function prototype in an include file and possibly in a class definition.
 */
public class PrototypePart extends FilePart {
    private Signature signature;

    public PrototypePart(String s, String className, String namespace) {
        super(s, PROTOTYPE);
        signature = new Signature(s);
        if (null != className)
            signature.setClassName(className);
        if (null != namespace && namespace.length()>0)
            signature.setNamespace(namespace);
    }

    String className() {
        return signature.getClassName();
    }

    public Signature getSignature() {
        return signature;
    }

    public void setScope(String scope) {
        signature.setScope(scope);
    }
}