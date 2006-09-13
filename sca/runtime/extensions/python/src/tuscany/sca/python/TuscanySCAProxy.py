# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
# 
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
#
# 
# 
# This Python code is used in the TuscanySCA package and
# provides the proxy objects to a Python SCA component or SCA 
# client.  
# 
# This is done by overloading the __getattr__ method which makes 
# calls to myProxy.anyMethod go to __getattr__ which will return
# the invokeFunction defined inside the invoke method

import TuscanySCA

class TuscanySCAProxyClass:

    TuscanySCAProxyName = ''
    TuscanySCAProxyIsReference = 1

    def __init__(self, name, isReference):
        self.TuscanySCAProxyName = name 
        self.TuscanySCAProxyIsReference = isReference
        return

    def invoke(self, operationName):
        
        def invokeFunction(*args,**kwargs):
            return TuscanySCA.invoke(self.TuscanySCAProxyName, self.TuscanySCAProxyIsReference, operationName, args)

        return invokeFunction

    def __getattr__(self, operationName):
        return self.invoke(operationName)

    def __str__(self):
        return  '<TuscanySCAProxy.TuscanySCAProxyClass instance for ' + self.TuscanySCAProxyName + '>'

    def __repr__(self):
        return  '<TuscanySCAProxy.TuscanySCAProxyClass instance for ' + self.TuscanySCAProxyName + '>'

