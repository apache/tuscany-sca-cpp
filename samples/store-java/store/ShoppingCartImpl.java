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
import static org.apache.tuscany.UUIDUtil.*;

import org.apache.tuscany.Service;

/**
 * Shopping cart component implementation.
 */
public class ShoppingCartImpl {

    static String cartId = "1234";

    /**
     * Get the shopping cart from the cache. Return an empty cart if not found.
     */
    public Iterable<?> getcart(final String id, final Service cache) {
        final Iterable<String> iid = list(id);
        final Iterable<?> cart = cache.get(iid);
        if(cart == null)
            return list();
        return cart;
    }

    /**
     * Post a new item to the cart. Create a new cart if necessary.
     */
    public Iterable<String> post(final Iterable<String> collection, final Iterable<?> item, final Service cache) {
        final String id = uuid();
        final Iterable<?> newItem = list(car(item), id, caddr(item));
        final Iterable<?> cart = cons(newItem, this.getcart(cartId, cache));
        final Iterable<String> iid = list(cartId);
        cache.put(iid, cart);
        return list(id);
    }

    /**
     * Find an item in the cart.
     */
    Iterable<?> find(final String id, final Iterable<?> cart) {
        if(isNil(cart))
            return cons("Item", list("0", list()));
        if(id.equals(cadr(car(cart))))
            return car(cart);
        return this.find(id, cdr(cart));
    }

    /**
     * Return items from the cart.
     */
    public Iterable<?> get(final Iterable<String> id, final Service cache) {
        if(isNil(id))
            return cons("Your Cart", cons(cartId, this.getcart(cartId, cache)));
        return this.find((String)car(id), this.getcart(cartId, cache));
    }

    /**
     * Delete items from the cart.
     */
    public Boolean delete(final Iterable<String> id, final Service cache) {
        if(isNil(id)) {
            final Iterable<String> iid = list(cartId);
            return cache.delete(iid);
        }
        return true;
    }

    /**
     * Return the price of an item.
     */
    Double price(final Iterable<?> item) {
        return Double.valueOf((String)cadr(assoc("'price", caddr(item))));
    }

    /**
     * Sum the prices of a list of items.
     */
    Double sum(final Iterable<?> items) {
        if(isNil(items))
            return 0.0;
        return this.price((Iterable<?>)car(items)) + this.sum(cdr(items));
    }

    /**
     * Return the total price of the items in the cart.
     */
    public Double gettotal(final Service cache) {
        final Iterable<?> cart = this.getcart(cartId, cache);
        return this.sum(cart);
    }

    /**
     * TODO remove this JSON-RPC specific function.
     */
    public Iterable<?> listMethods(final Service cache) {
        return list("Service.gettotal");
    }

}
