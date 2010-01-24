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

import static java.util.Arrays.*;
import static org.apache.tuscany.IterableUtil.Test.*;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

/**
 * Utility functions to help work efficiently with iterable lists, inspired from Lisp.
 */
public class IterableUtil {

    /**
     * Convert an array or a variable list of arguments to an iterable list.
     */
    public static <T> Iterable<T> list(final Object... a) {
        return new ArrayIterable<T>(a, 0);
    }

    /**
     * Convert an iterable list to a java.util.Collection.
     */
    @SuppressWarnings("unchecked")
    public static <T> Collection<T> collection(final Object l) {
        final Collection<T> c = new ArrayList<T>();
        for(final Object x : (Iterable<?>)l)
            c.add((T)x);
        return c;
    }

    /**
     * Construct a new list from an element and a list.
     */
    public static <T> Iterable<T> cons(final Object car, final Iterable<?> cdr) {
        return new PairIterable<T>(car, cdr);
    }

    /**
     * Return true if a list is nil (empty).
     */
    public static boolean isNil(final Object l) {
        if(l instanceof BasicIterable<?>)
            return ((BasicIterable<?>)l).isNil();
        if(l instanceof Collection<?>)
            return ((Collection<?>)l).isEmpty();
        return !((Iterable<?>)l).iterator().hasNext();
    }

    /**
     * Return the car (first element) of a list.
     */
    @SuppressWarnings("unchecked")
    public static <T> T car(final Object l) {
        if(l instanceof BasicIterable<?>)
            return ((BasicIterable<T>)l).car();
        if(l instanceof List<?>)
            return (T)((List<?>)l).get(0);
        return (T)((Iterable<?>)l).iterator().next();
    }

    /**
     * Return the cdr (rest after the first element) of a list.
     */
    @SuppressWarnings("unchecked")
    public static <T> Iterable<T> cdr(final Object l) {
        if(l instanceof BasicIterable<?>)
            return ((BasicIterable<T>)l).cdr();
        if(l instanceof List<?>)
            return new ListIterable<T>((List<?>)l, 1);
        if(l instanceof Collection<?>)
            return new ArrayIterable<T>(((Collection<?>)l).toArray(), 1);
        return new Iterable<T>() {
            @Override
            public Iterator<T> iterator() {
                final Iterator<T> i = ((Iterable<T>)l).iterator();
                i.next();
                return i;
            }
        };
    }

    /**
     * Return the car of the cdr of a list.
     */
    @SuppressWarnings("unchecked")
    public static <T> T cadr(final Object l) {
        return (T)car(cdr(l));
    }

    /**
     * Return the cdr of the cdr of a list.
     */
    public static <T> Iterable<T> cddr(final Object l) {
        return cdr(cdr(l));
    }

    /**
     * Return the car of the cdr of the cdr of a list.
     */
    @SuppressWarnings("unchecked")
    public static <T> T caddr(final Object l) {
        return (T)car(cddr(l));
    }

    /**
     * Return the first pair matching a key from a list of key value pairs.
     */
    public static <T> Iterable<T> assoc(final Object k, final Object l) {
        if(isNil(l))
            return list();
        if(k.equals(car(car(l))))
            return car(l);
        return assoc(k, cdr(l));
    }

    /**
     * Internal base implementation class for iterable and immutable lists.
     */
    static abstract class BasicIterable<T> extends AbstractList<T> {
        abstract T car();

        abstract Iterable<T> cdr();

        abstract Boolean isNil();

        @Override
        public int size() {
            throw new UnsupportedOperationException();
        }

        @Override
        public T get(final int index) {
            throw new UnsupportedOperationException();
        }
    }

    /**
     * Internal implementation of a list backed by an array.
     */
    static class ArrayIterable<T> extends BasicIterable<T> {
        final Object[] a;
        final int start;

        ArrayIterable(final Object[] a, final int start) {
            this.a = a;
            this.start = start;
        }

        @Override
        Boolean isNil() {
            return this.a.length - this.start == 0;
        }

        @SuppressWarnings("unchecked")
        @Override
        T car() {
            return (T)this.a[this.start];
        }

        @Override
        BasicIterable<T> cdr() {
            return new ArrayIterable<T>(this.a, this.start + 1);
        }

        @Override
        public Iterator<T> iterator() {
            return new Iterator<T>() {
                int i = ArrayIterable.this.start;

                @Override
                public boolean hasNext() {
                    return this.i < ArrayIterable.this.a.length;
                }

                @SuppressWarnings("unchecked")
                @Override
                public T next() {
                    return (T)ArrayIterable.this.a[this.i++];
                }

                @Override
                public void remove() {
                    throw new UnsupportedOperationException();
                }
            };
        }
    }

