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

package store;

import static org.apache.tuscany.IterableUtil.list;

/**
 * Catalog component implementation.
 */
public class FruitsCatalogImpl {
    
    /**
     * Returns the catalog.
     */
    public Iterable<?> get(final CurrencyConverter converter) {
        class Converter {
            Double convert(Double price) {
                return converter.convert("USD", "USD", price);
            }
        };
        Converter c = new Converter();
        
        String code = "USD";
        String symbol = converter.symbol(code);
        
        return list(
                list(list("'javaClass", "services.Item"), list("'name", "Apple"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(2.99))),
                list(list("'javaClass", "services.Item"), list("'name", "Orange"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(3.55))),
                list(list("'javaClass", "services.Item"), list("'name", "Pear"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(1.55)))
              );
    }

    /**
     * TODO remove this JSON-RPC specific function.
     */
    public Iterable<?> listMethods(final CurrencyConverter converter) {
        return list("Service.get");
    }
    
}
