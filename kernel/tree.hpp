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

namespace tuscany {

/**
 * Delete assocs matching a path of keys in a tree of assocs.
 * The path can be a complete or partial path to an assoc.
 * Requires T to support isList, isAssoc, and cast to list<T>.
 */
template<typename T> inline const list<T> treeDelAssoc(const list<T>& k, const list<T>& l) noexcept {
    if (isNull(k) || isNull(l))
        return l;
    const list<T> lv = l;

    // If list is an assoc and matches, skip it
    if (isAssoc(lv)) {
        if (car<T>(lv) == car(k) && isNull(cdr(k)))
            return list<T>();
    }

    // If list element is not an assoc, lookup children and rest of the list
    const T a = car(lv);
    if (!isAssoc(a)) {
        if (!isList(a))
            return cons<T>(a, treeDelAssoc<T>(k, cdr(lv)));
        const list<T> da = treeDelAssoc<T>(k, a);
        return isNull(da)? treeDelAssoc<T>(k, cdr(lv)) : cons<T>(da, treeDelAssoc<T>(k, cdr(lv)));
    }

    // If we found a match, skip it and lookup children and rest of the list
    if (car<T>(a) == car(k)) {
        if (isNull(cdr(k)))
            return treeDelAssoc<T>(k, cdr(lv));
        return cons<T>(cons<T>(car<T>(a), treeDelAssoc<T>(cdr(k), cdr<T>(a))), treeDelAssoc<T>(k, cdr(lv)));
    }

    // No match, lookup children and rest of the list
    if (isNull(cdr<T>(a)))
        return cons<T>(a, treeDelAssoc<T>(k, cdr(lv)));
    if (!isList(cadr<T>(a)))
        return cons<T>(cons<T>(car<T>(a), cons<T>(cadr<T>(a), treeDelAssoc<T>(k, cddr<T>(a)))), treeDelAssoc<T>(k, cdr(lv)));
    const list<T> da = treeDelAssoc<T>(k, cadr<T>(a));
    if (isNull(da))
        return cons<T>(cons<T>(car<T>(a), treeDelAssoc<T>(k, cddr<T>(a))), treeDelAssoc<T>(k, cdr(lv)));
    return cons<T>(cons<T>(car<T>(a), cons<T>(da, treeDelAssoc<T>(k, cddr<T>(a)))), treeDelAssoc<T>(k, cdr(lv)));
}

/**
 * Substitute assocs matching a path of keys in a tree of assocs.
 * The path can be a complete or partial path to an assoc.
 * Requires T to support isList, isAssoc, and cast to list<T>.
 */
template<typename T> inline const list<T> treeSubstAssoc(const list<T>& k, const list<T>& n, const list<T>& lv) noexcept {
    if (isNull(k) || isNull(lv))
        return lv;

    // If list is an assoc and matches, substitute it
    if (isAssoc(lv)) {
        if (car<T>(lv) == car(k) && isNull(cdr(k)))
            return n;
    }

    // If list element is not an assoc, lookup children and rest of the list
    const T a = car(lv);
    if (!isAssoc(a)) {
        if (!isList(a))
            return cons<T>(a, treeSubstAssoc<T>(k, n, cdr(lv)));
        return cons<T>(treeSubstAssoc<T>(k, n, a), treeSubstAssoc<T>(k, n, cdr(lv)));
    }

    // If we found a match, substitute it and lookup children and rest of the list
    if (car<T>(a) == car(k)) {
        if (isNull(cdr(k)))
            return cons<T>(n, treeSubstAssoc<T>(k, n, cdr(lv)));
        return cons<T>(cons<T>(car<T>(a), treeSubstAssoc<T>(cdr(k), n, cdr<T>(a))), treeSubstAssoc<T>(k, n, cdr(lv)));
    }

    // No match, lookup children and rest of the list
    if (isNull(cdr<T>(a)))
        return cons<T>(a, treeSubstAssoc<T>(k, n, cdr(lv)));
    if (!isList(cadr<T>(a)))
        return cons<T>(cons<T>(car<T>(a), cons<T>(cadr<T>(a), treeSubstAssoc<T>(k, n, cddr<T>(a)))), treeSubstAssoc<T>(k, n, cdr(lv)));
    return cons<T>(cons<T>(car<T>(a), cons<T>(treeSubstAssoc<T>(k, n, cadr<T>(a)), treeSubstAssoc<T>(k, n, cddr<T>(a)))), treeSubstAssoc<T>(k, n, cdr(lv)));
}

/**
 * Select assocs matching a path of keys in a tree of assocs.
 * The path can be a complete or partial path to an assoc.
 * Requires T to support isList, isAssoc, and cast to list<T>.
 */
template<typename T> inline const list<T> treeSelectAssoc(const list<T>& k, const list<T>& lv) noexcept {
    if (isNull(k) || isNull(lv))
        return list<T>();

    // If list is an assoc and matches, select it
    if (isAssoc(lv)) {
        if (car<T>(lv) == car(k) && isNull(cdr(k)))
            return mklist<T>(lv);
    }

    // If list element is not an assoc, lookup children and rest of the list
    const T a = car(lv);
    if (!isAssoc(a)) {
        if (!isList(a))
            return treeSelectAssoc<T>(k, cdr(lv));
        return append<T>(treeSelectAssoc<T>(k, a), treeSelectAssoc<T>(k, cdr(lv)));
    }

    // If we found a match, select it and lookup children and rest of the list
    if (car<T>(a) == car(k)) {
        if (isNull(cdr(k)))
            return cons<T>(a, treeSelectAssoc<T>(k, cdr(lv)));
        return append<T>(treeSelectAssoc<T>(cdr(k), cdr<T>(a)), treeSelectAssoc<T>(k, cdr(lv)));
    }

    // No match, lookup children and rest of the list
    if (isNull(cdr<T>(a)))
        return treeSelectAssoc<T>(k, cdr(lv));
    if (!isList(cadr<T>(a)))
        return append<T>(treeSelectAssoc<T>(k, cddr<T>(a)), treeSelectAssoc<T>(k, cdr(lv)));
    return append<T>(append<T>(treeSelectAssoc<T>(k, cadr<T>(a)), treeSelectAssoc<T>(k, cddr<T>(a))), treeSelectAssoc<T>(k, cdr(lv)));
}

/**
 * Make a rooted binary tree from a leaf and two branches.
 */
template<typename T> inline const list<T> mkrbtree(const T& e, const list<T>& left, const list<T>& right) {
    return mklist<T>(e, left, right);
}

/**
 * Find a leaf with the given key in a rooted binary tree.
 */
template<typename T> inline const list<T> rbtreeAssoc(const T& k, const list<T>& tree) {
    if (isNull(tree))
        return tree;
    if (k == car<T>(car(tree)))
        return car(tree);
    if (k < car<T>(car(tree)))
        return rbtreeAssoc<T>(k, cadr(tree));
    return rbtreeAssoc<T>(k, caddr(tree));
}

/**
 * Construct a new rooted binary tree from a leaf and a tree.
 */
template<typename T> inline const list<T> rbtreeCons(const T& e, const list<T>& tree) {
    if (isNull(tree))
        return mkrbtree(e, list<T>(), list<T>());
    if (e == car(tree))
        return tree;
    if (e < car(tree))
        return mkrbtree<T>(car(tree), rbtreeCons<T>(e, cadr(tree)), caddr(tree));
    return mkrbtree<T>(car(tree), cadr(tree), rbtreeCons<T>(e, caddr(tree)));
}

/**
 * Make a rooted binary tree from an unordered list of leaves.
 */
template<typename T> inline const list<T> mkrbtree(const list<T>& l) {
    if (isNull(l))
        return l;
    return rbtreeCons(car(l), mkrbtree(cdr(l)));
}

/**
 * Convert a rooted binary tree to an ordered list of leaves.
 */
template<typename T> inline const list<T> flatten(const list<T>& tree) {
    if (isNull(tree))
        return tree;
    return append<T>(flatten<T>(cadr(tree)), cons<T>(car(tree), flatten<T>(caddr(tree))));
}

/**
 * Sort a list, using a rooted binary tree.
 */
template<typename T> inline const list<T> sort(const list<T>& l) {
    return flatten(mkrbtree(l));
}

/**
 * Make a balanced rooted binary tree from an ordered list of leaves.
 */
template<typename T> inline const list<T> brbtreeHelper(const list<T>& elements, const size_t n) {
    if (n == 0)
        return cons<T>(list<T>(), elements);
    const size_t leftSize = (n - 1) / 2; {
        const list<T> leftResult = brbtreeHelper<T>(elements, leftSize); {
            const list<T> leftTree = car(leftResult);
            const list<T> nonLeftElements = cdr(leftResult);
            const size_t rightSize = n - (leftSize + 1); {
                const T thisEntry = car(nonLeftElements);
                const list<T> rightResult = brbtreeHelper<T>(cdr(nonLeftElements), rightSize); {
                    const list<T> rightTree = car(rightResult);
                    const list<T> remainingElements = cdr(rightResult); {
                        return cons<T>(mkrbtree<T>(thisEntry, leftTree, rightTree), remainingElements);
                    }
                }
            }
        }
    }
}

template<typename T> inline const list<T> mkbrbtree(const list<T>& elements) {
    return car(brbtreeHelper<T>(elements, length(elements)));
}

}

#endif /* tuscany_tree_hpp */
