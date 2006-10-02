<!--
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0
   
 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.

$Rev$ $Date: 2005/12/22 11:33:21 $ 
-->
<head>
    <title>BigBank Sample</title>
</head>

<body BGCOLOR="#EFEFEF">

<script type="text/javascript"></script>

<?php
  try
  {
  	// In case we want to see what's in the request
  	//print_r ($_POST);
  	
    // get the entered account name
    $accountname =  $_POST['accountname'];

    try 
    {
      // configure the SOAP client 
      $client = new SoapClient("AccountService.wsdl");
      
      // make the call but its a wrapped call so we
      // can't just do...
      // $result = $client->getAccountReport($accountname);
      // we have to construct an object hierarchy that matches the 
      // WSDL request. We can use SDO to do this for us
      
      // configure SDO based on the WSDL
      $xmldas = SDO_DAS_XML::create("AccountService.wsdl");
      
      // Get a document that represents the request
      // and get the root element
	  $requestxdoc = $xmldas->createDocument("getAccountReport");
	  $requestsdo = $requestxdoc->getRootDataObject();

	  // add the parameter
	  $requestsdo['customerID'] = $accountname;
	      
      // make the call
      $result = $client->getAccountReport($requestsdo);
      
      // now we have the result as a PHP object
      // we can turn this into an SDO but we have to do it manually
      // as the SOAP extension won't generate types for us
      // automatically just yet
      $responsexdoc = $xmldas->createDocument("getAccountReportResponse");
	  $responsesdo = $responsexdoc->getRootDataObject();
      
	  // flesh out the object hierarchy
	  $account = $responsesdo->createDataObject ('result');
	  $checking = $account->createDataObject ('checking');
	  $saving = $account->createDataObject ('savings');
	  $stocks = $account->createDataObject ('stocks');
 
	  //copy the data
	  $checking['accountNumber'] = $result->result->checking->accountNumber;
	  $checking['balance'] = $result->result->checking->balance;
	  //etc..
	  // This is a bit rubbish as we would want this to happen automatically
	  
	  // Now just put the data on the screen
	  // To do this we don't need to use SDO as you can see but
	  // it would be really handy if we wanted to pass the XML doc 
	  // onto another service or if we wanted to do some other 
	  // more complex XML manipulation or XPath searches
	  echo "<h2>Account         $accountname</h2>";
	  echo "<h2>Checking Account</h2><table>";
	  echo "<tr><td>Account Number</td><td>  $checking->accountNumber </td></tr>";
	  echo "<tr><td>Balance</td><td>  $checking->balance </td></tr>";
	  echo "</table><h2>Saving Account</h2><table>";
	  echo "<tr><td>Account Number</td><td>" . $result->result->savings->accountNumber . "</td></tr>";
	  echo "<tr><td>Balance</td><td>" . $result->result->savings->balance . "</td></tr>";
	  echo "</table><h2>Stock Account</h2><table>";
	  echo "<tr><td>Account Number</td><td>" . $result->result->stocks->accountNumber . "</td></tr>";
      echo "<tr><td>Symbol</td><td>" . $result->result->stocks->symbol . "</td></tr>";
	  echo "<tr><td>Quantity</td><td>" . $result->result->stocks->quantity . "</td></tr>";
      echo "<tr><td>Banalce</td><td>" . $result->result->stocks->balance . "</td></tr>";	  
      echo "</table>";
    } 
    catch (SoapFault $f) 
    {
      echo "Caught soap exception: \n";
  	  print_r ( $f );
    }
  }
  catch ( Exception $e )
  {
  	echo "Caught exception: \n";
  	print_r ( $e );
  	
  }
?>
