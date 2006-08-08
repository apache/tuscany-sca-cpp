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
 * This test case will test all the composites placed in the "composites" directory
 * against the results in their repectice "expected_results" folders.
 */
public class TestAllCompositesTest extends TuscanyTestCase {

    String testsDir = TuscanyTestCase.junit_composites;

    Set excludes = new HashSet();

    public TestAllCompositesTest(String arg0) {
        super(arg0);
        excludes.add("MyValueServiceCompositeMissingScaComposite");
        excludes.add("CVS");
        excludes.add(".svn");
    }

    
    /* main exists to allow running from the java ant task */
    public static void main(String[] args) {
        TestAllCompositesTest test = new TestAllCompositesTest("");
        test.testAllCompositesRegression();
    }
    
    public void testAllCompositesRegression() {
        File dir = new File(testsDir);

        if (dir.isDirectory()) {
            System.out.println("Testing all composites under "
                    + dir.getAbsolutePath());
            String[] test_composites = dir.list();

            for (int i = 0; i < test_composites.length; i++) {
                File test_composite = new File(testsDir, test_composites[i]);

                String composite_name = null;
                if (test_composite.isDirectory()) {
                    composite_name = test_composite.getName();
                    if (!excludes.contains(composite_name.intern())) {
                        testComposite(composite_name, check_results);
                        System.out.println("Test of composite \"" + composite_name
                                + "\" passed.");
                        continue;
                    } else {
                        System.out
                                .println("Ignoring excluded composite subdirectory \""
                                        + composite_name + "\"");
                    }
                }
            }

        } else {

            fail("Test directory is not a directory! \r The variable org.apache.tuscany.sca.cpp.tools.junit.TuscanyTestCase.root sets the location of the junit input data,\r it is currently set as "
                    + TuscanyTestCase.root + "\rand we expect to find a testinput\\composites directory under there.");
        }

        System.out.println("testAllComposites test passed.");
    }

}