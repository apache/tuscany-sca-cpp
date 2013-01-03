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
 * by a selector component, responsible for selecting the target data stores given the
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
 * Return a list of target partitions for a key.
 */
const failable<list<value> > partition(const value& key, const lvvlambda& selector, const list<value>& partitions) {

    // Call the selector component to convert the given key to a list of partitions
    const value p = selector(mklist<value>("get", key, partitions));
    if (isNull(p)) {
        ostringstream os;
        os << "Couldn't get partition: " << key;
        return mkfailure<list<value> >(str(os), -1, false);
    }
    return (list<value>)p;
}


/**
 * Get lists of items from a list of partitions.
 */
const failable<list<value> > getlist(const value& key, const list<value>& partitions) {
    if (isNull(partitions))
        return nilListValue;

    const lvvlambda l = car(partitions);
    const value val = l(mklist<value>("get", key));
    if (isNull(val))
        return getlist(key, cdr(partitions));
    if (!isList(val)) {
        ostringstream os;
        os << "Couldn't get list of entries from partition: " << key;
        return mkfailure<list<value> >(str(os), 500, false);
    }

    const failable<list<value> > cdrval = getlist(key, cdr(partitions));
    if (!hasContent(cdrval))
        return cdrval;

    return append<value>((list<value>)val, content(cdrval));
}

/**
 * Get an item from a partition.
 */
const failable<value> get(const value& key, const lvvlambda& selector, const list<value>& partitions) {

    // Select partition
    const failable<list<value> > fp = partition(key, selector, partitions);
    if (!hasContent(fp))
        return mkfailure<value>(fp);
    const list<value> p = content(fp);

    // Get a single item from the selected partition
    if (length(p) == 1) {
        const lvvlambda l = car(p);
        const value val = l(mklist<value>("get", key));
        if (isNull(val)) {
            ostringstream os;
            os << "Couldn't get entry from partition: " << key;
            return mkfailure<value>(str(os), 404, false);
        }
        return val;
    }

    // Get list of items from the list of selected partitions
    const failable<list<value> > val = getlist(key, p);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return (value)content(val);
}

/**
 * Post an item to a partition.
 */
const failable<value> post(const value& key, const value& val, const lvvlambda& selector, const list<value>& partitions) {
    const value id = append<value>(key, mklist(mkuuid()));

    // Select partition
    const failable<list<value> > p = partition(id, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(p);

    // Put into select partition
    const lvvlambda l = car(content(p));
    l(mklist<value>("post", id, val));

    return id;
}

/**
 * Put an item into a partition.
 */
const failable<value> put(const value& key, const value& val, const lvvlambda& selector, const list<value>& partitions) {

    // Select partition
    const failable<list<value> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(p);

    // Put into selected partition
    const lvvlambda l = car(content(p));
    l(mklist<value>("put", key, val));

    return trueValue;
}

/**
 * Patch an item in a partition.
 */
const failable<value> patch(const value& key, const value& val, const lvvlambda& selector, const list<value>& partitions) {

    // Select partition
    const failable<list<value> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(p);

    // Path item in selected partition
    const lvvlambda l = car(content(p));
    l(mklist<value>("patch", key, val));

    return trueValue;
}

/**
 * Delete an item from a partition.
 */
const failable<value> del(const value& key, const lvvlambda& selector, const list<value>& partitions) {

    // Select partition
    const failable<list<value> > p = partition(key, selector, partitions);
    if (!hasContent(p))
        return mkfailure<value>(p);

    // Delete from selected partition
    const lvvlambda l = car(content(p));
    l(mklist<value>("delete", key));

    return trueValue;
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
    if (func == "patch")
        return tuscany::partitioner::patch(cadr(params), caddr(params), cadddr(params), cddddr(params));
    if (func == "delete")
        return tuscany::partitioner::del(cadr(params), caddr(params), cdddr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
