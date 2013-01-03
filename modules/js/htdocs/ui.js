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
        if (isNull(child))
            continue;
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
    return memo(document, '$' + id, function() {
        return ui.elementByID($(document), id);
    });
}

/**
 * Un-memoize elements previously found by id.
 */
ui.unmemo$ = function(prefix) {
    return prefix? unmemo(document, '$' + prefix) : unmemo(document);
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
 * Convert a base64-encoded PNG image to a data URL.
 */
ui.b64png = function(b64) {
    return 'data:image/png;base64,' + b64.trim();
};

/**
 * Convert a base64-encoded JPEG image to a data URL.
 */
ui.b64jpeg = function(b64) {
    return 'data:image/jpeg;base64,' + b64.trim();
};

/**
 * Convert a data URL to a base64-encoded image.
 */
ui.imgb64 = function(img) {
    if (img.startsWith('data:'))
        return img.split(',')[1]
    return '';
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
 * Include a CSS stylesheet.
 */
ui.includeCSS = function(s) {
    var e = ui.declareCSS(s);
    var head = document.getElementsByTagName('head')[0];
    head.appendChild(e);
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
    return eval('(function evalscript() { try { \n' + s + '\n} catch(e) { debug(e.stack); throw e; }})();');
};

/**
 * Include a script.
 */
ui.includeScript = function(s) {
    //debug('include', s);
    return eval('try { \n' + s + '\n} catch(e) { debug(e.stack); throw e; }');
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
    if (ua.match(/iPhone/i) || ua.match(/iPad/i) || ua.match(/iPod/i) || ua.match(/Android/i) || ua.match(/Blackberry/i) || ua.match(/WebOs/i))
        ui.mobile = true;
    ui.mobiledetected = true;
    return ui.mobile;
};

/**
 * Return true if the client is Webkit based.
 */
ui.isWebkit = function() {
    return navigator.userAgent.match(/WebKit/i);
};

/**
 * Return the Webkit version.
 */
ui.webkitVersion = function() {
    return Number(navigator.userAgent.replace(/.*AppleWebKit\/(\d+\.\d+).*/, '$1'));
};

/**
 * Return true if the client is Firefox.
 */
ui.isFirefox = function() {
    return navigator.userAgent.match(/Firefox/i);
};

/**
 * Return the Firefox version.
 */
ui.firefoxVersion = function() {
    return Number(navigator.userAgent.replace(/.*Firefox\/(\d+\.\d+).*/, '$1'));
};

/**
 * Return true if the client is Safari.
 */
ui.isSafari = function() {
    return navigator.userAgent.match(/Safari/i);
};

/**
 * Return true if the client is Chrome.
 */
ui.isChrome = function() {
    return navigator.userAgent.match(/Chrome/i);
};

/**
 * Return true if the client is Internet Explorer.
 */
ui.isMSIE = function() {
    return navigator.userAgent.match(/MSIE/i);
};

/**
 * Return the Internet Explorer version.
 */
ui.msieVersion = function() {
    return Number(navigator.userAgent.replace(/.*MSIE (\d+\.\d+).*/, '$1'));
};

/**
 * Run a UI rendering function asynchronously.
 */
ui.asyncFrame = null;
ui.async = function(f) {
    if (isNull(ui.asyncFrame))
        // Use requestAnimationFrame when available, fallback to setTimeout
        ui.asyncFrame = window.requestAnimationFrame || window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame || window.oRequestAnimationFrame || window.msRequestAnimationFrame ||
            function(f) {
                return window.setTimeout(f, 16);
            };
    return ui.asyncFrame.call(window, f);
};

/**
 * Delay the execution of a function.
 */
ui.delayed = {}
ui.delay = function(f, t) {
    var id =  window.setTimeout(function() {
        delete ui.delayed[id];
        return f();
    }, isNull(t)? 16 : t);
    ui.delayed[id] = id;
    return id;
};

/**
 * Cancel the execution of a delayed function.
 */
ui.cancelDelay = function(id) {
    delete ui.delayed[id];
    return window.clearTimeout(id);
};

/**
 * Run a UI animation.
 */
ui.animationFrame = null;
ui.animation = function(f) {
    if (isNull(ui.animationFrame))
        // Use requestAnimationFrame when available, fallback to setInterval
        ui.animationFrame = window.requestAnimationFrame || window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame || window.oRequestAnimationFrame || window.msRequestAnimationFrame ||
            function(f) {
                if (!('interval' in f) || isNull(f.interval)) {
                    // First call, setup the interval
                    f.interval = window.setInterval(function animation() {
                        f.clearInterval = true;
                        try {
                            f();
                        } catch(ex) {}
                        // If the animation function didn't call ui.animation again to
                        // request another animation frame, clear the interval
                        if (f.clearInterval) {
                            f.clearInterval = false;
                            window.clearInterval(f.interval);
                            f.interval = null;
                        }
                    }, 16);
                } else {
                    // Called to request another animation frame, do not clear the
                    // interval
                    f.clearInterval = false;
                }
            };
    return ui.animationFrame.call(window, f);
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
 * Default page load behavior.
 */
ui.filler = null;
ui.onload = function() {

    // Add a filler div to make sure we can scroll
    if (ui.isMobile()) {
        ui.filler = document.createElement('div');
        ui.filler.id = 'filler';
        ui.filler.className = 'filler';
        ui.filler.style.height = ui.pixpos(window.orientation == 0? screen.height : screen.width * 2);
        document.body.appendChild(ui.filler);
    } else {
        // Style scroll bars
        var h = document.getElementsByTagName('html');
        if (!isNull(h))
            h[0].className = h[0].className? h[0].classname + ' flatscrollbars' : 'flatscrollbars';
    }

    // Scroll to hide the address bar
    document.body.style.display = 'block';
    window.scrollTo(0, 0);

    // Set unload handler
    window.onunload = function() {
        window.scrollTo(0, 0);
        return true;
    };

    return true;
};

/**
 * Default orientation change behavior.
 */
ui.onorientationchange = function(e) {

    // Adjust filler height
    if (!isNull(ui.filler))
        ui.filler.style.height = ui.pixpos(window.orientation == 0? screen.height : screen.width);

    // Scroll to hide the address bar
    window.scrollTo(0, 0);
    return true;
};

/**
 * Navigate to a new document.
 */
ui.navigate = function(url, win) {
    //debug('navigate', url, win);

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

        // Cleanup window event handlers
        window.onclick = null;
        if (!ui.isMobile()) {
            window.onmousedown = null;
            window.onmouseup = null;
            window.onmousemove = null;
        } else {
            window.ontouchstart = null;
            window.ontouchend = null;
            window.ontouchmove = null;
        }

        // Cleanup memoized element lookups
        ui.unmemo$();

        // Cancel any timers
        for (d in ui.delayed)
            ui.cancelDelay(d);

        // Navigate
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
        if (isNull(l))
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
};

/**
 * Convert a list of elements to an HTML single column table.
 */
ui.datalist = function(l) {

    function rows(l, i) {
        if (isNull(l))
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
};

/**
 * Read a file and convert it to a data url.
 */
ui.readfile = function(file, onerror, onprogress, onload) {
    var reader = new FileReader();
    reader.onerror = function(e) {
        return onerror();
    };
    reader.onprogress = function(e) {
        return onprogress(e.lengthComputable? Math.round((e.loaded / e.total) * 90) : 50);
    };
    reader.onload = function(r) {
        return onload(r.target.result);
    };
    return reader.readAsDataURL(file);
};

/**
 * Read an image url and convert it to a data url.
 */
ui.readimageurl = function(url, onerror, onprogress, onload, width, height) {
    // Create a canvas to draw the image
    var canvas = document.createElement('canvas');
    if (width)
        canvas.width = width;
    if (height)
        canvas.height = height;

    // Create an image
    var img = new Image();
    img.onerror = function(e) {
        return onerror();
    };
    img.onload = function() {
        // Draw the image
        var ctx = canvas.getContext('2d');
        if (width || height)
            ctx.drawImage(img, 0, 0, width, height);
        else
            ctx.drawImage(img, 0, 0);

        // Convert new canvas image to a data url
        return onload(canvas.toDataURL('image/png'));
    };

    // Load the image
    onprogress(90);
    img.src = url;
    return true;
};

/**
 * Read an image file or url and convert it to a data url.
 */
ui.readimage = function(img, onerror, onprogress, onload, width, height) {
    if (isString(img))
        return ui.readimageurl(img, onerror, onprogress, onload, width, height);
    return ui.readfile(img, onerror, onprogress, function(url) {
            return ui.readimageurl(url, onerror, onprogress, onload, width, height);
        }, width, height);
};

