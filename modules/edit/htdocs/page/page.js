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
var page = {};

if (ui.isIE()) {

    /**
     * Init a page editor. IE-specific implementation.
     */
    page.initpage = function(elem) {

        // Keep track of the current dragged element
        page.dragging = null;

        /**
         * Handle a mouse down event.
         */
        elem.onmousedown = function() {
            window.event.returnValue = false;

            // Find a draggable element
            page.dragging = page.draggable(window.event.srcElement, elem);
            if (page.dragging == null)
                return false;

            // Bring it to the top
            page.bringtotop(page.dragging);

            // Save the mouse position
            page.dragX = window.event.clientX;
            page.dragY = window.event.clientY;
            elem.setCapture();
            return false;
        };

        /**
         * Handle a mouse up event.
         */
        elem.onmouseup = function() {
            if (page.dragging == null)
                return false;

            // Discard element dragged out of page area
            if (ui.csspos(page.dragging.style.left) < 350 && page.dragging.id.substring(0, 8) != 'palette:')
                page.dragging.parentNode.removeChild(page.dragging);

            // Forget current dragged element
            page.dragging = null;
            elem.releaseCapture();
            return false;
        };

        /**
         * Handle a mouse move event.
         */
        elem.onmousemove = function() {
            if (page.dragging == null)
                return false;

            // Compute dragged element position
            var curX = ui.csspos(page.dragging.style.left);
            var curY = ui.csspos(page.dragging.style.top);
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

            // Clone element dragged from palette
            if (page.dragging.id.substring(0, 8) == 'palette:')
                page.dragging = page.clone(page.dragging);

            // Move dragged element
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;

        };

        // Cover child elements with span elements to prevent
        // any input events to reach them
        map(page.cover, nodeList(elem.childNodes));

        return elem;
    };

} else {

    /**
     * Init a page editor. Generic implementation for all other browsers.
     */
    page.initpage = function(elem) {
        page.dragging = null;

        /**
         * Handle a mouse down event.
         */
        elem.onmousedown = function(e) {
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;

            // Find a draggable element
            page.dragging = page.draggable(e.target, elem);
            if (page.dragging == null)
                return false;

            // Bring it to the top
            page.bringtotop(page.dragging);

            // Remember mouse position
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            page.dragX = pos.screenX;
            page.dragY = pos.screenY;
            return false;
        };

        // Support touch devices
        elem.ontouchstart = elem.onmousedown;

        /**
         * Handle a mouse up event.
         */
        window.onmouseup = function(e) {
            if (page.dragging == null)
                return false;

            // Discard element if dragged out of page area
            if (ui.csspos(page.dragging.style.left) < 350 && page.dragging.id.substring(0, 8) != 'palette:')
                page.dragging.parentNode.removeChild(page.dragging);

            // Forget dragged element
            page.dragging = null;
            return false;
        };

        // Support touch devices
        window.top.onmouseup = window.onmouseup;
        window.ontouchend = window.onmouseup;
        window.top.ontouchend = window.onmouseup;

        /**
         * Handle a mouse move event.
         */
        window.onmousemove = function(e) {
            if (page.dragging == null)
                return false;

            // Compute position of dragged element
            var curX = ui.csspos(page.dragging.style.left);
            var curY = ui.csspos(page.dragging.style.top);
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

            // Clone element dragged from palette
            if (page.dragging.id.substring(0, 8) == 'palette:') {
                page.dragging = page.clone(page.dragging);
            }

            // Move the dragged element
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;
            return false;
        };

        // Support touch devices
        window.top.onmousemove = window.onmousemove;
        window.ontouchmove = window.onmousemove;
        window.top.ontouchmove = window.onmousemove;

        // Cover child elements with span elements to prevent
        // any input events to reach them
        map(page.cover, nodeList(elem.childNodes));

        return elem;
    };
}

/**
 * Find a draggable element in a hierarchy of elements.
 */
page.draggable = function(n, e) {
    if (n == e)
        return null;
    if (n.id != '')
        return n;
    if (n.covered)
        return n.covered;
    return page.draggable(n.parentNode, e);
}

/**
 * Bring an element and its parent to the top.
 */
page.bringtotop = function(n) {
    n.parentNode.appendChild(n);
    n.cover.parentNode.appendChild(n.cover);
}

/**
 * Cover a page element with a <span> element to prevent
 * any input events to reach it.
 */
page.cover = function(e) {
    if (e.id == '' || isNil(e.style))
        return e;
    var cover = document.createElement('span');
    cover.style.position = 'absolute';
    cover.style.left = ui.csspos(e.style.left) - 5;
    cover.style.top = ui.csspos(e.style.top) - 5;
    cover.style.width = e.clientWidth + 10;
    cover.style.height = e.clientHeight + 10;
    cover.style.visibility = 'visible';
    cover.covered = e;
    e.cover = cover;
    e.parentNode.appendChild(cover);
    return e;
}

/**
 * Clone a palette element.
 */
page.clone = function(e) {

    /**
     * Clone an element's HTML.
     */
    function mkclone(e) {
        var ne = document.createElement('span');

        // Skip the palette: prefix
        ne.id = e.id.substr(8);

        // Copy the HTML content
        ne.innerHTML = e.innerHTML;
        return ne;
    }

    /**
     * Clone an element's position.
     */
    function posclone(ne, e) {
        ne.style.position = 'absolute';
        ne.style.left = ui.csspos(e.style.left);
        ne.style.top = ui.csspos(e.style.top);
        e.parentNode.appendChild(ne);
        page.cover(ne);
        return ne;
    }

    return posclone(mkclone(e), e);
};

