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

class AccountServiceImpl

  attr_writer :accountDataService
  attr_writer :stockQuoteService
  attr_writer :currency
  
  def initialize()
    print "Ruby - AccountServiceImpl.initialize\n"
  end

  def getAccountReport(customerID)
  
    # Get the checking account info
    checking = @accountDataService.getCheckingAccount(customerID)

    # Get the savings account info
    savings = @accountDataService.getSavingsAccount(customerID)
    
    # Get the stocks account info
    stock = @accountDataService.getStockAccount(customerID);
    
    symbol = stock.root.elements["symbol"].text
    quantity = stock.root.elements["quantity"].text
    
    # Get the stock price
    price = @stockQuoteService.getQuote(symbol);
    
    # Convert to the configured currency
    price = price.to_f * 0.80 if @currency == "EUR"

    # Calculate the balance
    balance = price.to_f * quantity.to_f
    
    # Form the AccountReport document
    report = Document.new <<-eof
      <AccountReport xmlns="http://www.bigbank.com/AccountService"
        xsi:type="AccountReport" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

        <checking>
          <accountNumber>#{ checking.root.elements["accountNumber"].text }</accountNumber>
          <balance>#{ checking.root.elements["balance"].text }</balance>
        </checking>

        <savings>
          <accountNumber>#{ savings.root.elements["accountNumber"].text }</accountNumber>
          <balance>#{ savings.root.elements["balance"].text }</balance>
        </savings>

        <stocks>
          <accountNumber>#{ stock.root.elements["accountNumber"].text }</accountNumber>
          <symbol>#{ symbol }</symbol>
          <quantity>#{ quantity }</quantity>
          <balance>#{ balance }</balance>
        </stocks>
        
      </AccountReport>
    eof
    
    return report
  end
  
end
