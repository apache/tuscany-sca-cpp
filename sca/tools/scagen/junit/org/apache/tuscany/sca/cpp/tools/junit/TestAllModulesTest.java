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

import org.apache.tuscany.sca.cpp.tools.junit.TuscanyTestCase;

/**
 * This test case will test all the modules placed in the "modules" directory
 * against the results in their repectice "expected_results" folders.
 */
public class TestAllModulesTest extends TuscanyTestCase {

    String testsDir = TuscanyTestCase.junit_modules;

    Set excludes = new HashSet();

    public TestAllModulesTest(String arg0) {
        super(arg0);
        excludes.add("MyValueServiceModuleMissingScaModule");
        excludes.add("CVS");
        excludes.add(".svn");
    }

    
    /* main exists to allow running from the java ant task */
    public static void main(String[] args) {
        TestAllModulesTest test = new TestAllModulesTest("");
        test.testAllModulesRegression();
    }
    
    public void testAllModulesRegression() {
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            System.out.println("Testing all modules under "
                    + dir.getAbsolutePath());
            String[] test_modules = dir.list();

            for (int i = 0; i < test_modules.length; i++) {
                File test_module = new File(testsDir, test_modules[i]);

                String module_name = null;
                if (test_module.isDirectory()) {
                    module_name = test_module.getName();
                    if (!excludes.contains(module_name.intern())) {
                        testModule(module_name, check_results);
                        System.out.println("Test of module \"" + module_name
                                + "\" passed.");
                        continue;
                    } else {
                        System.out
                                .println("Ignoring excluded module subdirectory \""
                                        + module_name + "\"");
                    }
                }
            }

        } else {

            fail("Test directory is not a directory! \r The variable org.apache.tuscany.sca.cpp.tools.junit.TuscanyTestCase.root sets the location of the junit input data,\r it is currently set as "
                    + TuscanyTestCase.root + "\rand we expect to find a testinput\\modules directory under there.");
        }

        System.out.println("testAllModules test passed.");
    }

}