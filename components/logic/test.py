#!/usr/bin/python
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

# Test the simple parallel programming components

import unittest
from property import *
from reference import *
from environment import *

from nuvem import true_
from nuvem import false_
from nuvem import number
from nuvem import text
from nuvem import comment
from nuvem import name
from nuvem import nothing
from nuvem import item
from nuvem import single
from nuvem import pair
from nuvem import triple
from nuvem import call
from nuvem import if_
from nuvem import and_
from nuvem import or_
from nuvem import not_
from nuvem import equals
from nuvem import lesser
from nuvem import greater
from nuvem import list_
from nuvem import empty
from nuvem import first
from nuvem import second
from nuvem import rest
from nuvem import insert
from nuvem import append
from nuvem import itemnb
from nuvem import names
from nuvem import values
from nuvem import lookup
from nuvem import select
from nuvem import reverse
from nuvem import shuffle_
from nuvem import range_
from nuvem import map_
from nuvem import valueof
from nuvem import filter_
from nuvem import reduce_
from nuvem import add
from nuvem import subtract
from nuvem import multiply
from nuvem import divide
from nuvem import random_
from nuvem import randoms
from nuvem import sin_
from nuvem import cos_
from nuvem import round_
from nuvem import floor_
from nuvem import ceil_
from nuvem import mod
from nuvem import sum_
from nuvem import min_
from nuvem import max_
from nuvem import host
from nuvem import path
from nuvem import params
from nuvem import param
from nuvem import user
from nuvem import realm
from nuvem import email
from nuvem import keychain
from nuvem import url
from nuvem import contains
from nuvem import split
from nuvem import join
from nuvem import replace
from nuvem import lowercase
from nuvem import uppercase
from nuvem import format_
from nuvem import parse
from nuvem import htattrs
from nuvem import htimg
from nuvem import htinline
from nuvem import htlink
from nuvem import htbutton
from nuvem import htcheck
from nuvem import htstyle
from nuvem import pixels
from nuvem import left
from nuvem import top
from nuvem import transform
from nuvem import transition
from nuvem import frames
from nuvem import eval_
from nuvem import exec_

def testValues():
    assert true_.get(()) == True
    assert false_.get(()) == False
    assert nothing.get(()) == None
    assert number.get((), mkprop('value', lambda: 1)) == 1
    assert text.get((), mkprop('value', lambda: 'abc')) == 'abc'
    assert comment.get((), mkprop('value', lambda: 'abc')) == 'abc'
    assert name.get((), mkprop('value', lambda: 'abc')) == "'abc"
    assert item.get((), mkref('value', lambda r: 'def'), mkprop('name', lambda: 'abc')) == ("'abc", 'def')
    return True

def testLogic():
    assert call.get((), mkref('name', lambda r: "'abc"), mkref('proxy', lambda c, r: (c, r))) == ("'abc", ())
    assert call.get((), mkref('name', lambda r: 'abc'), mkref('proxy', lambda c, r: (c, r))) == ("'abc", ())
    assert if_.get((), mkref('cond', lambda r: True), mkref('then', lambda r: 'abc'), mkref('els', lambda r: 'def')) == 'abc'
    assert if_.get((), mkref('cond', lambda r: False), mkref('then', lambda r: 'abc'), mkref('els', lambda r: 'def')) == 'def'
    assert and_.get((), mkref('a', lambda r: False), mkref('b', lambda r: True)) == False
    assert and_.get((), mkref('a', lambda r: True), mkref('b', lambda r: True)) == True
    assert or_.get((), mkref('a', lambda r: False), mkref('b', lambda r: False)) == False
    assert or_.get((), mkref('a', lambda r: False), mkref('b', lambda r: True)) == True
    assert not_.get((), mkref('expr', lambda r: False)) == True
    assert not_.get((), mkref('expr', lambda r: True)) == False
    assert equals.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'abc')) == True
    assert equals.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'def')) == False
    assert lesser.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'abc')) == False
    assert lesser.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'def')) == True
    assert greater.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'abc')) == False
    assert greater.get((), mkref('a', lambda r: 'def'), mkref('b', lambda r: 'abc')) == True
    return True