    /**
     * Internal implementation of a list backed by a java.util.List.
     */
    static class ListIterable<T> extends BasicIterable<T> {
        final List<?> l;
        final int start;

        ListIterable(final List<?> l, final int start) {
            this.l = l;
            this.start = start;
        }

        @Override
        Boolean isNil() {
            return this.l.size() - this.start == 0;
        }

        @SuppressWarnings("unchecked")
        @Override
        T car() {
            return (T)this.l.get(this.start);
        }

        @Override
        BasicIterable<T> cdr() {
            return new ListIterable<T>(this.l, this.start + 1);
        }

        @Override
        public Iterator<T> iterator() {
            return new Iterator<T>() {
                int i = ListIterable.this.start;

                @Override
                public boolean hasNext() {
                    return this.i < ListIterable.this.l.size();
                }

                @SuppressWarnings("unchecked")
                @Override
                public T next() {
                    return (T)ListIterable.this.l.get(this.i++);
                }

                @Override
                public void remove() {
                    throw new UnsupportedOperationException();
                }
            };
        }
    }

    /**
     * Internal implementation of a list backed by an element / iterable pair.
     */
    static class PairIterable<T> extends BasicIterable<T> {
        final Object car;
        final Iterable<?> cdr;

        PairIterable(final Object car, final Iterable<?> cdr) {
            this.car = car;
            this.cdr = cdr;
        }

        @Override
        Boolean isNil() {
            return false;
        }

        @SuppressWarnings("unchecked")
        @Override
        T car() {
            return (T)this.car;
        }

        @SuppressWarnings("unchecked")
        @Override
        Iterable<T> cdr() {
            return (Iterable<T>)this.cdr;
        }

        @Override
        public Iterator<T> iterator() {
            return new Iterator<T>() {
                boolean carIterator = true;
                Iterator<?> cdrIterator = PairIterable.this.cdr.iterator();

                @Override
                public boolean hasNext() {
                    if(this.carIterator)
                        return true;
                    return this.cdrIterator.hasNext();
                }

                @SuppressWarnings("unchecked")
                @Override
                public T next() {
                    if(this.carIterator) {
                        this.carIterator = false;
                        return (T)PairIterable.this.car;
                    }
                    return (T)this.cdrIterator.next();
                }

                @Override
                public void remove() {
                    throw new UnsupportedOperationException();
                }
            };
        }
    }

    /**
     * Test the list functions.
     */
    static class Test {
        Boolean testList() {
            final Iterable<Object> l = list(2, 3, 4);
            assert car(l) == Integer.valueOf(2);
            assert cadr(l) == Integer.valueOf(3);
            assert caddr(l) == Integer.valueOf(4);

            final Iterable<Object> c = cons(0, cons(1, l));
            assert car(c) == Integer.valueOf(0);
            assert cadr(c) == Integer.valueOf(1);
            assert caddr(c) == Integer.valueOf(2);
            assert c.toString().equals("[0, 1, 2, 3, 4]");

            final Iterable<Object> cl = cons(0, cons(1, new ArrayList<Object>(asList(2, 3, 4))));
            assert car(cl) == Integer.valueOf(0);
            assert cadr(cl) == Integer.valueOf(1);
            assert caddr(cl) == Integer.valueOf(2);
            assert cl.toString().equals("[0, 1, 2, 3, 4]");

            final List<Object> jl = new ArrayList<Object>(collection(cl));
            assert jl.size() == 5;
            assert jl.get(0) == Integer.valueOf(0);
            assert jl.get(1) == Integer.valueOf(1);
            assert jl.get(2) == Integer.valueOf(2);

            final Iterable<Object> n = list();
            assert isNil(n);
            assert n.toString().equals("[]");

            final Iterable<Object> cn = cons(0, n);
            assert !isNil(cn);
            assert isNil(cdr(cn));
            assert cn.toString().equals("[0]");

            final Iterable<Object> al = new ArrayList<Object>(Arrays.asList(1, 2, 3));
            assert car(al) == Integer.valueOf(1);
            assert cadr(al) == Integer.valueOf(2);
            assert caddr(al) == Integer.valueOf(3);
            return true;
        }
    }

    public static void main(final String[] args) {
        System.out.println("Testing...");
        
        Test.class.getClassLoader().setDefaultAssertionStatus(true);
        new Test().testList();
        
        System.out.println("OK");
    }

}
