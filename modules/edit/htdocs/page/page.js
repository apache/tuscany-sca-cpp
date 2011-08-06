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
var palcx = 2500;

/**
 * Init a page editor. Works with all browsers except IE.
 */
page.edit = function(elem, wvalue, wadd, wcopy, wdelete, onchange, onselect) {

    // Track element dragging and selection
    page.dragging = null;
    page.selected = null;
    wvalue.disabled = true;
    wcopy.disabled = true;
    wdelete.disabled = true;

    // Trigger widget select and page change events
    page.onpagechange = onchange;
    page.onwidgetselect = onselect;

    /**
     * Handle a mouse down event.
     */
    elem.onmousedown = function(e) {

        // On mouse controlled devices, engage the click component selection
        // logic right away
        if (typeof e.touches == 'undefined')
            elem.onclick(e);

        // Find a draggable element
        var dragging = page.draggable(e.target, elem);
        if (dragging == null || dragging != page.selected)
            return true;
        page.dragging = dragging;

        // Remember mouse position
        var pos = typeof e.touches != "undefined"? e.touches[0] : e;
        page.dragX = pos.screenX;
        page.dragY = pos.screenY;

        if (e.preventDefault)
            e.preventDefault();
        else
            e.returnValue = false;
        return true;
    };

    // Support touch devices
    elem.ontouchstart = elem.onmousedown;

    /**
     * Handle a mouse up event.
     */
    elem.onmouseup = function(e) {
        if (page.dragging == null)
            return true;

        // Snap to grid
        var newX = page.gridsnap(ui.numpos(page.dragging.style.left));
        var newY = page.gridsnap(ui.numpos(page.dragging.style.top));
        page.dragging.style.left = ui.pixpos(newX);
        page.dragging.style.top = ui.pixpos(newY);
        page.dragging.cover.style.left = ui.pixpos(newX);
        page.dragging.cover.style.top = ui.pixpos(newY);

        // Fixup widget style
        page.fixupwidget(page.dragging);

        // Forget dragged element
        page.dragging = null;

        // Trigger page change event
        page.onpagechange(false);
        return true;
    };

    // Support touch devices
    elem.ontouchend = elem.onmouseup;

    /**
     * Handle a mouse move event.
     */
    window.onmousemove = function(e) {
        if (page.dragging == null)
            return true;

        // Get the mouse position
        var pos = typeof e.touches != "undefined"? e.touches[0] : e;
        if (pos.screenX == page.dragX && pos.screenY == page.dragY)
            return true;

        // Compute position of dragged element
        var curX = ui.numpos(page.dragging.style.left);
        var curY = ui.numpos(page.dragging.style.top);
        var newX = curX + (pos.screenX - page.dragX);
        var newY = curY + (pos.screenY - page.dragY);
        if (newX >= palcx)
            page.dragX = pos.screenX;
        else
            newX = palcx;
        if (newY >= 0)
            page.dragY = pos.screenY;
        else
            newY = 0;

        // Move the dragged element
        page.dragging.style.left = ui.pixpos(newX);
        page.dragging.style.top = ui.pixpos(newY);
        page.dragging.cover.style.left = ui.pixpos(newX);
        page.dragging.cover.style.top = ui.pixpos(newY);
        return true;
    };

    // Support touch devices
    elem.ontouchmove = window.onmousemove;

    /**
     * Handle a mouse click event.
     */
    elem.onclick = function(e) {

        // Find selected element
        var selected = page.draggable(e.target, elem);
        if (selected == null) {
            if (page.selected != null) {

                // Reset current selection
                page.widgetselect(page.selected, false, wvalue, wcopy, wdelete);
                page.selected = null;

                // Trigger widget select event
                page.onwidgetselect(null);
            }

            // Dismiss the palette
            if (ui.numpos(elem.style.left) != (palcx * -1))
                elem.style.left = ui.pixpos(palcx * -1);

            return true;
        }

        // Deselect the previously selected element
        page.widgetselect(page.selected, false, wvalue, wcopy, wdelete);

        // Clone element dragged from palette
        if (selected.id.substring(0, 8) == 'palette:') {
            page.selected = page.clone(selected);

            // Move into the editing area and hide the palette
            page.selected.style.left = ui.pixpos(ui.numpos(page.selected.style.left) + palcx);
            page.selected.cover.style.left = ui.pixpos(ui.numpos(page.selected.cover.style.left) + palcx);
            elem.style.left = ui.pixpos(palcx * -1);
        
            // Bring it to the top
            page.bringtotop(page.selected);

            // Trigger page change event
            page.onpagechange(true);

        } else {

            // Bring selected element to the top
            page.selected = selected;
            page.bringtotop(page.selected);
        }

        // Select the element
        page.widgetselect(page.selected, true, wvalue, wcopy, wdelete);

        // Trigger widget select event
        page.onwidgetselect(page.selected);

        return true;
    };

    /**
     * Handle field on change events.
     */
    wvalue.onchange = wvalue.onblur = function() {
        if (page.selected == null)
            return false;
        page.settext(page.selected, wvalue.value);
        page.selected.cover.style.width = ui.pixpos(page.selected.clientWidth + 4);
        page.selected.cover.style.height = ui.pixpos(page.selected.clientHeight + 4);

        // Trigger page change event
        page.onpagechange(true);
        return false;
    };

    // Handle add widget event.
    wadd.onclick = function() {

        // Show the palette
        elem.style.left = ui.pixpos(0);
        return false;
    };

    // Handle delete event.
    wdelete.onclick = function() {
        if (page.selected == null)
            return false;

        // Reset current selection
        page.widgetselect(page.selected, false, wvalue, wcopy, wdelete);

        // Remove selected widget
        page.selected.parentNode.removeChild(page.selected);
        page.selected.cover.parentNode.removeChild(page.selected.cover);
        page.selected = null;

        // Trigger widget select event
        page.onwidgetselect(null);

        // Trigger page change event
        page.onpagechange(true);
        return false;
    };

    // Handle copy event.
    wcopy.onclick = function() {
        if (page.selected == null)
            return false;
        if (page.selected.id.substring(0, 8) == 'palette:')
            return false;

        // Reset current selection
        page.widgetselect(page.selected, false, wvalue, wcopy, wdelete);

        // Clone selected widget
        page.selected = page.clone(page.selected);

        // Move 10 pixels down right
        page.selected.style.left = ui.pixpos(ui.numpos(page.selected.style.left) + 10);
        page.selected.style.top = ui.pixpos(ui.numpos(page.selected.style.top) + 10);
        page.selected.cover.style.left = ui.pixpos(ui.numpos(page.selected.cover.style.left) + 10);
        page.selected.cover.style.top = ui.pixpos(ui.numpos(page.selected.cover.style.top) + 10);
    
        // Bring it to the top
        page.bringtotop(page.selected);

        // Select the element
        page.widgetselect(page.selected, true, wvalue, wcopy, wdelete);

        // Trigger widget select event
        page.onwidgetselect(page.selected);

        // Trigger page change event
        page.onpagechange(true);
        return false;
    };

    // Cover child elements with span elements to prevent
    // any input events to reach them
    map(page.cover, nodeList(elem.childNodes));

    return elem;
};

