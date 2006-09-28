cd $TUSCANY_SCACPP/test/ws.service.interop/deploy/bin

# Start the axis http server. Output goes to a log file
./runwsserver.sh > runwsserver.log &

# The wget command makes sure the server has fully started up
wget -O - http://localhost:9090/axis2/services

# Run the tests against the server
./runclient.sh

# Finally, kill the axis http server by finding the PID
kill -9 `ps | grep axis2_http_serv | cut -c1-6`
