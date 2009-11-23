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

package org.apache.tuscany.sca.cpp.tools.junit;

/**
 * This is a "run this first" testcase that just tests some simple operations of
 * the scagen tools it uses the
 * Hydra.sca.core\test\testSCASystem\composites\MyValueServiceComposite SCA composite as
 * input data.
 */
public class SimplePublicPrivateProtectedTest extends TuscanyTestCase {

    /**
     * Constructor for CustomerInfoTestOLD.
     * 
     * @param arg0
     */
    public SimplePublicPrivateProtectedTest(String arg0) {
        super(arg0);
    }

    /**
     * Test the Hydra.sca.core\test\testSCASystem\composites\MyValueServiceComposite
     * proxy and wrapper generation.
     */
    public void testEnvHandler() {

        testComposite("SimplePublicPrivateProtectedTest", check_results);
    }

}