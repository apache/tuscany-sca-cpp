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

require "rexml/document"
include REXML

class AccountDataServiceImpl

  def initialize()
    print "Ruby - AccountDataServiceImpl.initialize\n"
  end

  def getCheckingAccount(id)
    print "Ruby - AccountDataServiceImpl.getCheckingAccount ", id, "\n"
    
    result = Document.new <<-eof
      <CheckingAccount xmlns="http://www.bigbank.com/AccountService"
        xsi:type="CheckingAccount" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <accountNumber>#{ id.to_s + "_CHA12345"}</accountNumber>
        <balance>1500.0</balance>
      </CheckingAccount>
    eof
    
    return result
  end
	
  def getSavingsAccount(id)
    print "Ruby - AccountDataServiceImpl.getSavingsAccount ", id, "\n"

    result = Document.new <<-eof
      <SavingsAccount xmlns="http://www.bigbank.com/AccountService"
        xsi:type="SavingsAccount"  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <accountNumber>#{ id.to_s + "_SAA12345"}</accountNumber>
        <balance>1500.0</balance>
      </SavingsAccount>
    eof
    
    return result
  end
	
  def getStockAccount(id)
    print "Ruby - AccountDataServiceImpl.getStockAccount ", id, "\n"

    result = Document.new <<-eof
      <StockAccount xmlns="http://www.bigbank.com/AccountService"
        xsi:type="StockAccount"  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <accountNumber>#{ id.to_s + "_STA12345"}</accountNumber>
        <symbol>IBM</symbol>
        <quantity>100</quantity>
        <balance></balance>
      </StockAccount>
    eof
    
    return result
  end
  
end
