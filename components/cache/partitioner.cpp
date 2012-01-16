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

/* $Rev$ $Date$ */

/**
 * A partitioner component implementation which forwards data access requests to a
 * dynamically selected data store component. The selection is externalized, performed
 * by a selector component, responsible for selecting the target data store given the
 * data access request key and a list of references to available data store components.
 * This pattern can be used for sharding or load balancing for example.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace partitioner {

/**
 * Return the target partition for a key.
 */
const failable<lambda<value(const list<value>&)> > partition(const value& key, const lambda<value(const list<value>&)> selector, const list<value>& partitions) {

    // Call the selector component to convert the given key to a partition number
    const value p = selector(mklist<value>("get", key, partitions));
    if (isNil(p)) {
        ostringstream os;
        os << "Couldn't get partition number: " << key;
        return mkfailure<lambda<value(const list<value>&)> >(str(os), false);
    }
    return (const lambda<value(const list<value>&)>)p;
}

/**
 * Get an item from a partition.
 */
const failable<value> get(const value& key, const lambda<value(const list<value>&)> selector, const list<value>& partitions) {

    // Select partition
    const failable<lambda<value(const list<value>&)> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(reason(p));

    // Get from selected partition
    const value val = content(p)(mklist<value>("get", key));
    if (isNil(val)) {
        ostringstream os;
        os << "Couldn't get entry from partition: " << key;
        return mkfailure<value>(str(os), false);
    }

    return val;
}

/**
 * Post an item to a partition.
 */
const failable<value> post(const value& key, const value& val, const lambda<value(const list<value>&)> selector, const list<value>& partitions) {
    const value id = append<value>(key, mklist(mkuuid()));

    // Select partition
    const failable<lambda<value(const list<value>&)> > p = partition(id, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(reason(p));

    // Put into select partition
    content(p)(mklist<value>("put", id, val));

    return id;
}

/**
 * Put an item into a partition.
 */
const failable<value> put(const value& key, const value& val, const lambda<value(const list<value>&)> selector, const list<value>& partitions) {

    // Select partition
    const failable<lambda<value(const list<value>&)> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(reason(p));

    // Put into selected partition
    content(p)(mklist<value>("put", key, val));

    return value(true);
}

/**
 * Delete an item from a partition.
 */
const failable<value> del(const value& key, const lambda<value(const list<value>&)> selector, const list<value>& partitions) {

    // Select partition
    const failable<lambda<value(const list<value>&)> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(reason(p));

    // Delete from selected partition
    content(p)(mklist<value>("delete", key));

    return value(true);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "get")
        return tuscany::partitioner::get(cadr(params), caddr(params), cdddr(params));
    if (func == "post")
        return tuscany::partitioner::post(cadr(params), caddr(params), cadddr(params), cddddr(params));
    if (func == "put")
        return tuscany::partitioner::put(cadr(params), caddr(params), cadddr(params), cddddr(params));
    if (func == "delete")
        return tuscany::partitioner::del(cadr(params), caddr(params), cdddr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
