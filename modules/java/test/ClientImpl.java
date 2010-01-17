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

public class ClientImpl {

    public String echo(String x, Server server) {
        return server.echo(x);
    }
    
    public Object[] getall(Server server) {
        return server.getall();
    }
    
    public Object[] get(String id, Server server) {
        return server.get(id);
    }
    
    public String post(Object[] item, Server server) {
        return server.post(item);
    }
    
    public Boolean put(String id, Object[] item, Server server) {
        return server.put(id, item);
    }
    
    public Boolean deleteall(Server server) {
        return server.deleteall();
    }
    
    public Boolean delete(String id, Server server) {
        return server.delete(id);
    }
    
}
