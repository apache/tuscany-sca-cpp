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

import static org.apache.tuscany.IterableUtil.*;

public class ServerImpl {
    
    public String echo(String x) {
        return x;
    }
    
    public Iterable<?> getall() {
        return list("Sample Feed", "123456789",
                list("Item", "111", list(list("'javaClass", "services.Item"), list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99))),
                list("Item", "222", list(list("'javaClass", "services.Item"), list("'name", "Orange"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 3.55))),
                list("Item", "333", list(list("'javaClass", "services.Item"), list("'name", "Pear"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 1.55))));
    }
    
    public Iterable<?> get(String id) {
        Iterable<?> entry = list(list("'javaClass", "services.Item"), list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99));
        return list("Item", id, entry);
    }

    public String post(Iterable<?> item) {
        return "123456789";
    }
    
    public Boolean put(String id, Iterable<?> entry) {
        return true;
    }

    public Boolean deleteall() {
        return true;
    }

    public Boolean delete(String id) {
        return true;
    }
}
