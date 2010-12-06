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

/**
 * Build a menu bar.
 */ 
function menu(name, href) {
    function Menu(n, h) {
        this.name = n;
        this.href = h;

        this.content = function() {
            function complete(uri) {
                if (uri.match('.*\.html$'))
                    return uri;
                if (uri.match('.*/$'))
                    return uri + 'index.html';
                return uri + '/index.html';
            }

            if (complete(this.href) != complete(window.top.location.pathname))
                return '<a href="' + this.href + '" target="_parent">' + this.name + '</a>';
            return '<span><b>' + this.name + '</b></span>';
        };
    }
    return new Menu(name, href);
}

function menubar(left, right) {
    var bar = '<table cellpadding="0" cellspacing="0" width="100%" class=tbar><tr>' +
    '<td class=ltbar><table border="0" cellspacing="0" cellpadding="0"><tr>';
    for (i in left)
        bar = bar + '<td class=ltbar>' + left[i].content() + '</td>'

    bar = bar + '</tr></table></td>' +
    '<td class=rtbar><table border="0" cellpadding="0" cellspacing="0" align="right"><tr>';
    for (i in right)
        bar = bar + '<td class=rtbar>' + right[i].content() + '</td>'

    bar = bar + '</tr></table></td></tr></table>';
    return bar;
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
function selectSuggestion(node, value) {
    for (;;) {
        node = node.parentNode;
        if (node.tagName.toLowerCase() == 'div')
              break;
    }
    node.selectSuggestion(value);
}

function hilightSuggestion(node, over) {
    if (over)
        node.className = 'suggestHilighted';
    node.className = 'suggestItem';
}

function suggest(input, suggestFunction) {
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
                items += '<table class=suggestTable>';
            items += '<tr><td class="suggestItem" ' +
            'onmouseover="hilightSuggestion(this, true)" onmouseout="hilightSuggestion(this, false)" ' +
            'onmousedown="selectSuggestion(this, \'' + values[i] + '\')">' + values[i] + '</td></tr>';
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
}

/**
 * Return the content document of a window.
 */
function content(win) {
    if (!isNil(win.document))
        return win.document;
    if (!isNil(win.contentDocument))
        return win.contentDocument;
    return null;
}

/**
 * Return a child element of a node with the given id.
 */
function elementByID(node, id) {
    for (var i in node.childNodes) {
        var child = node.childNodes[i];
        if (child.id == id)
            return child;
        var gchild = elementByID(child, id);
        if (gchild != null)
            return gchild;
    }
    return null;
}

/**
 * Return the current document, or a child element with the given id.
 */
function $(id) {
    if (id == document) {
        if (!isNil(document.widget))
            return widget;
        return document;
    }
    return elementByID($(document), id);
}

/**
 * Initialize a widget.
 */
function onloadwidget() {
    if (isNil(window.parent) || isNil(window.parent.widgets))
        return true;
    var pdoc = content(window.parent);
    for (w in window.parent.widgets) {
        var ww = elementByID(pdoc, w).contentWindow;
        if (ww == window) {
            document.widget = elementByID(pdoc, window.parent.widgets[w]);
            document.widget.innerHTML = document.body.innerHTML;
            return true;
        }
    }
    return true;
}

/**
 * Load a widget into an element.
 */
var widgets = new Array();

function bindwidget(f, el) {
    window.widgets[f] = el;
    return f;
}

