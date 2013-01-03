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
            return list(list("'feed", list("'title", "Sample Feed"), list("'id", "123456789"), list("'entry", list(
                    list(list("'title", "Item"), list("'id", "111"),
                        list("'content", list("'item", list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99)))),
                    list(list("'title", "Item"), list("'id", "222"),
                        list("'content", list("'item", list("'name", "Orange"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 3.55)))),
                    list(list("'title", "Item"), list("'id", "333"),
                        list("'content", list("'item", list("'name", "Pear"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 1.55))))))));

        final Iterable<?> content = list("'content", list("'item", list("'name", "Apple"), list("'currencyCode", "USD"), list("'currencySymbol", "$"), list("'price", 2.99)));
        return list(list("'entry", list("'title", "Item"), list("'id", car(id)), content));
    }

    public Iterable<String> post(final Iterable<String> collection, final Iterable<?> item) {
        return list("123456789");
    }

    public Boolean put(final Iterable<String> id, final Iterable<?> item) {
        return true;
    }

    public Boolean patch(final Iterable<String> id, final Iterable<?> item) {
        return true;
    }

    public Boolean delete(final Iterable<String> id) {
        return true;
    }
}
