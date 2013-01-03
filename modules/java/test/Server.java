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

package test;

public interface Server {

    String echo(String x);
    
    Iterable<?> get(Iterable<String> id);
    
    Iterable<String> post(Iterable<String> collection, Iterable<?> item);
    
    Boolean put(Iterable<String> id, Iterable<?> item);
    
    Boolean patch(Iterable<String> id, Iterable<?> item);
    
    Boolean delete(Iterable<String> id);
    
}
