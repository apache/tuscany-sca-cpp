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

if WANT_CHAT

INCLUDES = -I${LIBSTROPHE_INCLUDE}

compdir=$(prefix)/components/chat
comp_LTLIBRARIES = libchat-sender.la libchat-listener.la

libchat_sender_la_SOURCES = chat-sender.cpp
libchat_sender_la_LDFLAGS = -L${LIBSTROPHE_LIB} -R${LIBSTROPHE_LIB} -lstrophe -lexpat -lssl -lresolv

libchat_listener_la_SOURCES = chat-listener.cpp
libchat_listener_la_LDFLAGS = -L${LIBSTROPHE_LIB} -R${LIBSTROPHE_LIB} -lstrophe -lexpat -lssl -lresolv

endif
