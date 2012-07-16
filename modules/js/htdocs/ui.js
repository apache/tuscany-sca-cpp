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
 * UI utility functions.
 */

var ui = {};

/**
 * Return a child element of a node with the given id.
 */
ui.elementByID = function(node, id) {
    if (node.skipNode == true)
        return null;
    for (var i in node.childNodes) {
        var child = node.childNodes[i];
        if (child.id == id)
            return child;
        var gchild = ui.elementByID(child, id);
        if (gchild != null)
            return gchild;
    }
    return null;
};

/**
 * Return the current document, or a child element with the given id.
 */
function $(id) {
    if (id == document)
        return document;
    return ui.elementByID($(document), id);
};

/**
 * Return a list of elements with the given class name.
 */
ui.elementsByClassName = function(node, c) {
    return nodeList(node.getElementsByClassName(c));
};

/**
 * Return the query string of a URL.
 */
ui.query = function(url) {
    var u = '' + url;
    var q = u.indexOf('?');
    return q >= 0? u.substring(q + 1) : '';
};

/**
 * Return the fragment part of a URL.
 */
ui.fragment = function(url) {
    var u = '' + url;
    var h = u.indexOf('#');
    return h >= 0? u.substring(h + 1) : '';
};

/**
 * Return the path and parameters of a URL.
 */
ui.pathandparams = function(url) {
    var u = '' + url;
    var ds = u.indexOf('//');
    var u2 = ds > 0? u.substring(ds + 2) : u;
    var s = u2.indexOf('/');
    return s > 0? u2.substring(s) : '';
};

/**
 * Return a dictionary of query parameters in a URL.
 */
ui.queryParams = function(url) {
    var qp = new Array();
    var qs = ui.query(url).split('&');
    for (var i = 0; i < qs.length; i++) {
        var e = qs[i].indexOf('=');
        if (e > 0)
            qp[qs[i].substring(0, e)] = unescape(qs[i].substring(e + 1));
    }
    return qp;
};

/**
 * Return a dictionary of fragment parameters in a URL.
 */
ui.fragmentParams = function(url) {
    var qp = new Array();
    var qs = ui.fragment(url).split('&');
    for (var i = 0; i < qs.length; i++) {
        var e = qs[i].indexOf('=');
        if (e > 0)
            qp[qs[i].substring(0, e)] = unescape(qs[i].substring(e + 1));
    }
    return qp;
};

/**
 * Convert a base64-encoded image to a data URL.
 */
ui.b64img = function(b64) {
    return 'data:image/png;base64,' + b64;
};

/**
 * Declare a CSS stylesheet.
 */
ui.declareCSS = function(s) {
    var e = document.createElement('style');
    e.type = 'text/css';
    e.textContent = s;
    return e;
};

/**
 * Declare a script.
 */
ui.declareScript = function(s) {
    var e = document.createElement('script');
    e.type = 'text/javascript';
    e.text = s;
    return e;
};

/**
 * Return the scripts elements under a given element.
 */
ui.innerScripts = function(e) {
    return map(function(s) { return s.text; },  nodeList(e.getElementsByTagName('script')));
};

/**
 * Evaluate a script.
 */
ui.evalScript = function(s) {
    return eval('(function() {\n' + s + '\n})();');
};

/**
 * Include a script.
 */
ui.includeScript = function(s) {
    //log('include', s);
    return eval(s);
};

/**
 * Return true if the client is a mobile device.
 */
ui.mobiledetected = false;
ui.mobile = false;
ui.isMobile = function() {
    if (ui.mobiledetected)
        return ui.mobile;
    var ua = navigator.userAgent;
    if (ua.match(/iPhone/i) || ua.match(/iPad/i) || ua.match(/Android/i) || ua.match(/Blackberry/i) || ua.match(/WebOs/i))
        ui.mobile = true;
    ui.mobiledetected = true;
    return ui.mobile;
};

/**
 * Convert a CSS position to a numeric position.
 */
ui.numpos = function(p) {
    return p == ''? 0 : Number(p.substr(0, p.length - 2));
};

/**
 * Convert a numeric position to a CSS pixel position.
 */
ui.pixpos = function(p) {
    return p + 'px';
};

/**
 * Default orientation change behavior.
 */
ui.onorientationchange = function(e) {

    // Scroll to the top and hide the address bar
    window.scrollTo(0, 0);

    // Change fixed position elements to absolute then back to fixed
    // to make sure they're correctly layed out after the orientation
    // change
    map(function(e) {
            e.style.position = 'absolute';
            return e;
        }, ui.elementsByClassName(document, 'fixed'));

    setTimeout(function() {
        map(function(e) {
                e.style.position = 'fixed';
                return e;
            }, ui.elementsByClassName(document, 'fixed'));
        }, 0);
    return true;
};

/**
 * Default page load behavior.
 */
ui.onload = function() {

    // Scroll to the top and hide the address bar
    window.scrollTo(0, 0);

    // Initialize fixed position elements only after the page is loaded,
    // to workaround layout issues with fixed position on mobile devices
    setTimeout(function() {
        map(function(e) {
                e.style.position = 'fixed';
                return e;
            }, ui.elementsByClassName(document, 'fixed'));
        }, 0);
    return true;
};

