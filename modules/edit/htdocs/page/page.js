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

/**
 * Page editing functions.
 */
var page = new Object();

if (ui.isIE()) {

    /**
     * Init a page.
     */
    page.initpage = function(elem) {
        page.dragging = null;

        function draggable(n) {
            if (n == elem)
                return null;
            if (n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            if (n == elem)
                return null;
            n.parentNode.appendChild(n);
            return bringtotop(n.parentNode);
        }

        elem.onmousedown = function() {
            window.event.returnValue = false;
            page.dragging = draggable(window.event.srcElement);
            if (page.dragging == null)
                return false;
            bringtotop(page.dragging);
            page.dragX = window.event.clientX;
            page.dragY = window.event.clientY;
            elem.setCapture();
            return false;
        };

        elem.onmouseup = function() {
            if (page.dragging == null)
                return false;
            page.dragging = null;
            elem.releaseCapture();
            return false;
        };

        elem.onmousemove = function() {
            if (page.dragging == null)
                return false;
            var origX = page.dragging.coordorigin.X;
            var origY = page.dragging.coordorigin.Y;
            var newX = origX - (window.event.clientX - page.dragX);
            var newY = origY - (window.event.clientY - page.dragY);
            page.dragX = window.event.clientX;
            page.dragY = window.event.clientY;

            if (page.dragging.id.substring(0, 8) == 'palette:') {
                // Clone the dragged element
                page.dragging = page.clone(page.dragging);
            }
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            return false;
        };

        return elem;
    };

} else {

    /**
     * Init a page.
     */
    page.initpage = function(elem) {
        page.dragging = null;

        function draggable(n) {
            if (n == elem)
                return null;
            if (n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            if (n == elem)
                return null;
            n.parentNode.appendChild(n);
            return bringtotop(n.parentNode);
        }

        elem.onmousedown = function(e) {
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;
            page.dragging = draggable(e.target);
            if (page.dragging == null)
                return false;
            bringtotop(page.dragging);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            page.dragX = pos.clientX;
            page.dragY = pos.clientY;
            return false;
        };

        elem.ontouchstart = elem.onmousedown;

        elem.onmouseup = function(e) {
            if (page.dragging == null)
                return false;
            page.dragging = null;
            return false;
        };

        elem.ontouchend = elem.onmouseup;

        elem.onmousemove = function(e) {
            if (page.dragging == null)
                return false;
            var curX = ui.posn(page.dragging.style.left);
            var curY = ui.posn(page.dragging.style.top);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = curX + (pos.clientX - page.dragX);
            var newY = curY + (pos.clientY - page.dragY);
            page.dragX = pos.clientX;
            page.dragY = pos.clientY;

            if (page.dragging.id.substring(0, 8) == 'palette:') {
                // Clone the dragged element
                page.dragging = page.clone(page.dragging);
            }
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            return false;
        };

        elem.ontouchmove = elem.onmousemove;

        return elem;
    };
}

/**
 * Clone an HTML element.
 */
page.elemcount = 0;

page.clone = function(e) {
    function mkclone(e) {
        if (e.nodeName == 'INPUT' && e.type == 'button') {
            var ne = document.createElement('input');
            ne.type = 'button';
            ne.id = 'button' + (++page.elemcount);
            ne.value = ne.id;
            return ne;
        }
        if (e.nodeName == 'INPUT' && e.type == 'text') {
            var ne = document.createElement('input');
            ne.type = 'text';
            ne.id = 'entry' + (++page.elemcount);
            ne.value = ne.id;
            return ne;
        }
        if (e.nodeName == 'SPAN') {
            var ne = document.createElement('span');
            ne.id = 'text' + (++page.elemcount);
            ne.innerHTML = ne.id;
            return ne;
        }
    }

    function posclone(ne, e) {
        ne.style.position = 'absolute';
        ne.style.left = ui.posn(e.style.left);
        ne.style.top = ui.posn(e.style.top);
        e.parentNode.appendChild(ne);
        return ne;
    }

    return posclone(mkclone(e), e);
};

