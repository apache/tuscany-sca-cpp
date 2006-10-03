<!--
   Licensed to the Apache Software Foundation (ASF) under one
   or more contributor license agreements.  See the NOTICE file
   distributed with this work for additional information
   regarding copyright ownership.  The ASF licenses this file
   to you under the Apache License, Version 2.0 (the
   "License"); you may not use this file except in compliance
   with the License.  You may obtain a copy of the License at
   
     http://www.apache.org/licenses/LICENSE-2.0
     
   Unless required by applicable law or agreed to in writing,
   software distributed under the License is distributed on an
   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
   KIND, either express or implied.  See the License for the
   specific language governing permissions and limitations
   under the License.
-->

package org.apache.tuscany.sca.cpp.tools.junit;

import java.io.File;
import java.util.HashSet;
import java.util.Set;

/**
 * This test case will test all the composites placed in the "composites" directory
 * against the results in their repectice "expected_results" folders.
 */
public class TestDeployAssistTool extends TuscanyTestCase {

    String testsDir = TuscanyTestCase.junit_composites;

    Set excludes = new HashSet();

    public TestDeployAssistTool(String arg0) {
        super(arg0);
        excludes.add("MyValueServiceCompositeMissingScaComposite");
        excludes.add("CVS");
    }

    /* main exists to allow running from the test script which will
     * wrap this test case and pipe the satandard output to somewhere
     * for checking. 
     */

    public static void main(String[] args) {
        TestDeployAssistTool test = new TestDeployAssistTool("");
        test.testDeployToolNullParms();
    }

    public void testDeployTool() {
        //Utils.setReportArtefacts(true);
        TestAllCompositesTest t = new TestAllCompositesTest("");
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            String[] test_composites = dir.list();

            for (int i = 0; i < test_composites.length; i++) {
                File test_composite = new File(testsDir, test_composites[i]);
                String composite_name = null;
                if (test_composite.isDirectory()) {
                    composite_name = test_composite.getName();
                    if (!excludes.contains(composite_name.intern())) {
                        t.testCompositeDeploy(composite_name, create_results, "c:\\colin", "cp");
                    }
                }
                System.out.println("");
            }
        }
    }
    
    public void testDeployToolNullParms() {
        //Utils.setReportArtefacts(true);
        TestAllCompositesTest t = new TestAllCompositesTest("");
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            String[] test_composites = dir.list();

            for (int i = 0; i < test_composites.length; i++) {
                File test_composite = new File(testsDir, test_composites[i]);
                String composite_name = null;
                if (test_composite.isDirectory()) {
                    composite_name = test_composite.getName();
                    if (!excludes.contains(composite_name.intern())) {
                        t.testCompositeDeploy(composite_name, create_results, null, null);
                    }
                }
                System.out.println("");
            }
        }
    }
}

