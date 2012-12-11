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

#ifndef tuscany_hash_hpp
#define tuscany_hash_hpp

/**
 * Fast hash functions.
 */

#include <apr_hash.h>
#include <apr_memcache.h>

namespace tuscany
{

/**
 * Apache apr-util CRC32 hash function.
 *
 * See srclib/apr-util/memcache/apr_memcache.c from the Apache HTTPD
 * source tree. Reproducing the comments from apr_memcache.c here:
 *
 * The crc32 functions and data were originally written by Spencer
 * Garrett <srg@quick.com> and were gleaned from the PostgreSQL source
 * tree at contrib/ltree/crc32.[ch] and from FreeBSD at
 * src/usr.bin/cksum/crc32.c.
 */
inline const unsigned int crc32hash(const char* const data, const size_t len) {
    return (unsigned int)apr_memcache_hash_default(NULL, data, len);
}

/**
 * Apache apr tables default hash function.
 *
 * See srclib/apr/tables/apr_hash.c from the Apache HTTPD source tree.
 * Reproducing the comments from apr_hash.c here:
 *
 * This is the popular `times 33' hash algorithm which is used by
 * perl and also appears in Berkeley DB. This is one of the best
 * known hash functions for strings because it is both computed
 * very fast and distributes very well.
 *
 * The originator may be Dan Bernstein but the code in Berkeley DB
 * cites Chris Torek as the source. The best citation I have found
 * is "Chris Torek, Hash function for text in C, Usenet message
 * <27038@mimsy.umd.edu> in comp.lang.c , October, 1990." in Rich
 * Salz's USENIX 1992 paper about INN which can be found at
 * <http://citeseer.nj.nec.com/salz92internetnews.html>.
 *
 * The magic of number 33, i.e. why it works better than many other
 * constants, prime or not, has never been adequately explained by
 * anyone. So I try an explanation: if one experimentally tests all
 * multipliers between 1 and 256 (as I did while writing a low-level
 * data structure library some time ago) one detects that even
 * numbers are not useable at all. The remaining 128 odd numbers
 * (except for the number 1) work more or less all equally well.
 * They all distribute in an acceptable way and this way fill a hash
 * table with an average percent of approx. 86%.
 *
 * If one compares the chi^2 values of the variants (see
 * Bob Jenkins ``Hashing Frequently Asked Questions'' at
 * http://burtleburtle.net/bob/hash/hashfaq.html for a description
 * of chi^2), the number 33 not even has the best value. But the
 * number 33 and a few other equally good numbers like 17, 31, 63,
 * 127 and 129 have nevertheless a great advantage to the remaining
 * numbers in the large set of possible multipliers: their multiply
 * operation can be replaced by a faster operation based on just one
 * shift plus either a single addition or subtraction operation. And
 * because a hash function has to both distribute good _and_ has to
 * be very fast to compute, those few numbers should be preferred.
 *
 *                  -- Ralf S. Engelschall <rse@engelschall.com>
 */
inline const unsigned int times33hash(const char* const data, const size_t len) {
    apr_ssize_t l = len;
    return apr_hashfunc_default(data, &l);
}

/**
 * A very fast, non-cryptographic hash suitable for general hash-based
 * lookup. See http://murmurhash.googlepages.com/ for more details.
 *
 * Original code by Austin Appleby, released to the public domain and under
 * the MIT license.
 *
 * Compiles down to ~52 instructions on x86.
 * Passes chi^2 tests for practically all keysets & bucket sizes.
 * Excellent avalanche behavior. Maximum bias is under 0.5%.
 * Passes Bob Jenkin's frog.c torture-test. No collisions possible for 4 byte
 * keys, no small 1 to 7 bit differentials.
 */
inline const unsigned int murmurhash(const char* const key, const size_t klen) {
    unsigned int len = (unsigned int)klen;
    const unsigned int seed = 0;

    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value
    unsigned int h = seed ^ len;

    // Mix 4 bytes at a time into the hash
    const unsigned char* data = (const unsigned char*)key;
    while(len >= 4) {
        unsigned int k = *(unsigned int*)(void*)data;
        k *= m; 
        k ^= k >> r; 
        k *= m; 
        h *= m; 
        h ^= k;
        data += 4;
        len -= 4;
    }
    
    // Handle the last few bytes of the input array
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
            h *= m;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
} 

/**
 * An endian and alignment neutral, but half the speed, version of
 * the murmur hash.
 */
inline const unsigned int portablemurmurhash(const char* const key, const size_t klen) {
    unsigned int len = (unsigned int)klen;
    const unsigned int seed = 0;

    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value
    unsigned int h = seed ^ len;

    // Mix 4 bytes at a time into the hash
    const unsigned char* data = (const unsigned char *)key;
    while(len >= 4) {
        unsigned int k;
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;
        k *= m; 
        k ^= k >> r; 
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    
    // Handle the last few bytes of the input array
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
            h *= m;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
} 

inline const unsigned int hashselect(const unsigned int hash, const unsigned int max) {
    return hash % max;
}

}
#endif /* tuscany_hash_hpp */
