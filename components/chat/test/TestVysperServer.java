/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package test;

/**
 * A test XMPP server, using Apache Vysper.
 */
import static java.lang.System.*;

import java.io.File;

import org.apache.vysper.mina.TCPEndpoint;
import org.apache.vysper.stanzasession.StanzaSessionFactory;
import org.apache.vysper.storage.StorageProviderRegistry;
import org.apache.vysper.storage.inmemory.MemoryStorageProviderRegistry;
import org.apache.vysper.xmpp.authorization.AccountManagement;
import org.apache.vysper.xmpp.modules.extension.xep0049_privatedata.PrivateDataModule;
import org.apache.vysper.xmpp.modules.extension.xep0054_vcardtemp.VcardTempModule;
import org.apache.vysper.xmpp.modules.extension.xep0092_software_version.SoftwareVersionModule;
import org.apache.vysper.xmpp.modules.extension.xep0119_xmppping.XmppPingModule;
import org.apache.vysper.xmpp.modules.extension.xep0202_entity_time.EntityTimeModule;
import org.apache.vysper.xmpp.server.XMPPServer;

class TestVysperServer {
    public static void main(final String args[]) throws Exception {
        out.println("Starting test Vysper server...");

        // Add the XMPP users used by the xmpp-test and server-test test cases
        // If you're using your own XMPP server you need to add these users manually
        final StorageProviderRegistry providerRegistry = new MemoryStorageProviderRegistry();
        final AccountManagement accountManagement = (AccountManagement)providerRegistry.retrieve(AccountManagement.class);
        accountManagement.addUser("sca1@localhost", "sca1");
        accountManagement.addUser("sca2@localhost", "sca2");
        accountManagement.addUser("sca3@localhost", "sca3");

        // Create and start XMPP server for domain: localhost
        final XMPPServer server = new org.apache.vysper.xmpp.server.XMPPServer("localhost");
        server.addEndpoint(new TCPEndpoint());
        server.addEndpoint(new StanzaSessionFactory());
        server.setStorageProviderRegistry(providerRegistry);
        final File cert = new File(TestVysperServer.class.getClassLoader().getResource("bogus_mina_tls.cert").getPath());
        server.setTLSCertificateInfo(cert, "boguspw");
        server.start();
        server.addModule(new SoftwareVersionModule());
        server.addModule(new EntityTimeModule());
        server.addModule(new VcardTempModule());
        server.addModule(new XmppPingModule());
        server.addModule(new PrivateDataModule());
        out.println("Test Vysper server started...");

        // Wait forever
        final Object lock = new Object();
        synchronized(lock) {
            lock.wait();
        }

        System.out.println("Stopping test Vysper server...");
        server.stop();
        out.println("Test Vysper server stopped.");
        System.exit(0);
    }
}
