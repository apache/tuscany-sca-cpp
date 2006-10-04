@echo off 
@REM  Licensed to the Apache Software Foundation (ASF) under one
@REM  or more contributor license agreements.  See the NOTICE file
@REM  distributed with this work for additional information
@REM  regarding copyright ownership.  The ASF licenses this file
@REM  to you under the Apache License, Version 2.0 (the
@REM  "License"); you may not use this file except in compliance
@REM  with the License.  You may obtain a copy of the License at
@REM  
@REM    http://www.apache.org/licenses/LICENSE-2.0
@REM    
@REM  Unless required by applicable law or agreed to in writing,
@REM  software distributed under the License is distributed on an
@REM  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
@REM  KIND, either express or implied.  See the License for the
@REM  specific language governing permissions and limitations
@REM  under the License.

if not exist deploy mkdir deploy
if not exist deploy\packages mkdir deploy\packages
if not exist deploy\packages\bigbank.account mkdir deploy\packages\bigbank.account
if not exist deploy\configuration mkdir deploy\configuration
if not exist deploy\configuration\bigbank.account mkdir deploy\configuration\bigbank.accountmanagement
if not exist deploy\bin mkdir deploy\bin

copy bigbank.account\Debug\Account.dll deploy\packages\bigbank.account
copy bigbank.account\Debug\Account.pdb deploy\packages\bigbank.account
copy bigbank.account\AccountServiceImpl.h deploy\packages\bigbank.account
copy bigbank.account\AccountService.h deploy\packages\bigbank.account
copy bigbank.account\AccountDataServiceImpl.h deploy\packages\bigbank.account
copy bigbank.account\AccountDataService.h deploy\packages\bigbank.account
copy bigbank.account\StockQuoteService.h deploy\packages\bigbank.account
copy bigbank.account\StockQuoteServiceImpl.h deploy\packages\bigbank.account
copy bigbank.account\StockQuoteWebService.h deploy\packages\bigbank.account
copy bigbank.account\bigbank.account.composite deploy\packages\bigbank.account
copy bigbank.account\AccountServiceImpl.componentType deploy\packages\bigbank.account
copy bigbank.account\AccountDataServiceImpl.componentType deploy\packages\bigbank.account
copy bigbank.account\StockQuoteServiceImpl.componentType deploy\packages\bigbank.account
copy bigbank.account\AccountService.wsdl deploy\packages\bigbank.account
copy bigbank.account\StockQuoteService.wsdl deploy\packages\bigbank.account
copy bigbank.account\StockQuoteTypes.xsd deploy\packages\bigbank.account

copy bigbank.accountmanagement\bigbank.accountmanagement.composite deploy\configuration\bigbank.accountmanagement

copy bigbank.client\Debug\account_client.exe deploy\bin
copy bigbank.client\Debug\account_client.pdb deploy\bin
copy bigbank.client\runclient.cmd deploy\bin

copy bigbank.wsclient\Debug\account_wsclient.exe deploy\bin
copy bigbank.wsclient\Debug\account_wsclient.pdb deploy\bin
copy bigbank.wsclient\runwsclient.cmd deploy\bin


