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

/**
 * Default positions and sizes.
 */
var palcx = 250;
var trashcx = 230;

if (ui.isIE()) {

    /**
     * Init a page editor. IE-specific implementation.
     */
    page.edit = function(elem, wname, wtext, onchange) {

        // Track element dragging and selection
        page.dragging = null;
        page.selected = null;
        wname.disabled = true;
        wtext.disabled = true;

        // Trigger page change events
        page.onpagechange = onchange;

        /**
         * Handle a mouse down event.
         */
        elem.onmousedown = function() {
            window.event.returnValue = false;

            // Find a draggable element
            page.dragging = page.draggable(window.event.srcElement, elem);
            page.selected = page.dragging;
            if (page.dragging == null) {

                // Reset current selection
                wname.value = '';
                wname.disabled = true;
                wtext.value = '';
                wtext.disabled = true;
                return false;
            }

            // Clone element dragged from palette
            if (page.dragging.id.substring(0, 8) == 'palette:') {
                page.dragging = page.clone(page.dragging);
                page.selected = page.dragging;
            }

            // Bring it to the top
            page.bringtotop(page.dragging);

            // Save the mouse position
            page.dragX = window.event.clientX;
            page.dragY = window.event.clientY;
            elem.setCapture();

            // Update the widget name and text fields
            wname.value = page.selected.id;
            wname.disabled = false;
            wtext.value = page.text(page.selected);
            wtext.disabled = !page.hastext(page.selected);
            return false;
        };

        /**
         * Handle a mouse up event.
         */
        elem.onmouseup = function() {
            if (page.dragging == null)
                return false;

            // Snap to grid
            var newX = page.gridsnap(ui.csspos(page.dragging.style.left));
            var newY = page.gridsnap(ui.csspos(page.dragging.style.top));
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;

            // Fixup widget style
            page.fixupwidget(page.dragging);

            // Discard element dragged out of page area
            if (ui.csspos(page.dragging.style.left) < palcx && page.dragging.id.substring(0, 8) != 'palette:') {
                if (ui.csspos(page.dragging.style.left) >= trashcx) {

                    // Unless it's close enough to page area, then move it there
                    page.dragging.style.left = palcx;
                    page.dragging.cover.style.left = palcx;
                } else {
                    page.dragging.parentNode.removeChild(page.dragging);

                    // Reset current selection
                    page.selected = null;
                    wname.value = '';
                    wname.disabled = true;
                    wtext.value = '';
                    wtext.disabled = true;
                }
            }

            // Forget current dragged element
            page.dragging = null;
            elem.releaseCapture();
            
            // Trigger page change event
            page.onpagechange(false);
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

            // Move dragged element
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;
            return false;
        };

        /**
         * Handle field on change events.
         */
        wname.onchange = wname.onblur = function() {
            if (page.selected == null)
                return false;
            page.selected.id = wname.value;

            // Trigger page change event
            page.onpagechange(true);
            return false;
        };

        wtext.onchange = wtext.onblur = function() {
            if (page.selected == null)
                return false;
            page.settext(page.selected, wtext.value);

            // Trigger page change event
            page.onpagechange(true);
            return false;
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
    page.edit = function(elem, wname, wtext, onchange) {

        // Track element dragging and selection
        page.dragging = null;
        page.selected = null;
        wname.disabled = true;
        wtext.disabled = true;

        // Trigger page change events
        page.onpagechange = onchange;

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
            page.selected = page.dragging;
            if (page.dragging == null) {

                // Reset current selection
                wname.value = '';
                wname.disabled = true;
                wtext.value = '';
                wtext.disabled = true;
                return false;
            }

            // Clone element dragged from palette
            if (page.dragging.id.substring(0, 8) == 'palette:') {
                page.dragging = page.clone(page.dragging);
                page.selected = page.dragging;
            }

            // Bring it to the top
            page.bringtotop(page.dragging);

            // Remember mouse position
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            page.dragX = pos.screenX;
            page.dragY = pos.screenY;

            // Update the widget name and text fields
            wname.value = page.selected.id;
            wname.disabled = false;
            wtext.value = page.text(page.selected);
            wtext.disabled = !page.hastext(page.selected);
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

            // Snap to grid
            var newX = page.gridsnap(ui.csspos(page.dragging.style.left));
            var newY = page.gridsnap(ui.csspos(page.dragging.style.top));
            page.dragging.style.left = newX;
            page.dragging.style.top = newY;
            page.dragging.cover.style.left = newX;
            page.dragging.cover.style.top = newY;

            // Fixup widget style
            page.fixupwidget(page.dragging);

            // Discard element dragged out of page area
            if (ui.csspos(page.dragging.style.left) < palcx && page.dragging.id.substring(0, 8) != 'palette:') {
                if (ui.csspos(page.dragging.style.left) >= trashcx) {

                    // Unless it's close enough to page area, then move it there
                    page.dragging.style.left = palcx;
                    page.dragging.cover.style.left = palcx;
                } else {
                    page.dragging.parentNode.removeChild(page.dragging);

                    // Reset current selection
                    page.selected = null;
                    wname.value = '';
                    wname.disabled = true;
                    wtext.value = '';
                    wtext.disabled = true;
                }
            }

            // Forget dragged element
            page.dragging = null;

            // Trigger page change event
            page.onpagechange(false);
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

        /**
         * Handle field on change events.
         */
        wname.onchange = wname.onblue = function() {
            if (page.selected == null)
                return false;
            page.selected.id = wname.value;

            // Trigger page change event
            page.onpagechange(true);
            return false;
        };

        wtext.onchange = wtext.onblur = function() {
            if (page.selected == null)
                return false;
            page.settext(page.selected, wtext.value);

            // Trigger page change event
            page.onpagechange(true);
            return false;
        };

        // Cover child elements with span elements to prevent
        // any input events to reach them
        map(page.cover, nodeList(elem.childNodes));

        return elem;
    };
}

/**
 * Return the text of a widget.
 */
page.text = function(e) {
    if (e.className == 'h1' || e.className == 'h2' || e.className == 'text' || e.className == 'section')
        return car(childElements(e)).innerHTML;
    if (e.className == 'button' || e.className == 'checkbox')
        return car(childElements(e)).value;
    if (e.className == 'entry' || e.className == 'password')
        return car(childElements(e)).defaultValue;
    if (e.className == 'select')
        return car(childElements(car(childElements(e)))).value;
    if (e.className == 'link') {
        var hr = car(childElements(e)).href;
        var t = car(childElements(car(childElements(e)))).innerHTML;
        return hr == t? hr : hr + ',' + t;
    }
    if (e.className == 'img') {
        var src = car(childElements(e)).src;
        return src == window.location.href? '' : src;
    }
    if (e.className == 'iframe')
        return car(childElements(e)).href;
    if (e.className == 'list')
        return '';
    if (e.className == 'table')
        return '';
    return '';
};

/**
 * Return true if a widget has editable text.
 */
page.hastext = function(e) {
    if (e.className == 'h1' || e.className == 'h2' || e.className == 'text' || e.className == 'section')
        return true;
    if (e.className == 'button' || e.className == 'checkbox')
        return true;
    if (e.className == 'entry' || e.className == 'password')
        return true;
    if (e.className == 'select')
        return false;
    if (e.className == 'link')
        return true;
    if (e.className == 'img')
        return true;
    if (e.className == 'iframe')
        return true;
    if (e.className == 'list')
        return false;
    if (e.className == 'table')
        return false;
    return false;
};

/**
 * Set the text of a widget.
 */
page.settext = function(e, t) {
    if (e.className == 'h1' || e.className == 'h2' || e.className == 'text' || e.className == 'section') {
        car(childElements(e)).innerHTML = t;
        return t;
    }
    if (e.className == 'button' || e.className == 'entry' || e.className == 'password') {
        car(childElements(e)).defaultValue = t;
        return t;
    }
    if (e.className == 'checkbox') {
        car(childElements(e)).value = t;
        map(function(n) { if (n.nodeName == "SPAN") n.innerHTML = t; return n; }, nodeList(e.childNodes));
        return t;
    }
    if (e.className == 'select') {
        var ce = car(childElements(car(childElements(e))));
        ce.value = t;
        ce.innerHTML = t;
        return t;
    }
    if (e.className == 'list') {
        return '';
    }
    if (e.className == 'table') {
        return '';
    }
    if (e.className == 'link') {
        var l = t.split(',');
        var ce = car(childElements(e));
        ce.href = car(l);
        car(childElements(ce)).innerHTML = isNil(cdr(l))? car(l) : cadr(l);
        return t;
    }
    if (e.className == 'img') {
        car(childElements(e)).src = t;
        return t;
    }
    if (e.className == 'iframe') {
        car(childElements(e)).href = t;
        return t;
    }
    return '';
};

/**
 * Initial fixup of a widget.
 */
page.fixupwidget = function(e) {
    if (e.className == 'iframe') {
        var f = car(childElements(e));
        //e.innerHTML = '<iframe src="' + f.href + '" frameborder="no" scrolling="no"></iframe>';
        return e;
    }
    if (e.className == 'section') {
        e.style.width = '100%';
        return e;
    }
    if (e.className == 'list') {
        car(childElements(e)).style.width = '100%';
        return e;
    }
    if (e.className == 'table') {
        car(childElements(e)).style.width = '100%';
        return e;
    }
    return e;
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
 * Align a pos along a 9pixel grid.
 */
page.gridsnap = function(x) {
    return Math.round(x / 9) * 9;
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

        // Copy the class and HTML content
        ne.className = e.className;
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

