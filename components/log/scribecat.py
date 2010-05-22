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

# Write messages to Scribe

import sys
from scribe import scribe
from thrift.transport import TTransport, TSocket
from thrift.protocol import TBinaryProtocol

category = sys.argv[1]
host = 'localhost'
port = 1464

entry = scribe.LogEntry(category=category, message=sys.stdin.read())
socket = TSocket.TSocket(host=host, port=port)
transport = TTransport.TFramedTransport(socket)
protocol = TBinaryProtocol.TBinaryProtocol(trans=transport, strictRead=False, strictWrite=False)
client = scribe.Client(iprot=protocol, oprot=protocol)
transport.open()
result = client.Log(messages=[entry])
transport.close()

if result == scribe.ResultCode.OK:
  sys.exit()
if result == scribe.ResultCode.TRY_LATER:
  print >> sys.stderr, "Try later"
  sys.exit(84)
sys.exit(result)

