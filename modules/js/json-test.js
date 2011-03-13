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

/**
 * Test JSON data encoding functions.
 */
function testJSON() {
    var ad = mklist(mklist(attribute, "'city", "san francisco"), mklist(attribute, "'state", "ca"));
    var ac = mklist(mklist(element, "'id", "1234"), mklist(attribute, "'balance", 1000));
    var cr = mklist(mklist(attribute, "'name", "jdoe"), cons(element, cons("'address", ad)), cons(element, cons("'account", ac)));
    var c = mklist(cons(element, cons("'customer", cr)));
    var s = json.writeJSON(c);
    assert(car(s) == "{\"customer\": {\"@name\": \"jdoe\", \"address\": {\"@city\": \"san francisco\", \"@state\": \"ca\"}, \"account\": {\"id\": \"1234\", \"@balance\": 1000}}}");

    var phones = mklist("408-1234", "650-1234");
    var l = mklist(mklist(element, "'phones", phones), mklist(element, "'lastName", "test\ttab"), mklist(attribute, "'firstName", "test1"));
    var s2 = json.writeJSON(l);
    assert(car(s2) == "{\"phones\": [\"408-1234\", \"650-1234\"], \"lastName\": \"test\\ttab\", \"@firstName\": \"test1\"}");

    var r = json.readJSON(s2);
    var w = json.writeJSON(r);
    assert(car(w) == "{\"@firstName\": \"test1\", \"lastName\": \"test\\ttab\", \"phones\": [\"408-1234\", \"650-1234\"]}");

    var l4 = mklist(mklist("'ns1:echoString", mklist("'@xmlns:ns1", "http://ws.apache.org/axis2/services/echo"), mklist("'text", "Hello World!")));
    var s4 = json.writeJSON(valuesToElements(l4));
    assert(car(s4) == "{\"ns1:echoString\": {\"@xmlns:ns1\": \"http://ws.apache.org/axis2/services/echo\", \"text\": \"Hello World!\"}}");

    var r5 = elementsToValues(json.readJSON(s4));
    var s5 = json.writeJSON(valuesToElements(r5));
    assert(car(s5) == "{\"ns1:echoString\": {\"text\": \"Hello World!\", \"@xmlns:ns1\": \"http://ws.apache.org/axis2/services/echo\"}}");
    return true;
}

testJSON();