def testLists():
    assert single.get((), mkref('x', lambda r: 'abc')) == ('abc',)
    assert pair.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'def')) == ('abc', 'def')
    assert triple.get((), mkref('a', lambda r: 'abc'), mkref('b', lambda r: 'def'), mkref('c', lambda r: 'ghi')) == ('abc', 'def', 'ghi')
    assert list_.get((), mkref('item', lambda r: None)) == ()
    assert list_.get((), mkref('item', lambda r: 'abc'), mkref('item', lambda r: None)) == ('abc',)
    assert list_.get((), mkref('item', lambda r: 'abc'), mkref('item', lambda r: 'def'), mkref('item', lambda r: None)) == ('abc', 'def')
    assert insert.get((), mkref('first', lambda r: 'abc'), mkref('rest', lambda r: None)) == ('abc',)
    assert insert.get((), mkref('first', lambda r: 'abc'), mkref('rest', lambda r: ())) == ('abc',)
    assert insert.get((), mkref('first', lambda r: 'abc'), mkref('rest', lambda r: ('def',))) == ('abc', 'def')
    assert insert.get((), mkref('first', lambda r: 'abc'), mkref('rest', lambda r: ('def', 'ghi'))) == ('abc', 'def', 'ghi')
    assert empty.get(()) == ()
    assert first.get((), mkref('l', lambda r: ('abc', 'def'))) == 'abc'
    assert second.get((), mkref('l', lambda r: ('abc', 'def'))) == 'def'
    assert rest.get((), mkref('l', lambda r: ('abc', 'def', 'ghi'))) == ('def', 'ghi')
    assert rest.get((), mkref('l', lambda r: ('abc',))) == ()
    assert append.get((), mkref('l', lambda r: ('abc', 'def')), mkref('b', lambda r: ('ghi',))) == ('abc', 'def', 'ghi')
    assert append.get((), mkref('l', lambda r: 'abc'), mkref('b', lambda r: ('def', 'ghi'))) == ('abc', 'def', 'ghi')
    assert append.get((), mkref('l', lambda r: ('abc', 'def')), mkref('b', lambda r: 'ghi')) == ('abc', 'def', 'ghi')
    assert itemnb.get((), mkref('i', lambda r: 1), mkref('l', lambda r: ('abc', 'def', 'ghi'))) == 'def'
    assert names.get((), mkref('l', lambda r: (("'a", 'abc'), ("'d", 'def'), ("'g", 'ghi'), ("'d", 'def2')))) == ("'a", "'d", "'g", "'d")
    assert values.get((), mkref('l', lambda r: (("'a", 'abc'), ("'d", 'def'), ("'g", 'ghi'), ("'d", 'def2')))) == ('abc', 'def', 'ghi', 'def2')
    assert reverse.get((), mkref('l', lambda r: ('abc', 'def', 'ghi'))) == ('ghi', 'def', 'abc')
    assert shuffle_.get((), mkref('l', lambda r: ('abc', 'def', 'ghi'))) != ()
    assert range_.get((), mkref('a', lambda r: '1'), mkref('b', lambda r: '4')) == (1, 2, 3)

    assert map_.get((), mkref('item', lambda r: "'i"), mkref('transform', lambda r: valueof.get(r, mkprop('name', lambda: 'i')) * 2), mkref('list', lambda r: (1, 2, 3))) == (2, 4, 6)
    assert filter_.get((), mkref('item', lambda r: "'i"), mkref('cond', lambda r: valueof.get(r, mkprop('name', lambda: 'i')) % 2 == 0), mkref('list', lambda r: (1, 2, 3, 4))) == (2, 4)
    assert reduce_.get((), mkref('item', lambda r: "'i"), mkref('accum', lambda r: "'a"), mkref('transform', lambda r: valueof.get(r, mkprop('name', lambda: 'a')) + valueof.get(r, mkprop('name', lambda: 'i'))), mkref('init', lambda r: 0), mkref('list', lambda r: (1, 2, 3, 4))) == 10
    return True

