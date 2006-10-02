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
# This Python code builds and/or installs the sca and 
# sca_proxy extension modules
# 
# Build/install with the following comands:
# python scaSetup.py build
# python scaSetup.py install
# 
# 

from distutils.core import setup, Extension, os

TUSCANY_SCACPP = os.environ.get("TUSCANY_SCACPP")
print 'Using TUSCANY_SCACPP at ' + TUSCANY_SCACPP

TUSCANY_SDOCPP = os.environ.get("TUSCANY_SDOCPP")
print 'Using TUSCANY_SDOCPP at ' + TUSCANY_SDOCPP

macros = []
if os.name == 'nt':
    macros = [ ('WIN32',None) ]

module1 = Extension('sca',
                    define_macros = macros,
                    sources = ['sca_module.cpp'],
                    include_dirs = [ TUSCANY_SCACPP + '/include',
                                     TUSCANY_SCACPP + '/extensions/python/include',
                                     TUSCANY_SDOCPP + '/include'],
                    libraries = ['tuscany_sca',
                                 'tuscany_sdo',
                                 'tuscany_sca_python'],
                    library_dirs = [ TUSCANY_SCACPP + '/lib',
                                     TUSCANY_SCACPP + '/extensions/python/lib',
                                     TUSCANY_SDOCPP + '/lib'])

setup (name = 'sca',
       version = '1.0',
       description = 'The Tuscany SCA package',
       url='http://incubator.apache.org/tuscany/',
       ext_modules = [module1],
       py_modules = ['sca_proxy'])
