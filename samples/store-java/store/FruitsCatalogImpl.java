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

import static org.apache.tuscany.IterableUtil.*;

import org.apache.tuscany.Service;

/**
 * Catalog component implementation.
 */
public class FruitsCatalogImpl {

    /**
     * Returns the catalog.
     */
    public Iterable<?> getcatalog(final CurrencyConverter converter, final Service currencyCode) {
        final String code = currencyCode.eval();

        class Converter {
            Double convert(final Double price) {
                return converter.convert(code, "USD", price);
            }
        }

        final Converter c = new Converter();
        final String symbol = converter.symbol(code);

        return list(list(list("'name", "Apple"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(2.99))),
                list(list("'name", "Orange"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(3.55))),
                list(list("'name", "Pear"), list("'currencyCode", code), list("'currencySymbol", symbol), list("'price", c.convert(1.55))));
    }

}