def testSelect():
    assert lookup.get((), mkref('n', lambda r: "'d"), mkref('l', lambda r: (("'a", 'abc'), ("'d", 'def'), ("'g", 'ghi'), ("'d", 'def2')))) == (("'d", 'def'), ("'d", 'def2'))
    assert select.get((), mkref('n', lambda r: "'d.x"), mkref('l', lambda r: (("'a", 'abc'), ("'d", (("'x", 'def'), ("'y", "yyy"))), ("'g", 'ghi'), ("'d", (("'y", 'yyy'), ("'x", 'def2')))))) == (("'x", 'def'), ("'x", 'def2'))
    assert select.get((), mkref('n', lambda r: "'d.x"), mkref('l', lambda r: (("'a", 'abc'), ("'d", (("'x", 'def'), ("'y", "yyy"))), ("'g", 'ghi'), ("'d", (("'y", 'yyy'), ("'x", 'def2')))))) == (("'x", 'def'), ("'x", 'def2'))
    assert select.get((), mkref('n', lambda r: ("'d", "'x")), mkref('l', lambda r: (("'a", 'abc'), ("'d", (("'x", 'def'), ("'y", "yyy"))), ("'g", 'ghi'), ("'d", (("'y", 'yyy'), ("'x", 'def2')))))) == (("'x", 'def'), ("'x", 'def2'))

    dnf = (("'status", 'ok'), ("'origin", (''),), ("'dest", ('sc'),), ("'rows", ((("'elements", ((("'status", 'notfound'),),),),),),),)
    assert select.get((), mkref('n', lambda r: "'status"), mkref('l', lambda r: dnf)) == (("'status", 'ok'), ("'status", 'notfound'))
    assert select.get((), mkref('n', lambda r: "'rows.elements.status"), mkref('l', lambda r: dnf)) == (("'status", 'notfound'),)

    dok = (("'status", 'ok'), ("'origin", ('scruz'),), ("'dest", ('sc'),), ("'rows", ((("'elements", ((("'status", 'ok'), ("'duration", ("'value", 3761), ("'text", '1 hour 3 mins'),), ("'distance", ("'value", 82923), ("'text", '51.5 mi'),),),),),),),),)
    assert select.get((), mkref('n', lambda r: "'distance.text"), mkref('l', lambda r: dok)) == (("'text", '51.5 mi'),)

def testMath():
    assert add.get((), mkref('a', lambda r: 3), mkref('b', lambda r: 2)) == 5
    assert subtract.get((), mkref('a', lambda r: 3), mkref('b', lambda r: 2)) == 1
    assert multiply.get((), mkref('a', lambda r: 2), mkref('b', lambda r: 3)) == 6
    assert divide.get((), mkref('a', lambda r: 3), mkref('b', lambda r: 2)) == 1.5
    r1 = random_.get((), mkref('m', lambda r: 2))
    assert r1 >= 0 and r1 <= 2
    r2 = random_.get((), mkref('m', lambda r: 2))
    assert r2 >= 0 and r2 <= 2 and r2 != r1
    r3 = random_.get((), mkref('m', lambda r: ('a', 'b')))
    assert r3 == 'a' or r3 == 'b'
    r4 = randoms.get((), mkref('n', lambda r: 5.0), mkref('m', lambda r: 2))
    assert len(r4) == 5
    assert r4[0] >= 0 and r4[0] <= 2
    assert r4[1] >= 0 and r4[1] <= 2 and r4[1] != r4[0]
    r5 = randoms.get((), mkref('n', lambda r: 5.0), mkref('m', lambda r: ('a', 'b')))
    assert len(r5) == 5
    assert r5[0] == 'a' or r5[0] == 'b'
    assert r5[1] == 'a' or r5[1] == 'b'
    assert sin_.get((), mkref('x', lambda r: 0.0)) == 0.0
    assert cos_.get((), mkref('x', lambda r: 0.0)) == 1.0
    assert round_.get((), mkref('d', lambda r: 2), mkref('x', lambda r: 2.336)) == 2.34
    assert floor_.get((), mkref('x', lambda r: 2.336)) == 2.0
    assert ceil_.get((), mkref('x', lambda r: 2.336)) == 3.0
    assert mod.get((), mkref('n', lambda r: 10), mkref('x', lambda r: 12)) == 2.0
    assert sum_.get((), mkref('l', lambda r: (1, 2, 3))) == 6
    assert min_.get((), mkref('l', lambda r: (2, 1, 3))) == 1
    assert max_.get((), mkref('l', lambda r: (1, 3, 2))) == 3
    return True

