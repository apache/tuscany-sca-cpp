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
     * Init a page editor.
     */
    page.initpage = function(elem) {
        page.dragging = null;

        function draggable(n) {
            if (n == elem)
                return null;
            if (n.id != '')
                return n;
            if (n.covered)
                return n.covered;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            n.parentNode.appendChild(n);
            n.cover.parentNode.appendChild(n.cover);
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
            var curX = ui.posn(page.dragging.style.left);
            var curY = ui.posn(page.dragging.style.top);
            var newX = curX + (window.event.clientX - page.dragX);
            var newY = curY + (window.event.clientY - page.dragY);
            if (newX >= 0)
                page.dragX = window.event.clientX;
            else
                newX = 0;
            if (newY >= 0)
                page.dragY = window.event.clientY;
            else
                newY = 0;

            if (page.dragging.id.substring(0, 8) == 'palette:') {
                // Clone the dragged element
                page.dragging = page.clone(page.dragging);
            }
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;

        };

        // Cover child elements with span elements
        map(page.cover, nodeList(elem.childNodes));

        return elem;
    };

} else {

    /**
     * Init a page editor.
     */
    page.initpage = function(elem) {
        page.dragging = null;

        function draggable(n) {
            if (n == elem)
                return null;
            if (n.id != '')
                return n;
            if (n.covered)
                return n.covered;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            n.parentNode.appendChild(n);
            n.cover.parentNode.appendChild(n.cover);
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
            page.dragX = pos.screenX;
            page.dragY = pos.screenY;
            return false;
        };

        elem.ontouchstart = elem.onmousedown;

        window.onmouseup = function(e) {
            if (page.dragging == null)
                return false;
            page.dragging = null;
            return false;
        };

        window.top.onmouseup = window.onmouseup;
        window.ontouchend = window.onmouseup;
        window.top.ontouchend = window.onmouseup;

        window.onmousemove = function(e) {
            if (page.dragging == null)
                return false;
            var curX = ui.posn(page.dragging.style.left);
            var curY = ui.posn(page.dragging.style.top);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = curX + (pos.screenX - page.dragX);
            var newY = curY + (pos.screenY - page.dragY);
            if (newX >= 0)
                page.dragX = pos.screenX;
            else
                newX = 0;
            if (newY >= 0)
                page.dragY = pos.screenY;
            else
                newY = 0;

            if (page.dragging.id.substring(0, 8) == 'palette:') {
                // Clone the dragged element
                page.dragging = page.clone(page.dragging);
            }
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;
            return false;
        };

        window.top.onmousemove = window.onmousemove;
        window.ontouchmove = window.onmousemove;
        window.top.ontouchmove = window.onmousemove;

        // Cover child elements with span elements
        map(page.cover, nodeList(elem.childNodes));

        return elem;
    };
}

/**
 * Cover a page element with a <span> element to prevent mouse events to reach it.
 */
page.cover = function(e) {
    if (e.id == '' || isNil(e.style))
        return e;
    var cover = document.createElement('span');
    cover.style.position = 'absolute';
    cover.style.left = ui.posn(e.style.left) - 5;
    cover.style.top = ui.posn(e.style.top) - 5;
    cover.style.width = e.clientWidth + 10;
    cover.style.height = e.clientHeight + 10;
    cover.style.visibility = 'visible';
    cover.covered = e;
    e.cover = cover;
    e.parentNode.appendChild(cover);
    return e;
}

/**
 * Clone a page element.
 */
page.clone = function(e) {
    function mkclone(e) {
        var ne = document.createElement('span');
        ne.id = e.id.substr(8);
        ne.innerHTML = e.innerHTML;
        return ne;
    }

    function posclone(ne, e) {
        ne.style.position = 'absolute';
        ne.style.left = ui.posn(e.style.left);
        ne.style.top = ui.posn(e.style.top);
        e.parentNode.appendChild(ne);
        page.cover(ne);
        return ne;
    }

    return posclone(mkclone(e), e);
};

