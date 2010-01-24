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

import org.apache.tuscany.Service;
import java.lang.System;
import java.util.UUID;
import static org.apache.tuscany.IterableUtil.*;

/**
 * Shopping cart component implementation.
 */
public class ShoppingCartImpl {
    
    static String cartId = "1234";
    
    /**
     * Get the shopping cart from the cache. Return an empty
     * cart if not found.
     */
    public Iterable<?> getcart(String id, Service cache) {
        Iterable<?> cart = cache.get(id);
        if (cart == null)
            return list();
        return cart;
    }

    /**
     * Returns a UUID.
     */
    String uuid() {
        return UUID.randomUUID().toString();
    }

    /**
     * Post a new item to the cart. Create a new cart if necessary.
     */
    public String post(Iterable<?> item, Service cache) {
        String id = uuid();
        Iterable<?> newItem = list(car(item), id, caddr(item));
        Iterable<?> cart = cons(newItem, getcart(cartId, cache));
        cache.put(cartId, cart);
        return id;
    }

    /**
     * Return the contents of the cart.
     */
    public Iterable<?> getall(Service cache) {
        return cons("Your Cart", cons(cartId, getcart(cartId, cache)));
    }

    /**
     * Find an item in the cart.
     */
    public Iterable<?> find(String id, Iterable<?> cart) {
        if (isNil(cart))
            return cons("Item", list("0", list()));
        if (id.equals(cadr(car(cart))))
            return car(cart);
        return find(id, cdr(cart));
    }

    public Iterable<?> get(String id, Service cache) {
        return find(id, getcart(cartId, cache));
    }

    /**
     * Delete the whole cart.
     */
    public Boolean deleteall(Service cache) {
        return cache.delete(cartId);
    }

    /**
     * Delete an item from the cart.
     */
    public Boolean delete(String id, Service cache) {
        return true;
    }

    /**
     * Return the price of an item.
     */
    Double price(Iterable<?> item) {
        return Double.valueOf((String)cadr(assoc("'price", caddr(item))));
    }

    /**
     * Sum the prices of a list of items.
     */
    Double sum(Iterable<?> items) {
        if (isNil(items))
            return 0.0;
        return price((Iterable<?>)car(items)) + sum(cdr(items));
    }

    /**
     * Return the total price of the items in the cart.
     */
    public Double gettotal(Service cache) {
        Iterable<?> cart = getcart(cartId, cache);
        return sum(cart);
    }

    /**
     * TODO remove this JSON-RPC specific function.
     */
    public Iterable<?> listMethods(final CurrencyConverter converter) {
        return list("Service.gettotal");
    }
    
}