def testHTTP():
    assert host.get((), mkprop('host', lambda: 'localhost')) == 'localhost'
    assert path.get((), mkprop('path', lambda: ('abc', 'def'))) == ('abc', 'def')
    assert params.get((), mkprop('params', lambda: (("'a", 'abc'), ("'d", 'def')))) == (("'a", 'abc'), ("'d", 'def'))
    assert param.get((), mkprop('n', lambda: "d"), mkprop('params', lambda: (("'a", 'abc'), ("'d", 'def'), ("'g", 'ghi')))) == 'def'
    assert user.get((), mkprop('user', lambda: 'joe')) == 'joe'
    assert realm.get((), mkprop('realm', lambda: 'localhost')) == 'localhost'
    assert email.get((), mkprop('email', lambda: 'joe@localhost')) == 'joe@localhost'
    kc = (("'entry", ("'title", 'User'), ("'id", 'user'), ("'content", ("'account", ("'description", 'User'), ("'keys", (
        "'key", ((("'@name", 'key1'), ("'@value", 'value1')), (("'@name", 'key2'), ("'@value", 'value2')), (("'@name", 'key3'), ("'@value", 'value3')))))))),)
    assert keychain.get((), mkref('n', lambda r: 'key2'), mkref('a', lambda r: kc)) == 'value2'
    assert keychain.get((), mkref('n', lambda r: 'key4'), mkref('a', lambda r: kc)) is None
    return True

def testText():
    assert contains.get((), mkref('sub', lambda r: 'bc'), mkref('s', lambda r: 'abcd')) == True
    assert split.get((), mkref('sep', lambda r: 'x'), mkref('s', lambda r: 'abcxdef')) == ('abc', 'def')
    assert join.get((), mkref('sep', lambda r: 'x'), mkref('s', lambda r: ('abc', 'def'))) == 'abcxdef'
    assert join.get((), mkref('sep', lambda r: None), mkref('s', lambda r: ('abc', 'def'))) == 'abcdef'
    assert replace.get((), mkref('from', lambda r: 'x'), mkref('to', lambda r: 'y'), mkref('s', lambda r: 'abcxdefxghi')) == 'abcydefyghi'
    assert replace.get((), mkref('from', lambda r: 'x'), mkref('to', lambda r: None), mkref('s', lambda r: 'abcxdefxghi')) == 'abcdefghi'
    assert lowercase.get((), mkref('s', lambda r: 'ABC')) == 'abc'
    assert uppercase.get((), mkref('s', lambda r: 'abc')) == 'ABC'
    assert format_.get((), mkref('f', lambda r: '{0} + {1} = {r}'), mkref('a', lambda r: (2, 3, ("'r", 5)))) == '2 + 3 = 5'
    assert parse.get((), mkref('e', lambda r: r'(\w+) (\w+)'), mkref('s', lambda r: 'a b c')) == ('a', 'b')
    return True

