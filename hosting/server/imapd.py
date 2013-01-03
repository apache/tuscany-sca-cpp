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

# Load media received from an IMAP inbox
from imaplib2 import IMAP4_SSL
from threading import Thread, Event
from email import message_from_string
import re
from StringIO import StringIO
from PIL import Image
from base64 import b64encode, b64decode
from httplib import HTTPConnection, HTTPSConnection
from urlparse import urlparse
from util import *
from sys import stderr, argv, exit
from traceback import print_exc

# Debug print
def debug(*args):
    if True:
        print >> stderr, args

# Fetch an email and return the image it contains
def fetchmail(id, imap):

    # Extract address from the To header
    htyp, hdata = imap.fetch(id, '(BODY.PEEK[HEADER])')
    header = message_from_string(hdata[0][1])
    if header['To'] is None:
        return (None, 'Couldn\'t retrieve email address')
    b64aparts = re.findall('[\w=-_]+@[\w.]+', header['To'])
    if len(b64aparts) == 0:
        return (None, 'Couldn\'t parse email address')
    b64address = (b64aparts[len(b64aparts) - 1]).split('@')[0]
    try:
        address = b64decode((b64address.replace('-', '+').replace('_', '/') + '===')[0: len(b64address) + (len(b64address) % 4)])
    except:
        return (None, 'Couldn\'t decode email address')

    # Check if the address targets a picture (p/) or an icon (i/)
    if address[0:2] != 'p/' and address[0:2] != 'i/':
        return (None, 'Email address must start with p/ or i/')
    debug('putimages.py::readimage::address', address)

    # Extract image mime body part
    btyp, bdata = imap.fetch(id, '(BODY.PEEK[TEXT])')
    msg = message_from_string(hdata[0][1] + bdata[0][1])
    debug('putimages.py::readimage::msg', msg)
    parts = map(lambda p: p.get_payload(decode=True), filter(lambda p: p.get_content_type().startswith('image/'), msg.walk()))
    if len(parts) == 0:
        return (None, 'Email doesn\'t contain an image')

    # Convert image to a 50x50 PNG image
    img = Image.open(StringIO(parts[0]))
    t = img.resize((50, 50))
    obuf = StringIO()
    t.save(obuf, 'PNG')
    dataurl = 'data:image/png;base64,' + b64encode(obuf.getvalue()).replace('\n', '')

    # Return address, image url pair
    return (address, dataurl)

def putimage(address, dataurl, httpurl, httpuser, httppass):

    # Put image into the image database
    id = address.split('/')[1]
    token = address.split('/')[2]
    entry = '<?xml version="1.0" encoding="UTF-8"?>\n' + \
        '<entry xmlns="http://www.w3.org/2005/Atom">\n' + \
        '<title type="text">' + id + '</title>\n' + \
        '<id>' + id + '</id>\n' + \
        '<content type="application/xml">\n' + \
        ('<picture>\n' if address[0:2] == 'p/' else '<icon>\n') + \
        '<image>' + dataurl + '</image>\n' + \
        '<token>' + token + '</token>\n' + \
        ('</picture>\n' if address[0:2] == 'p/' else '</icon>\n') + \
        '</content>\n' + \
        '</entry>'

    url = urlparse(httpurl)
    conn = HTTPSConnection(url.hostname, 443 if url.port is None else url.port) if url.scheme == 'https' else \
            HTTPConnection(url.hostname, 80 if url.port is None else url.port)
    #conn.set_debuglevel(9)
    path = url.path + ('pictures/' if address[0:2] == 'p/' else 'icons/') + id
    puturl = url.scheme + '//' + url.netloc + path
    debug('imapd.py::putimage::url', puturl)
    auth = b64encode("%s:%s" % (httpuser, httppass)).replace('\n', '')
    headers = { 'Authorization' : 'Basic ' + auth, 'X-Forwarded-Server' : url.hostname, 'Content-type': 'application/atom+xml', 'Accept': '*/*' }
    conn.request('PUT', path, entry, headers)
    response = conn.getresponse()
    if response.status != 200:
        debug('imapd.py::putimage::error', response.status, response.reason)
        return (None, 'Put error: ' + repr(response.status) + ' : ' + response.reason)
    conn.close()

    return (puturl, entry)

# Read and process an email
def processmail(id, imap, httpurl, httpuser, httppass):
    if id == '':
        return None

    # Read email and any image in it
    address, dataurl = fetchmail(id, imap)
    if address is None:
        # Mark email as seen if it doesn't contain an image
        debug('imapd.py::processmail::seen', id)
        imap.store(id, '+FLAGS', '\SEEN')
        return None

    # Put image into the database
    put = putimage(address, dataurl, httpurl, httpuser, httppass)
    if put[0] is None:
        return None

    # Mark email as seen if processed successfully
    debug('imapd.py::processmail::seen', id)
    imap.store(id, '+FLAGS', '\SEEN')
    return put[0]
    
# IMAP idle thread
def idle(imap, httpurl, httpuser, httppass, stop, stopped):
    try:
        sync = Event()
        while True:
            # Stop the thread
            if stop.isSet():
                debug('imapd.py::idle::stopped')
                stopped.set()
                return

            # Wait for changes
            def callback(args):
                debug('imapd.py::idle::callback')
                if not stop.isSet():
                    sync.set()
                    stop.set()

            debug('imapd.py::idle::waiting')
            imap.idle(callback = callback)
            stop.wait()

            # Handle email change event
            if sync.isSet():
                stop.clear()
                sync.clear()
                debug('imapd.py::idle::sync')

                # List unseen emails
                typ, data = imap.search(None, 'UNSEEN')
                debug('imapd.py::idle::search', typ, data)

                # Process unseen email
                map(lambda id: processmail(id, imap, httpurl, httpuser, httppass), data[0].split(' '))

    except Exception as e:
        debug('imapd.py::idle::except', e)
        print_exc()
        stopped.set()
        return

# Main processing loop
def main(imapurl, imapuser, imappass, httpurl, httpuser, httppass):
    try:
        # Connect and login
        url = urlparse(imapurl)
        imap = IMAP4_SSL(url.hostname, 993 if url.port is None else url.port)
        imap.login(imapuser, imappass)
        imap.select('Inbox')
        debug('imapd.py::main::connected')

        try:
            # Start imap idle thread
            stop = Event()
            stopped = Event()
            idling = Thread(target=idle, args=(imap, httpurl, httpuser, httppass, stop, stopped))
            idling.start()

            # List unseen emails
            typ, data = imap.search(None, 'UNSEEN')
            debug('imapd.py::main::search', typ, data)

            # Process unseen emails
            map(lambda id: processmail(id, imap, httpurl, httpuser, httppass), data[0].split(' '))

            # Wait 60 seconds
            debug('imapd.py::main::waiting')
            try:
                stopped.wait()
            except KeyboardInterrupt:
                pass

            # Stop the thread
            debug('imapd.py::main::stopping')
            stop.set()
            idling.join()

            # Close and logout
            debug('imapd.py::main::disconnecting')
            imap.close()
            imap.logout()
            return 0

        except Exception as e:
            debug('imapd.py::except', e)
            print_exc()
            # Close and logout
            imap.close()
            imap.logout()
            return 1

    except Exception as e:
        debug('imapd.py::except', e)
        print_exc()
        return 1

if __name__ == '__main__':
    exit(main(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]))

