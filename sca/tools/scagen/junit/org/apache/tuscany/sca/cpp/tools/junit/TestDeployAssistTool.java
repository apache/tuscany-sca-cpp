/*
 * IBM Confidential
 *
 * OCO Source Materials
 *
 * <PID TBD>
 *
 * (C) Copyright IBM Corp. 2005
 *
 * The source code for this program is not published
 * or otherwise divested of its trade secrets,
 * irrespective of what has been deposited with the
 * U. S. Copyright Office.
 */
package org.apache.tuscany.sca.cpp.tools.junit;

import java.io.File;
import java.util.HashSet;
import java.util.Set;

import org.apache.tuscany.sca.cpp.tools.common.Utils;
import org.apache.tuscany.sca.cpp.tools.junit.TuscanyTestCase;

/**
 * This test case will test all the modules placed in the "modules" directory
 * against the results in their repectice "expected_results" folders.
 */
public class TestDeployAssistTool extends TuscanyTestCase {

    String testsDir = TuscanyTestCase.junit_modules;

    Set excludes = new HashSet();

    public TestDeployAssistTool(String arg0) {
        super(arg0);
        excludes.add("MyValueServiceModuleMissingScaModule");
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
        TestAllModulesTest t = new TestAllModulesTest("");
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            String[] test_modules = dir.list();

            for (int i = 0; i < test_modules.length; i++) {
                File test_module = new File(testsDir, test_modules[i]);
                String module_name = null;
                if (test_module.isDirectory()) {
                    module_name = test_module.getName();
                    if (!excludes.contains(module_name.intern())) {
                        t.testModuleDeploy(module_name, create_results, "c:\\colin", "cp");
                    }
                }
                System.out.println("");
            }
        }
    }
    
    public void testDeployToolNullParms() {
        //Utils.setReportArtefacts(true);
        TestAllModulesTest t = new TestAllModulesTest("");
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            String[] test_modules = dir.list();

            for (int i = 0; i < test_modules.length; i++) {
                File test_module = new File(testsDir, test_modules[i]);
                String module_name = null;
                if (test_module.isDirectory()) {
                    module_name = test_module.getName();
                    if (!excludes.contains(module_name.intern())) {
                        t.testModuleDeploy(module_name, create_results, null, null);
                    }
                }
                System.out.println("");
            }
        }
    }
}

