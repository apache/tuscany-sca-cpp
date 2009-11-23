<html>
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
define('APP_ROOT', $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/bigbank.phpwsclient');
?>

<h1>Welcome To The BigBank Sample</h1>

 <form action="http://<?php echo APP_ROOT . '/bigaccount.php' ?>" method="POST">
   Please enter your account ID 
   <input type="text" name="accountname" size="30" value="ED" />   
   <button type="submit">GetAccount</button>
 </form>

 <div id="statusString"/>
</body>
</html>
