
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

cd $TUSCANY_SCACPP/test/ws.service.interop/deploy/bin

# Start the axis http server. Output goes to a log file
./runwsserver.sh > runwsserver.log &

# The wget command makes sure the server has fully started up
wget -O - http://localhost:9090/axis2/services

# Run the tests against the server
./runclient.sh

# Finally, kill the axis http server by finding the PID
kill -9 `ps | grep axis2_http_serv | cut -c1-6`
