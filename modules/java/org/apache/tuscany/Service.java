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
     * Post a new item to a resource.
     */
    String post(Iterable<?> item);
    
    /**
     * Return an item.
     */
    Iterable<?> get(String id);
    
    /**
     * Return all items in the resource.
     */
    Iterable<?> getall();
    
    /**
     * Update am item.
     */
    boolean put(String id, Iterable<?> item);
    
    /**
     * Delete an item.
     */
    boolean delete(String id);
    
    /**
     * Delete all items in the resource.
     */
    boolean deleteall();
    
    /**
     * Apply a function.
     */
    <T> T apply(Object... params);
    
}