/**
 * Navigate to a new document.
 */
ui.navigate = function(url, win) {
    //log('navigate', url, win);

    // Open a new window
    if (win == '_blank') {
        window.top.open(url, win);
        return false;
    }

    // Open a new document in the current window
    if (win == '_self') {
        window.top.open(url, win);
        return false;
    }

    // Reload the current window
    if (win == '_reload') {
        window.top.location = url;
        window.top.location.reload();
        return false;
    }

    // Let the current top window handle the navigation
    if (win == '_view') {
        if (!window.top.onnavigate)
            return window.top.open(url, '_self');
        window.top.onnavigate(url);
        return false;
    }

    window.top.open(url, win);
    return false;
}

/**
 * Build a portable <a href> tag.
 */
ui.href = function(id, loc, target, html) {
    if (target == '_blank')
        return '<a id="' + id + '" href="' + loc + '" target="_blank">' + html + '</a>';
    return '<a id="' + id + '" href="' + loc + '" onclick="return ui.navigate(\'' + loc + '\', \'' + target + '\');">' + html + '</a>';
};

/**
 * Build a menu bar.
 */ 
ui.menu = function(id, name, href, target, hilight) {
    function Menu() {
        this.content = function() {
            if (hilight == true)
                return ui.href(id, href, target, '<span class="tbarsmenu">' + name + '</span>');
            else if (hilight == false)
                return ui.href(id, href, target, '<span class="tbaramenu">' + name + '</span>');
            else
                return ui.href(id, href, target, '<span class="' + hilight + '">' + name + '</span>');
        };
    }
    return new Menu();
};

ui.menufunc = function(id, name, fun, hilight) {
    function Menu() {
        this.content = function() {
            function href(id, fun, html) {
                return '<a id="' + id + '" href="/" onclick="' + fun + '">' + html + '</a>';
            }

            if (hilight == true)
                return href(id, fun, '<span class="tbarsmenu">' + name + '</span>');
            else if (hilight == false)
                return href(id, fun, '<span class="tbaramenu">' + name + '</span>');
            else
                return href(id, fun, '<span class="' + hilight + '">' + name + '</span>');
        };
    }
    return new Menu();
};

ui.menubar = function(left, right) {
    var bar = '';
    for (i in left)
        bar = bar + '<span class="tbarleft">' + left[i].content() + '</span>';
    for (i in right)
        bar = bar + '<span class="tbarright">' + right[i].content() + '</span>';
    return bar;
};
 
/**
 * Convert a list of elements to an HTML table.
 */
ui.datatable = function(l) {

    function indent(i) {
        if (i == 0)
            return '';
        return '&nbsp;&nbsp;' + indent(i - 1);
    }

    function rows(l, i) {
        if (isNil(l))
            return '';
        var e = car(l);

        // Convert a list of simple values into a list of name value pairs
        if (!isList(e))
            return rows(expandElementValues("'value", l), i);

        // Convert a list of complex values into a list of name value pairs
        if (isList(car(e)))
            return rows(expandElementValues("'value", l), i);

        // Generate table row for a simple element value
        if (elementHasValue(e)) {
            var v = elementValue(e);
            if (!isList(v)) {
                return '<tr><td class="datatdl">' + indent(i) + elementName(e).slice(1) + '</td>' +
                    '<td class="datatdr tdw">' + (v != null? v : '') + '</td></tr>' +
                    rows(cdr(l), i);
            }

            return rows(expandElementValues(elementName(e), v), i) + rows(cdr(l), i);
        }

        // Generate table row for an element with children
        return '<tr><td class="datatdl">' + indent(i) + elementName(e).slice(1) + '</td>' +
            '<td class="datatdr tdw">' + '</td></tr>' +
            rows(elementChildren(e), i + 1) +
            rows(cdr(l), i);
    }

    return '<table class="datatable ' + (window.name == 'dataFrame'? ' databg' : '') + '" style="width: 100%;">' + rows(l, 0) + '</table>';
}

/**
 * Convert a list of elements to an HTML single column table.
 */
ui.datalist = function(l) {

    function rows(l, i) {
        if (isNil(l))
            return '';
        var e = car(l);

        // Convert a list of simple values into a list of name value pairs
        if (!isList(e))
            return rows(expandElementValues("'value", l), i);

        // Convert a list of complex values into a list of name value pairs
        if (isList(car(e)))
            return rows(expandElementValues("'value", l), i);

        // Generate table row for a simple element value
        if (elementHasValue(e)) {
            var v = elementValue(e);
            if (!isList(v)) {
                return '<tr><td class="datatd tdw">' + (v != null? v : '') + '</td></tr>' +
                    rows(cdr(l), i);
            }

            return rows(expandElementValues(elementName(e), v), i) + rows(cdr(l), i);
        }

        // Generate rows for an element's children
        return rows(elementChildren(e), i + 1) + rows(cdr(l), i);
    }

    return '<table class="datatable ' + (window.name == 'dataFrame'? ' databg' : '') + '" style="width: 100%;">' + rows(l, 0) + '</table>';
}

