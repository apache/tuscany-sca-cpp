/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

#include "interop.h"

using namespace std;
/*
 * The interop tests exercised here do not actually interoperate with other
 * SDO implementations but simply check that and data that is read
 * in and written out remains unchanged
 */
int  main (int argc, char** argv)
{
	char *commonDir = "..\\..\\..\\..\\..\\interop\\src\\main\\resources\\";

	cout << "SOME TESTS COMMENTED OUT - CHECK SOURCE \n";

	Interop::test1ReadAndWriteXML(commonDir, "interop01");
    Interop::test1ReadAndWriteXML(commonDir, "interop02");
    Interop::test1ReadAndWriteXML(commonDir, "interop03");     
    //Interop::test1ReadAndWriteXML(commonDir, "interop04");
    Interop::test1ReadAndWriteXML(commonDir, "interop05");
    Interop::test1ReadAndWriteXML(commonDir, "interop06");
    Interop::test1ReadAndWriteXML(commonDir, "interop07");
    Interop::test1ReadAndWriteXML(commonDir, "interop08");
    Interop::test1ReadAndWriteXML(commonDir, "interop09");
    Interop::test1ReadAndWriteXML(commonDir, "interop10");
    Interop::test1ReadAndWriteXML(commonDir, "interop11");
    Interop::test1ReadAndWriteXML(commonDir, "interop12");
    //Interop::test1ReadAndWriteXML(commonDir, "interop13");
    Interop::test1ReadAndWriteXML(commonDir, "interop14");
    Interop::test1ReadAndWriteXML(commonDir, "interop15");
    Interop::test1ReadAndWriteXML(commonDir, "interop16");
    Interop::test1ReadAndWriteXML(commonDir, "interop17");
    Interop::test1ReadAndWriteXML(commonDir, "interop18");
    Interop::test1ReadAndWriteXML(commonDir, "interop19");
    Interop::test1ReadAndWriteXML(commonDir, "interop20");
    Interop::test1ReadAndWriteXML(commonDir, "interop21");
    Interop::test1ReadAndWriteXML(commonDir, "interop22");
    Interop::test1ReadAndWriteXML(commonDir, "interop23");
    Interop::test1ReadAndWriteXML(commonDir, "interop24");
    Interop::test1ReadAndWriteXML(commonDir, "interop25");
    Interop::test1ReadAndWriteXML(commonDir, "interop26");
    Interop::test1ReadAndWriteXML(commonDir, "interop27");
    Interop::test1ReadAndWriteXML(commonDir, "interop28");
    Interop::test1ReadAndWriteXML(commonDir, "interop29");
    Interop::test1ReadAndWriteXML(commonDir, "interop30");
    Interop::test1ReadAndWriteXML(commonDir, "interop31");
    Interop::test1ReadAndWriteXML(commonDir, "interop32");
    Interop::test1ReadAndWriteXML(commonDir, "interop33");
    Interop::test1ReadAndWriteXML(commonDir, "interop34");
    Interop::test1ReadAndWriteXML(commonDir, "interop35");
    Interop::test1ReadAndWriteXML(commonDir, "interop36");
    Interop::test1ReadAndWriteXML(commonDir, "interop37");
    Interop::test1ReadAndWriteXML(commonDir, "interop38");
    Interop::test1ReadAndWriteXML(commonDir, "interop39");
    Interop::test1ReadAndWriteXML(commonDir, "interop40");
    Interop::test1ReadAndWriteXML(commonDir, "interop41");
    Interop::test1ReadAndWriteXML(commonDir, "interop42");
    Interop::test1ReadAndWriteXML(commonDir, "interop43");
    Interop::test1ReadAndWriteXML(commonDir, "interop44");
    Interop::test1ReadAndWriteXML(commonDir, "interop45");
    Interop::test1ReadAndWriteXML(commonDir, "interop46");
    Interop::test1ReadAndWriteXML(commonDir, "interop47");
    Interop::test1ReadAndWriteXML(commonDir, "interop50");

	Interop::test4ReadAndWriteXSD(commonDir, "interop01");

    return 0;
}
