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

package org.apache.tuscany;

/**
 * Interface used to represent SCA component references providing both REST
 * access to a resource and function application.
 */
public interface Service {
    
    /**
     * Post a new item to a collection of items.
     */
    Iterable<String> post(Iterable<String> collection, Iterable<?> item);
    
    /**
     * Return an item.
     */
    Iterable<?> get(Iterable<String> id);
    
    /**
     * Update an item.
     */
    boolean put(Iterable<String> id, Iterable<?> item);
    
    /**
     * Delete an item.
     */
    boolean delete(Iterable<String> id);
    
    /**
     * Evaluate an expression.
     */
    <T> T eval(Object... params);
    
}
