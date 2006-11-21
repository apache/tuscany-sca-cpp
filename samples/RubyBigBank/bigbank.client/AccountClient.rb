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

require("tuscany_sca_ruby")

accountService = SCA::locateService("AccountServiceComponent")

report = accountService.getAccountReport(1234)

checking = report.root.elements["checking"]
savings = report.root.elements["savings"]
stocks = report.root.elements["stocks"]

print "\n"
print "Checking account: ", checking.elements["accountNumber"].text.strip, "\n"
print "Balance: ", checking.elements["balance"].text.strip, "\n"

print "Savings account: ", savings.elements["accountNumber"].text.strip, "\n"
print "Balance: ", savings.elements["balance"].text.strip, "\n"

print "Stocks account:", stocks.elements["accountNumber"].text.strip, "\n"
print "Symbol: ", stocks.elements["symbol"].text.strip, "\n"
print "Quantity: ", stocks.elements["quantity"].text.strip, "\n"
print "Balance: ", stocks.elements["balance"].text.strip, "\n"