def testAnimation():
    assert htattrs.get((), mkref('value', lambda r: ("'value", 'abc'))) == ("'htattrs", ("'value", 'abc'))
    assert htimg.get((), mkref('value', lambda r: 'http://abc.png')) == '<SPAN id="http://abc.png" class="img"><IMG src="http://abc.png"/></SPAN>'
    assert htimg.get((), mkref('value', lambda r: ('i1', 'http://abc.png'))) == '<SPAN id="i1" class="img"><IMG src="http://abc.png"/></SPAN>'
    assert htimg.get((), mkref('value', lambda r: ("'htattrs", (("'id", 'i1'), ("'src", 'http://abc.png'), ("'x", 'X'))))) == '<SPAN id="i1" class="img"><IMG src="http://abc.png" x="X"/></SPAN>'
    assert htimg.get((), mkref('value', lambda r: (("'htattrs", (("'id", 'i1'), ("'src", 'http://abc.png'), ("'x", 'X'))),))) == '<SPAN id="i1" class="img"><IMG src="http://abc.png" x="X"/></SPAN>'
    assert htinline.get((), mkref('value', lambda r: ('image/png', ('a', 'b')))) == 'data:image/png;base64,YWI=\n'
    assert htlink.get((), mkref('value', lambda r: ('http://abc'))) == '<SPAN id="http://abc" class="link"><A href="http://abc"><SPAN>http://abc</SPAN></A></SPAN>'
    assert htlink.get((), mkref('value', lambda r: ('http://abc', 'abc'))) == '<SPAN id="http://abc" class="link"><A href="http://abc"><SPAN>abc</SPAN></A></SPAN>'
    assert htlink.get((), mkref('value', lambda r: ('h1', 'http://abc', 'abc'))) == '<SPAN id="h1" class="link"><A href="http://abc"><SPAN>abc</SPAN></A></SPAN>'
    assert htlink.get((), mkref('value', lambda r: ("'htattrs", (("'id", 'h1'), ("'href", 'http://abc'), ("'value", 'abc'), ("'x", "X"))))) == '<SPAN id="h1" class="link"><A href="http://abc" x="X"><SPAN>abc</SPAN></A></SPAN>'
    assert htlink.get((), mkref('value', lambda r: (("'htattrs", (("'id", 'h1'), ("'href", 'http://abc'), ("'value", 'abc'), ("'x", "X"))),))) == '<SPAN id="h1" class="link"><A href="http://abc" x="X"><SPAN>abc</SPAN></A></SPAN>'
    assert htbutton.get((), mkref('value', lambda r: ('abc'))) == '<SPAN id="abc" class="button"><INPUT type="button" value="abc" class="graybutton"/></SPAN>'
    assert htbutton.get((), mkref('value', lambda r: ('b1', 'abc'))) == '<SPAN id="b1" class="button"><INPUT type="button" value="abc" class="graybutton"/></SPAN>'
    assert htbutton.get((), mkref('value', lambda r: ("'htattrs", (("'id", 'b1'), ("'value", 'abc'), ("'x", 'X'))))) == '<SPAN id="b1" class="button"><INPUT type="button" value="abc" x="X" class="graybutton"/></SPAN>'
    assert htbutton.get((), mkref('value', lambda r: (("'htattrs", (("'id", 'b1'), ("'value", 'abc'), ("'x", 'X'))),))) == '<SPAN id="b1" class="button"><INPUT type="button" value="abc" x="X" class="graybutton"/></SPAN>'
    assert htcheck.get((), mkref('value', lambda r: 'abc')) == '<SPAN id="abc" class="checkbox"><INPUT type="checkbox" value="abc"/><SPAN>abc</SPAN></SPAN>'
    assert htcheck.get((), mkref('value', lambda r: ('c1', 'abc'))) == '<SPAN id="c1" class="checkbox"><INPUT type="checkbox" value="abc"/><SPAN>abc</SPAN></SPAN>'
    assert htcheck.get((), mkref('value', lambda r: ("'htattrs", (("'id", 'c1'), ("'value", 'abc'), ("'x", 'X'))))) == '<SPAN id="c1" class="checkbox"><INPUT type="checkbox" value="abc" x="X"/><SPAN>abc</SPAN></SPAN>'
    assert htcheck.get((), mkref('value', lambda r: (("'htattrs", (("'id", 'c1'), ("'value", 'abc'), ("'x", 'X'))),))) == '<SPAN id="c1" class="checkbox"><INPUT type="checkbox" value="abc" x="X"/><SPAN>abc</SPAN></SPAN>'
    assert htstyle.get((), mkref('value', lambda r: (("'width", '320px'), ("'height", '460px')))) == ("'htstyle", 'width: 320px; height: 460px;')
    assert htstyle.get((), mkref('value', lambda r: ('width: 320px', ("'height", '460px')))) == ("'htstyle", 'width: 320px; height: 460px;')
    assert pixels.get((), mkref('value', lambda r: 320.1)) == '320px'
    assert left.get((), mkref('value', lambda r: 320.1)) == 'left: 320px'
    assert top.get((), mkref('value', lambda r: 320.1)) == 'top: 320px'
    assert transform.get((), mkref('x', lambda r: 320.1), mkref('y', lambda r: 480.2), mkref('d', lambda r: 90.3)) == 'position: absolute; -webkit-transform: translate3d(320px,480px,0px) rotate(90deg); -moz-transform: translate(320px,480px) rotate(90deg); -ms-transform: translate(320px,480px) rotate(90deg); transform: translate(320px,480px) rotate(90deg)'
    assert transition.get((), mkref('value', lambda r: 320.1)) == '-webkit-transition: all 0.3201s'
    kf = ((0, ('left: 0px', 'top: 0px')), (50, ('left: 10px', 'top: 10px')), (100, (("'left", '20px'), 'top: 20px')))
    assert frames.get((), mkref('msec', lambda r: 2000), mkref('loop', lambda r: True), mkref('content', lambda r: kf)) == 'position: absolute; -webkit-animation: {id} 2s ease 0 infinite normal; -moz-animation: {id} 2s ease 0 infinite normal; <style> @-webkit-keyframes {id} { 0% { left: 0px; top: 0px; } 50% { left: 10px; top: 10px; } 100% { left: 20px; top: 20px; } } @-moz-keyframes {id} { 0% { left: 0px; top: 0px; } 50% { left: 10px; top: 10px; } 100% { left: 20px; top: 20px; } } </style>'
    return True

def testEval():
    assert eval_.get((), mkref('py', lambda r: '1 + 2'), mkref('ref', lambda r: 4)) == 3
    assert eval_.get((), mkref('py', lambda r: 'ref.get(r) + 2'), mkref('ref', lambda r: 4)) == 6
    assert eval_.get((5,), mkref('py', lambda r: 'ref.get(r) + r[0] + 2'), mkref('ref', lambda r: 4)) == 11
    assert exec_.get((), mkref('py', lambda r: 'val = 1 + 2'), mkref('ref', lambda r: 4)) == 3
    assert exec_.get((), mkref('py', lambda r: 'val = ref.get(r) + 2'), mkref('ref', lambda r: 4)) == 6
    assert exec_.get((5,), mkref('py', lambda r: 'val = ref.get(r) + r[0] + 2'), mkref('ref', lambda r: 4)) == 11
    return True

if __name__ == '__main__':
    print 'Testing...'
    testValues()
    testLogic()
    testLists()
    testSelect()
    testMath()
    testHTTP()
    testText()
    testAnimation()
    testEval()
    print 'OK'

