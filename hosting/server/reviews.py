#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

# App reviews collection implementation
from util import *
from atomutil import *
from sys import debug

# Convert a particular user id to a reviews id
def reviewsid(user):
    return ("reviews", user.get(()), "user.reviews")

# Get user reviews from the cache
def getreviews(id, cache):
    debug('reviews.py::getreviews::id', id)
    val = cache.get(id)
    if isNull(val):
        return ()
    reviews = cdddr(car(val))
    if not isNull(reviews) and isList(car(cadr(car(reviews)))):
        # Expand list of entries
        ereviews = tuple(map(lambda e: cons("'entry", e), cadr(car(reviews))))
        debug('reviews.py::getreviews::ereviews', ereviews)
        return ereviews

    debug('reviews.py::getreviews::reviews', reviews)
    return reviews

# Get a review from a user's reviews
def getreview(id, reviews):
    if isNull(reviews):
        return None
    if car(id) == entryid(reviews):
        return (car(reviews),)
    return getreview(id, cdr(reviews))

# Get reviews from the user's reviews
def get(id, user, cache, apps, ratings):
    debug('reviews.py::get::id', id)
    if isNull(id):
        reviews = ((("'feed", ("'title", "Your Reviews"), ("'id", user.get(()))) + getreviews(reviewsid(user), cache)),)
        debug('reviews.py::get::reviews', reviews)
        return reviews

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('reviews.py::get', 'app not found', id)
        return False

    # Get the review
    review = getreview(id, getreviews(reviewsid(user), cache))
    if isNull(review):
        debug('reviews.py::get', 'review not found', id)

        # Return a default empty review
        return mkentry(car(id), car(id), user.get(()), now(), ())
        
    debug('reviews.py::get::review', review)
    return review

# Patch an app ratings
def patchratings(id, user, ratings, oreview, nreview):
    patch = ("'patch", ("'old", "0" if isNull(oreview) else cadr(content(oreview))), ("'new", "0" if isNull(nreview) else cadr(content(nreview))))
    patchentry = mkentry(car(id), car(id), user.get(()), now(), patch);
    debug('reviews.py::patchratings::patchentry', patchentry)
    return ratings.patch(id, patchentry)

# Put reviews into the cache
def putreviews(id, reviews, cache):
    debug('reviews.py::putreviews::id', id)
    debug('reviews.py::putreviews::reviews', reviews)
    val = ((("'feed", ("'title", "Your Reviews"), ("'id", cadr(id))) + reviews),)
    return cache.put(id, val)

# Put a review into a user's reviews
def putreview(id, review, reviews):
    if isNull(reviews):
        return review
    if car(id) == entryid(reviews):
        return cons(car(review), cdr(reviews))
    return cons(car(reviews), putreview(id, review, cdr(reviews)))

# Put a review into the user's reviews
def put(id, review, user, cache, apps, ratings):
    debug('reviews.py::put::id', id)
    debug('reviews.py::put::review', review)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('reviews.py::put', 'app not found', id)
        return False

    reviewentry = mkentry(title(review), car(id), user.get(()), now(), content(review))
    debug('reviews.py::put::reviewentry', reviewentry)

    # Get old review
    reviews = getreviews(reviewsid(user), cache)
    oreview = getreview(id, reviews)

    # Update the user's reviews record
    nreviews = putreview(id, reviewentry, reviews)
    putreviews(reviewsid(user), nreviews, cache)

    # Update the app's ratings
    return patchratings(id, user, ratings, oreview, review)

# Delete a review from a reviews record
def deletereview(id, reviews):
    if isNull(reviews):
        return ()
    if car(id) == entryid(reviews):
        return cdr(reviews)
    return cons(car(reviews), deletereview(id, cdr(reviews)))

# Delete reviews from the user's reviews record
def delete(id, user, cache, apps, ratings):
    debug('reviews.py::delete::id', id)
    if isNull(id):
        return cache.delete(reviewsid(user))

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('reviews.py::delete', 'app not found', id)
        return False

    # Get the review
    reviews = getreviews(reviewsid(user), cache)
    review = getreview(id, reviews)
    if isNull(review):
        debug('reviews.py::delete', 'review not found', id)
        return False

    # Update the user's reviews record
    nreviews = deletereview(id, reviews)
    putreviews(reviewsid(user), nreviews, cache)

    # Update the app's ratings
    return patchratings(id, user, ratings, review, None)

