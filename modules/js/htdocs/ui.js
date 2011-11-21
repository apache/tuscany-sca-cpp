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
    log('include', s);
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
 * Convert a host name to a home page title.
 */
ui.hometitle = function(host) {
    if (!isNil(window.top.config.hometitle))
        return window.top.config.hometitle;
    var h = reverse(host.split('.'));
    var d = isNil(cdr(h))? car(h) : cadr(h);
    return d.substr(0, 1).toUpperCase() + d.substr(1);
};

/**
 * Convert a host name to a window title.
 */
ui.windowtitle = function(host) {
    if (!isNil(window.top.config.windowtitle))
        return window.top.config.windowtitle;
    var h = reverse(host.split('.'));
    var d = isNil(cdr(h))? car(h) : cadr(h);
    return d.substr(0, 1).toUpperCase() + d.substr(1);
};

/**
 * Return true if the session cookie contains signin information.
 */
ui.signedin = function() {
    return !isNil(document.cookie) && document.cookie.indexOf('TuscanyOpenAuth=') != -1;
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
 * Reload the current document when orientation changes.
 */
ui.onorientationchange = function() {
    window.location.reload();
    return true;
}

/**
 * Navigate to a new document.
 */
ui.navigate = function(url, win) {
    log('navigate', url, win);

    // Open a new window
    if (win == '_blank')
        return window.top.open(url, win);

    // Open a new document in the current window
    if (win == '_self')
        return window.top.open(url, win);

    // Reload the current window
    if (win == '_reload') {
        window.top.location = url;
        return window.top.location.reload();
    }

    // Let the current top window handle the navigation
    if (win == '_view') {
        if (!window.top.onnavigate)
            return window.top.open(url, '_self');
        return window.top.onnavigate(url);
    }

    return window.top.open(url, win);
}

/**
 * Build a portable <a href> tag.
 */
ui.ahref = function(loc, target, html) {
    if (target == '_blank')
        return '<a href="' + loc + '" target="_blank">' + html + '</a>';
    return '<a href="javascript:void(0)" onclick="ui.navigate(\'' + loc + '\', \'' + target + '\');">' + html + '</a>';
};

/**
 * Build a menu bar.
 */ 
ui.menu = function(name, href, target, hilight) {
    function Menu() {
        this.content = function() {
            if (hilight)
                return ui.ahref(href, target, '<span class="tbarsmenu">' + name + '</span>');
            return ui.ahref(href, target, '<span class="tbaramenu">' + name + '</span>');
        };
    }
    return new Menu();
};

ui.menufunc = function(name, fun, hilight) {
    function Menu() {
        this.content = function() {
            function href(fun, html) {
                return '<a href="javascript:void(0)" onclick="' + fun + '">' + html + '</a>';
            }

            if (hilight)
                return href(fun, '<span class="tbarsmenu">' + name + '</span>');
            return href(fun, '<span class="tbaramenu">' + name + '</span>');
        };
    }
    return new Menu();
};

ui.menubar = function(left, right) {
    var bar = '<table cellpadding="0" cellspacing="0" width="100%" class="tbar"><tr>' +
    '<td class="dtbar"><table border="0" cellspacing="0" cellpadding="0"><tr>';
    for (i in left)
        bar = bar + '<td class="ltbar">' + left[i].content() + '</td>'

    bar = bar + '</tr></table></td>' +
    '<td class="dtbar"><table border="0" cellpadding="0" cellspacing="0" align="right"><tr>';
    for (i in right)
        bar = bar + '<td class="' + (i == 0? 'dtbar' : 'rtbar') + '">' + right[i].content() + '</td>'

    bar = bar + '</tr></table></td></tr></table>';
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

/**
 * Autocomplete / suggest support for input fields
 * To use it declare a 'suggest' function as follows:
 * function suggestItems() {
 *       return new Array('abc', 'def', 'ghi');
 * }
 * then hook it to an input field as follows:
 * suggest(document.yourForm.yourInputField, suggestItems);
 */ 
ui.selectSuggestion = function(node, value) {
    for (;;) {
        node = node.parentNode;
        if (node.tagName.toLowerCase() == 'div')
              break;
    }
    node.selectSuggestion(value);
};

ui.hilightSuggestion = function(node, over) {
    if (over)
        node.className = 'suggestHilighted';
    node.className = 'suggestItem';
};

ui.suggest = function(input, suggestFunction) {
    input.suggest = suggestFunction;
      
    input.selectSuggestion = function(value) {
        this.hideSuggestDiv();
        this.value = value;
    }
    
    input.hideSuggestDiv = function() {
        if (this.suggestDiv != null) {
            this.suggestDiv.style.visibility = 'hidden';
        }
    }
    
    input.showSuggestDiv = function() {
        if (this.suggestDiv == null) {
            this.suggestDiv = document.createElement('div');
            this.suggestDiv.input = this;
            this.suggestDiv.className = 'suggest';
            input.parentNode.insertBefore(this.suggestDiv, input);
            this.suggestDiv.style.visibility = 'hidden';
            this.suggestDiv.style.zIndex = '99';
            
            this.suggestDiv.selectSuggestion = function(value) {
                this.input.selectSuggestion(value);
            }
        }
        
        var values = this.suggest();
        var items = '';
        for (var i = 0; i < values.length; i++) {
            if (values[i].indexOf(this.value) == -1)
                continue;
            if (items.length == 0)
                items += '<table class="suggestTable">';
            items += '<tr><td class="suggestItem" ' +
            'onmouseover="ui.hilightSuggestion(this, true)" onmouseout="ui.hilightSuggestion(this, false)" ' +
            'onmousedown="ui.selectSuggestion(this, \'' + values[i] + '\')">' + values[i] + '</td></tr>';
        }
        if (items.length != 0)
            items += '</table>';
        this.suggestDiv.innerHTML = items;
        
        if (items.length != 0) {
            var node = input;              
            var left = 0;
            var top = 0;
            for (;;) {
                left += node.offsetLeft;
                top += node.offsetTop;
                node = node.offsetParent;
                if (node.tagName.toLowerCase() == 'body')
                  break;
            }
            this.suggestDiv.style.left = left;
            this.suggestDiv.style.top = top + input.offsetHeight;
            this.suggestDiv.style.visibility = 'visible';
        } else
            this.suggestDiv.style.visibility = 'hidden';
    }
      
    input.onkeydown = function(event) {
        this.showSuggestDiv();
    };

    input.onkeyup = function(event) {
        this.showSuggestDiv();
    };

    input.onmousedown = function(event) {
        this.showSuggestDiv();
    };

    input.onblur = function(event) {
        setTimeout(function() { input.hideSuggestDiv(); }, 50);
    };
};

