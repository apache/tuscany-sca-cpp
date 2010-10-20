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

    public String echo(final String x) {
        return x;
    }

    public Iterable<?> get(final Iterable<String> id) {
        if (isNil(id))
            return list("Sample Feed", "123456789",
                    list("Item", "111", list(list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99))),
                    list("Item", "222", list(list("'name", "Orange"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 3.55))),
                    list("Item", "333", list(list("'name", "Pear"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 1.55))));
        final Iterable<?> entry = list(list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99));
        return list("Item", car(id), entry);
    }

    public Iterable<String> post(final Iterable<String> collection, final Iterable<?> item) {
        return list("123456789");
    }

    public Boolean put(final Iterable<String> id, final Iterable<?> item) {
        return true;
    }

    public Boolean delete(final Iterable<String> id) {
        return true;
    }
}