/**
 * Return the text of a widget.
 */
page.text = function(e) {
    function formula(e) {
        var f = e.id;
        if (f.substring(0, 5) != 'page:')
            return '=' + f;
        return '';
    }

    function constant(e, f) {
        if (e.className == 'h1' || e.className == 'h2' || e.className == 'text' || e.className == 'section') {
            var t = car(childElements(e)).innerHTML;
            return t == f? '' : t;
        }
        if (e.className == 'button' || e.className == 'checkbox') {
            var t = car(childElements(e)).value;
            return t == f? '' : t;
        }
        if (e.className == 'entry' || e.className == 'password') {
            var t = car(childElements(e)).defaultValue;
            return t == f? '' : t;
        }
        if (e.className == 'select') {
            var t = car(childElements(car(childElements(e)))).value;
            return t == f? '' : t;
        }
        if (e.className == 'link') {
            var lhr = car(childElements(e)).href;
            var hr = lhr.substring(0, 5) == 'link:'? lhr.substring(5) : '';
            var t = car(childElements(car(childElements(e)))).innerHTML;
            return t == f? hr : (t == hr? hr : (t == ''? hr : hr + ',' + t));
        }
        if (e.className == 'img') {
            var src = car(childElements(e)).src;
            return src == window.location.href? '' : src;
        }
        if (e.className == 'iframe') {
            var hr = car(childElements(e)).href;
            return hr == window.location.href? '' : hr;
        }
        if (e.className == 'list')
            return '';
        if (e.className == 'table')
            return '';
        return '';
    }

    var f = formula(e);
    var c = constant(e, f);
    return f == ''? c : (c == ''? f : f + ',' + c);
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
    function formula(t) {
        if (t.length > 1 && t.substring(0, 1) == '=')
            return car(t.split(','));
        return '';
    }

    function constant(t) {
        return t.length > 1 && t.substring(0, 1) == '='? cdr(t.split(',')) : t.split(',');
    }

    var f = formula(t);
    var c = constant(t);

    e.id = f != ''? f.substring(1) : ('page:' + e.className);

    if (e.className == 'h1' || e.className == 'h2' || e.className == 'text' || e.className == 'section') {
        car(childElements(e)).innerHTML = isNil(c)? f : car(c);
        return t;
    }
    if (e.className == 'button' || e.className == 'entry' || e.className == 'password') {
        car(childElements(e)).defaultValue = isNil(c)? f : car(c);
        return t;
    }
    if (e.className == 'checkbox') {
        car(childElements(e)).value = isNil(c)? f : car(c);
        map(function(n) { if (n.nodeName == "SPAN") n.innerHTML = isNil(c)? f : car(c); return n; }, nodeList(e.childNodes));
        return t;
    }
    if (e.className == 'select') {
        var ce = car(childElements(car(childElements(e))));
        ce.value = isNil(c)? f : car(c);
        ce.innerHTML = isNil(c)? f : car(c);
        return t;
    }
    if (e.className == 'list') {
        e.innerHTML = '<table class="datatable" style="width: 100%;;"><tr><td class="datatd">' + (isNil(c)? f : car(c)) + '</td></tr><tr><td class="datatd">...</td></tr></table>';
        return t;
    }
    if (e.className == 'table') {
        e.innerHTML = '<table class="datatable" style="width: 100%;"><tr><td class="datatdl">' + (isNil(c)? f : car(c)) + '</td><td class="datatdr">...</td></tr><tr><td class="datatdl">...</td><td class="datatdr">...</td></tr></table>';
        return t;
    }
    if (e.className == 'link') {
        var ce = car(childElements(e));
        ce.href = isNil(c)? 'link:/index.html' : ('link:' + car(c));
        car(childElements(ce)).innerHTML = isNil(c)? (f != ''? f : '/index.html') : isNil(cdr(c))? (f != ''? f : car(c)) : cadr(c);
        return t;
    }
    if (e.className == 'img') {
        car(childElements(e)).src = isNil(c)? '/public/img.png' : car(c);
        return t;
    }
    if (e.className == 'iframe') {
        car(childElements(e)).href = isNil(c)? '/public/iframe.html' : car(c);
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
        e.style.width = '100%';
        car(childElements(e)).style.width = '100%';
        return e;
    }
    if (e.className == 'table') {
        e.style.width = '100%';
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
 * Draw widget selection.
 */
page.widgetselect = function(n, s, wvalue, wcopy, wdelete) {
    if (isNil(n) || !s) {
        // Clear the widget value field
        wvalue.value = '';
        wvalue.disabled = true;
        wcopy.disabled = true;
        wdelete.disabled = true;

        // Clear the widget outline
        if (!isNil(n))
            n.cover.style.borderWidth = '0px';
        return true;
    }

    // Update the widget value field
    wvalue.value = page.text(n);
    wvalue.disabled = false;
    wcopy.disabled = false;
    wdelete.disabled = false;

    // Outline the widget
    n.cover.style.borderWidth = '2px';
    return true;
};

/**
 * Cover a page element with a <span> element to prevent
 * any input events to reach it.
 */
page.cover = function(e) {
    if (e.id == '' || isNil(e.style))
        return e;
    var cover = document.createElement('div');
    cover.style.position = 'absolute';
    cover.style.left = ui.pixpos(ui.numpos(e.style.left) - 2);
    cover.style.top = ui.pixpos(ui.numpos(e.style.top) - 2);
    cover.style.width = ui.pixpos(e.clientWidth + 4);
    cover.style.height = ui.pixpos(e.clientHeight + 4);
    cover.style.visibility = 'inherit';
    cover.style.borderStyle = 'solid';
    cover.style.borderWidth = '0px';
    cover.style.borderColor = '#598edd';
    cover.style.padding = '0px';
    cover.style.margin = '0px';
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
        ne.id = 'page:' + e.id.substr(8);

        // Copy the class and HTML content
        ne.className = e.className;
        ne.innerHTML = e.innerHTML;

        // Fixup the widget style
        page.fixupwidget(ne);

        return ne;
    }

    /**
     * Clone an element's position.
     */
    function posclone(ne, e) {
        ne.style.position = 'absolute';
        ne.style.left = ui.pixpos(ui.numpos(e.style.left));
        ne.style.top = ui.pixpos(ui.numpos(e.style.top));
        e.parentNode.appendChild(ne);
        page.cover(ne);
        return ne;
    }

    return posclone(mkclone(e), e);
};

