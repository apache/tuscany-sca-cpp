;  Licensed to the Apache Software Foundation (ASF) under one
;  or more contributor license agreements.  See the NOTICE file
;  distributed with this work for additional information
;  regarding copyright ownership.  The ASF licenses this file
;  to you under the Apache License, Version 2.0 (the
;  "License"); you may not use this file except in compliance
;  with the License.  You may obtain a copy of the License at
;  
;    http://www.apache.org/licenses/LICENSE-2.0
;    
;  Unless required by applicable law or agreed to in writing,
;  software distributed under the License is distributed on an
;  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
;  KIND, either express or implied.  See the License for the
;  specific language governing permissions and limitations
;  under the License.

; OAuth support test case

(define (get id user email nickname fullname firstname lastname realm) (list "text/html" (list
    "<html><body><p>The following info is generated on the server:</p><div>User: " (user) "</div><div>Email: " (email) "</div><div>Nickname: " (nickname) "</div><div>Fullname: " (fullname) "</div><div>Firstname: " (firstname) "</div><div>Lastname: " (lastname) "</div><div>Realm: " (realm) "</div></body></html>")))

(define (getuser user email nickname fullname firstname lastname realm) (user))

(define (getemail user email nickname fullname firstname lastname realm) (email))

(define (getnickname user email nickname fullname firstname lastname realm) (nickname))

(define (getfullname user email nickname fullname firstname lastname realm) (fullname))

(define (getfirstname user email nickname fullname firstname lastname realm) (firstname))

(define (getlastname user email nickname fullname firstname lastname realm) (lastname))

(define (getrealm user email nickname fullname firstname lastname realm) (realm))

