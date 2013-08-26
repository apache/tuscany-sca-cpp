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

# Image processing functions
from StringIO import StringIO
try:
    from PIL import Image
    from PIL import ImageOps
    from PIL import ExifTags
except:
    Image = None
from base64 import b64encode, b64decode
from urllib import urlopen
from util import *
from sys import debug

# Rotate image if needed
def rotateimg(img):
    debug('imgutil.py::rotateimg')
    if not hasattr(img, '_getexif'):
        return img
    exif = img._getexif()
    if exif is None:
        return img
    for tag, value in exif.items():
        decoded = ExifTags.TAGS.get(tag, tag)
        if decoded == 'Orientation':
            if value == 3: return img.rotate(180)
            if value == 6: return img.rotate(270)
            if value == 8: return img.rotate(90)
    return img

# Convert image to a cropped thumbnail
def thumbnail(img, size):
    def intsz(*nums):
        return tuple(int(round(n)) for n in nums)

    class imgsz(object):
        def __init__(self, pair):
            self.width = float(pair[0])
            self.height = float(pair[1])
            self.aspect_ratio = self.width / self.height
            self.size = intsz(self.width, self.height)

    osz = imgsz(img.size)
    tsz = imgsz(size)

    if tsz.aspect_ratio > osz.aspect_ratio:
        scale = tsz.width / osz.width
        crop = imgsz((osz.width, tsz.height / scale))
        cut = (osz.height - crop.height) / 2
        img = img.crop(intsz(0, cut, crop.width, cut + crop.height))
    elif tsz.aspect_ratio < osz.aspect_ratio:
        scale = tsz.height / osz.height
        crop = imgsz((tsz.width / scale, osz.height))
        cut = (osz.width - crop.width) / 2
        img = img.crop(intsz(cut, 0,  cut + crop.width, crop.height))

    return img.resize(tsz.size, Image.ANTIALIAS)

# Convert image URL to a 50x50 JPEG
def urlto50x50jpeg(url):
    debug('imgutil.py::urlto50x50jpeg::url', url)
    if Image is None:
        return url
    img = Image.open(StringIO(b64decode(url.split(',')[1])) if url.startswith('data:') else StringIO(urlopen(url).read()))
    if img.size == (50, 50):
        debug('imgutil.py::urlto50x50jpeg::res', url)
        return url
    thumb = thumbnail(rotateimg(img), (50, 50))
    obuf = StringIO()
    thumb.save(obuf, 'JPEG')
    res = 'data:image/jpeg;base64,' + b64encode(obuf.getvalue()).replace('\n', '')
    debug('imgutil.py::urlto50x50jpeg::res', res)
    return res;

# Convert image to a 50x50 JPEG
def bufto50x50jpeg(buf):
    debug('imgutil.py::bufto50x50jpeg')
    if Image is None:
        return ''
    img = Image.open(StringIO(buf))
    if img.size == (50, 50):
        debug('imgutil.py::urlto50x50jpeg::res', url)
        return url
    thumb = thumbnail(rotateimg(img), (50, 50))
    obuf = StringIO()
    thumb.save(obuf, 'JPEG')
    res = 'data:image/jpeg;base64,' + b64encode(obuf.getvalue()).replace('\n', '')
    debug('imgutil.py::bufto50x50jpeg::res', res)
    return res

