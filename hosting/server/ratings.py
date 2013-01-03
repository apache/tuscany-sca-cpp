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

# App ratings collection implementation
from util import *
from atomutil import *
from sys import debug

# Convert an app id to an app ratings id
def ratingsid(id):
    return ("ratings", car(id), "app.ratings")

# Put an app ratings into the ratings db
def put(id, ratings, user, cache, db, apps):
    debug('ratings.py::put::id', id)
    debug('ratings.py::put::ratings', ratings)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('ratings.py::put', 'app not found', id)
        return False

    # Check user
    if user.get(()) != 'admin':
        debug('ratings.py::put', 'not admin', user.get(()))
        return False

    # Update app ratings in the ratings db
    ratingsentry = mkentry(title(app), car(id), author(app), now(), content(ratings))
    debug('ratings.py::put::ratingsentry', ratingsentry)
    return cache.put(ratingsid(id), ratingsentry)

# Patch an app ratings in the ratings db
def patch(id, ratings, user, cache, db, apps):
    debug('ratings.py::patch::id', id)
    debug('ratings.py::patch::ratings', ratings)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('ratings.py::patch', 'app not found', id)
        return False

    # Get old and new ratings
    orating = cadr(assoc("'old", content(ratings)))
    nrating = cadr(assoc("'new", content(ratings)))

    # Configure patch script
    script = """
        (define author "{0}")
        (define updated "{1}")
        (define orating {2})
        (define nrating {3})

        (define (patch id e)
            (define (rating x e)
                (define a (tree-select-assoc (list x) e))
                (if (null? a) (list x "0") (car a))
            )

            (define cratings (list (rating 'rating1 e) (rating 'rating2 e) (rating 'rating3 e) (rating 'rating4 e)))

            (define (calcrating v i)
                (define nv (+ v (if (= i orating) (- 1) (if (= i nrating) 1 0))))
                (if (< nv 0) 0 nv)
            )

            (define (calcratings r i)
                (if (null? r)
                    r
                    (cons (list (car (car r)) (calcrating (cadr (car r)) i)) (calcratings (cdr r) (+ 1 i)))
                )
            )

            (define nratings (calcratings cratings 1))

            (define neg (+ 1 (+ (* (cadr (assoc 'rating1 nratings)) 2) (cadr (assoc 'rating2 nratings)))))
            (define pos (+ 1 (+ (* (cadr (assoc 'rating4 nratings)) 2) (cadr (assoc 'rating3 nratings)))))
            (define arating (* 4 (/ (- (/ (+ pos 1.9208) (+ pos neg)) (/ (* 1.96 (sqrt (+ (/ (* pos neg) (+ pos neg)) 0.9604))) (+ pos neg))) (+ 1 (/ 3.8416 (+ pos neg))))))

            (list (list 'entry (list 'title (cadr id)) (list 'id (cadr id)) (list 'author author) (list 'updated updated) (list 'content (cons 'ratings (cons (list 'rating arating) nratings)))))
        )
    """.format(author(app), now(), orating, nrating)

    # Update app ratings in the ratings db
    ratingsentry = mkentry(title(app), car(id), author(app), now(), ("'patch", script))
    debug('ratings.py::put::ratingsentry', ratingsentry)
    return cache.patch(ratingsid(id), ratingsentry)

# Get app ratings from the ratings db
def get(id, user, cache, db, apps):
    debug('ratings.py::get::id', id)

    # Return the top ratings
    if isNull(id):
        topentries = db.get((("'regex", '("ratings" .* "app.ratings")'), ("'rank", "(regexp_matches(value, '(.*\(rating )([^\)]+)(\).*)'))[2]::float"), ("'limit", 25)))
        flatentries = tuple(map(lambda v: car(v), () if isNull(topentries) else topentries))
        def rating(e):
            return cadr(assoc("'rating", assoc("'ratings", assoc("'content", e))))
        sortedentries = tuple(sorted(flatentries, key = rating, reverse = True))
        topratings = ((("'feed", ("'title", "Ratings"), ("'id", 'ratings')) + sortedentries),)
        debug('ratings.py::get::topratings', topratings)
        return topratings

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('ratings.py::get', 'app not found', id)

        # Return default ratings
        return mkentry(car(id), car(id), user.get(()), now(), ())

    # Get the requested ratings
    ratings = cache.get(ratingsid(id))
    if isNull(ratings):
        debug('ratings.py::get', 'ratings not found', id)

        # Return default ratings
        return mkentry(title(app), car(id), author(app), now(), ())

    # Return the ratings
    ratingsentry = mkentry(title(app), car(id), author(app), updated(ratings), content(ratings))
    debug('ratings.py::get::ratings', ratingsentry)
    return ratingsentry

# Delete an app ratings from the ratings db
def delete(id, user, cache, db, apps):
    debug('ratings.py::delete::id', id)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('ratings.py::delete', 'app not found', id)
        return False

    # Check user
    if user.get(()) != 'admin':
        debug('ratings.py::delete', 'not admin', user.get(()))
        return False

    # Delete the composite
    return cache.delete(ratingsid(id))

