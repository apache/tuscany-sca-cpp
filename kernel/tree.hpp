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

#ifndef tuscany_tree_hpp
#define tuscany_tree_hpp

/**
 * Functions to work with trees.
 */

#include "stream.hpp"
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "monad.hpp"
#include "value.hpp"

namespace tuscany {

/**
 * Make a tree from a leaf and two branches.
 */
template<typename T> const list<T> mktree(const T& e, const list<T>& left, const list<T>& right) {
    return mklist<T>(e, left, right);
}

/**
 * Find a leaf with the given key in a tree.
 */
template<typename T> const list<T> assoctree(const T& k, const list<T>& tree) {
    if (isNil(tree))
        return tree;
    if (k == car<T>(car(tree)))
        return car(tree);
    if (k < car<T>(car(tree)))
        return assoctree<T>(k, cadr(tree));
    return assoctree<T>(k, caddr(tree));
}

/**
 * Construct a new tree from a leaf and a tree.
 */
template<typename T> const list<T> constree(const T& e, const list<T>& tree) {
    if (isNil(tree))
        return mktree(e, list<T>(), list<T>());
    if (e == car(tree))
        return tree;
    if (e < car(tree))
        return mktree<T>(car(tree), constree<T>(e, cadr(tree)), caddr(tree));
    return mktree<T>(car(tree), cadr(tree), constree<T>(e, caddr(tree)));
}

/**
 * Make a tree from an unordered list of leaves.
 */
template<typename T> const list<T> mktree(const list<T>& l) {
    if (isNil(l))
        return l;
    return constree(car(l), mktree(cdr(l)));
}

/**
 * Convert a tree to an ordered list of leaves.
 */
template<typename T> const list<T> flatten(const list<T>& tree) {
    if (isNil(tree))
        return tree;
    return append<T>(flatten<T>(cadr(tree)), cons<T>(car(tree), flatten<T>(caddr(tree))));
}

/**
 * Sort a list.
 */
template<typename T> const list<T> sort(const list<T>& l) {
    return flatten(mktree(l));
}

/**
 * Make a balanced tree from an ordered list of leaves.
 */
template<typename T> const list<T> btreeHelper(const list<T>& elements, const size_t n) {
    if (n == 0)
        return cons<T>(list<T>(), elements);
    const size_t leftSize = (n - 1) / 2; {
        const list<T> leftResult = btreeHelper<T>(elements, leftSize); {
            const list<T> leftTree = car(leftResult);
            const list<T> nonLeftElements = cdr(leftResult);
            const size_t rightSize = n - (leftSize + 1); {
                const T thisEntry = car(nonLeftElements);
                const list<T> rightResult = btreeHelper<T>(cdr(nonLeftElements), rightSize); {
                    const list<T> rightTree = car(rightResult);
                    const list<T> remainingElements = cdr(rightResult); {
                        return cons<T>(mktree<T>(thisEntry, leftTree, rightTree), remainingElements);
                    }
                }
            }
        }
    }
}

template<typename T> const list<T> mkbtree(const list<T>& elements) {
    return car(btreeHelper<T>(elements, length(elements)));
}

}

#endif /* tuscany_tree_hpp */
