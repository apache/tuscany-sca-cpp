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
ui.elementByID = function(id) {
    return document.getElementById(id);
};

/**
 * Return the current document, or a child element with the given id.
 */
function $(id) {
    if (id == document)
        return document;
    return document.getElementById(id);
}

/**
 * Remove ids from a tree of elements.
 */
ui.removeElementIDs = function(node) {
    function cleanIDs(node) {
        for(var i = 0; i < node.childNodes.length; i++) {
            var c = node.childNodes[i];
            if(c.nodeType == 1) {
                if (c.id != null)
                    c.id = null;
                cleanIDs(c);
            }
        }
        return true;
    }
    return cleanIDs(node);
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
ui.pathAndParams = function(url) {
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
    var qp = [];
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
    var qp = [];
    var qs = ui.fragment(url).split('&');
    for (var i = 0; i < qs.length; i++) {
        var e = qs[i].indexOf('=');
        if (e > 0)
            qp[qs[i].substring(0, e)] = unescape(qs[i].substring(e + 1));
    }
    return qp;
};

/**
 * Get a style property a DOM element.
 */
ui.getStyle = function(e, name) {
    return e.style.getPropertyValue(name);
};

/**
 * Set a style property of a DOM element.
 */
ui.setStyle = function(e, name, v, async) {
    if (e.style.getPropertyValue(name) == v)
        return false;
    if (!async)
        return e.style.setProperty(name, v, null);
    ui.render(function() {
        return e.style.setProperty(name, v, null);
    });
    return true;
};

/**
 * Remove a style property from a DOM element.
 */
ui.removeStyle = function(e, name, async) {
    if (!async)
        return e.style.removeProperty(name);
    ui.render(function() {
        return e.style.removeProperty(name);
    });
    return true;
};

/**
 * Set the CSS class of a DOM element.
 */
ui.getClass = function(e) {
    return e.className;
};

/**
 * Set the CSS class of a DOM element.
 */
ui.setClass = function(e, v, async) {
    if (e.className == v)
        return false;
    if (!async) {
        e.className = v;
        return true;
    }
    ui.render(function() {
        e.className = v;
    });
    return true;
};

/**
 * Convert a base64-encoded PNG image to a data URL.
 */
ui.b64PNG = function(b64) {
    return 'data:image/png;base64,' + b64.trim();
};

/**
 * Convert a base64-encoded JPEG image to a data URL.
 */
ui.b64JPEG = function(b64) {
    return 'data:image/jpeg;base64,' + b64.trim();
};

/**
 * Convert a data URL to a base64-encoded image.
 */
ui.imgB64 = function(img) {
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
    return nodeList(e.getElementsByTagName('script'));
};

/**
 * Evaluate a script.
 */
ui.evalScript = function(s) {
    return eval('(function evalscript() {\n' + s + '\n})();');
};

/**
 * Include a script.
 */
ui.includeScript = function(s) {
    debug('ui.include', s);
    return eval(s);
};

/**
 * Return true if the client is a mobile device.
 */
ui.mobileDetected = false;
ui.mobile = false;
ui.isMobile = function() {
    if (ui.mobileDetected)
        return ui.mobile;
    var ua = navigator.userAgent;
    if (ua.match(/iPhone/i) || ua.match(/iPad/i) || ua.match(/iPod/i) || ua.match(/Android/i) || ua.match(/Blackberry/i) || ua.match(/WebOs/i) || ua.match(/Mobile.*Firefox/i))
        ui.mobile = true;
    ui.mobileDetected = true;
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
 * Return true if the client is Android based.
 */
ui.isAndroid = function() {
    return navigator.userAgent.match(/Android/i);
};

/**
 * Return the Android version.
 */
ui.androidVersion = function() {
    return Number(navigator.userAgent.replace(/.*Version\/(\d+\.\d+).*/, '$1'));
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
 * Return the Safari version.
 */
ui.safariVersion = function() {
    return Number(navigator.userAgent.replace(/.*Version\/(\d+\.\d+).*/, '$1'));
};

/**
 * Return true if the client is Chrome.
 */
ui.isChrome = function() {
    return navigator.userAgent.match(/Chrome/i);
};

/**
 * Return the Chrome version.
 */
ui.chromeVersion = function() {
    return Number(navigator.userAgent.replace(/.*Chrome\/(\d+\.\d+).*/, '$1'));
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
 * Run a UI animation.
 */
ui.animationFrame = window.requestAnimationFrame || window.webkitRequestAnimationFrame || window.mozRequestAnimationFrame || window.msRequestAnimationFrame ||
    function(f) {
        if (isNull(f.interval)) {
            // First call, setup the interval
            f.interval = window.setInterval(function animation() {
                f.clearInterval = true;

                // Call the animation function
                f();

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

ui.animation = function(f) {
    return ui.animationFrame.call(window, f);
};

/**
 * Run a UI rendering function asynchronously.
 */
ui.render = function(f) {
    return ui.animation(f);
};

/**
 * Delay the execution of a function.
 */
ui.unimportant = {}
ui.delay = function(f, t, unimportant) {
    var id = window.setTimeout(function delayed() {
        delete ui.unimportant[id];
        return f();
    }, isNull(t)? 0 : t);
    if (unimportant)
        ui.unimportant[id] = id;
    return id;
};

/**
 * Cancel the execution of a delayed function.
 */
ui.cancelDelay = function(id) {
    delete ui.unimportant[id];
    return window.clearTimeout(id);
};

/**
 * Convert a CSS position to a numeric position.
 */
ui.npos = function(p) {
    return p == null || p == ''? 0 : Number(p.substr(0, p.length - 2));
};

/**
 * Convert a numeric position to a CSS pixel position.
 */
ui.pxpos = function(p) {
    return p + 'px';
};

/**
 * Show a status message.
 */
ui.statusElement = undefined;

ui.initStatus = function() {
    ui.statusElement = $('status');
    if (isNull(ui.statusElement))
        return;
    ui.setClass(ui.statusElement, ui.isMobile()? 'status3dm' : 'status3d');
    ui.setStyle(ui.statusElement, 'display', 'none');
    
    function divtransitionend(e) {
        ui.setClass(e.target, ui.isMobile()? 'status3dm' : 'status3d');
        ui.setStyle(e.target, 'display', 'none');
        e.target.error = false;
    }
    ui.statusElement.addEventListener('webkitTransitionEnd', divtransitionend, false);
    ui.statusElement.addEventListener('transitionend', divtransitionend, false);
    return true;
};

ui.status = function(s, c) {
    debug('ui.status', s);
    if(isNull(ui.statusElement) || ui.statusElement.error)
        return s;
    ui.statusElement.innerHTML = '<span class="' + (c? c : 'okstatus') + '">' + s + '</span>';
    ui.setClass(ui.statusElement, ui.isMobile()? 'status3dm' : 'status3d');
    ui.setStyle(ui.statusElement, 'display', 'block');
    ui.statusElement.error = c == 'errorstatus';
    if(ui.statusElement.delay)
        ui.cancelDelay(ui.statusElement.delay);
    ui.statusElement.delay = ui.delay(function hidestatus() {
        ui.setClass(ui.statusElement, ui.isMobile()? 'statusout3dm' : 'statusout3d');
        ui.statusElement.error = false;
    }, c == 'errorstatus'? 8000 : 3000);
    return s;
};

/**
 * Show an error message.
 */
ui.error = function(s) {
    debug('ui.error', s);
    return ui.status(s, 'errorstatus');
};

/**
 * Show the online/offline status.
 */
ui.onlineStatus = function() {
    return navigator.onLine? true : errorstatus('Offline');
};

/**
 * Show the working/ready indicator.
 */
ui.workingElement = undefined;
ui.initWorking = function() {
    ui.workingElement = $('working');
};

ui.working = function() {
    debug('ui.working');
    if (isNull(ui.workingElement))
        return false;
    return ui.setStyle(ui.workingElement, 'display', 'block');
};

ui.ready = function() {
    debug('ui.ready');
    if (isNull(ui.workingElement))
        return false;
    return ui.setStyle(ui.workingElement, 'display', 'none');
};

/**
 * Get and cache a resource.
 */
ui.appcache = {};
ui.appcache.get = function(uri, mode) {
    debug('ui.appcache.get', uri, mode);

    // Get resource from local storage first
    var h = uri.indexOf('#');
    var u = h == -1? uri : uri.substring(0, h);
    if(mode != 'remote') {
        var item = lstorage.getItem('ui.r.' + u);
        if(item != null && item != '')
            return item;
        if(mode == 'local')
            return undefined;
    }

    // Get resource from network
    var http = new XMLHttpRequest();
    http.open("GET", mode == 'remote'? (u + '?t=' + new Date().getTime() + '&r=' + Math.random()) : u, false);
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(null);
    if(http.status == 200) {
        var ct = http.getResponseHeader("Content-Type");
        if(http.responseText == '' || ct == null || ct == '') {
            error('http error', u, 'No-Content');
            return undefined;
        }
        lstorage.setItem('ui.r.' + u, http.responseText);
        return http.responseText;
    }
    error('http error', u, http.status, http.statusText);

    // Redirect to login page
    if(http.status == 403 && window.top.location.href.pathname != '/logout/dologout/') {
        if(window.onloginredirect)
            window.onloginredirect(new Error('403 ' + http.statusText));
    }
    return undefined;
};

/**
 * Remove a resource from the cache.
 */
ui.appcache.remove = function(uri) {
    debug('ui.appcache.remove', uri);
    var h = uri.indexOf('#');
    var u = h == -1? uri : uri.substring(0, h);
    return lstorage.removeItem(u);
};

/**
 * Default app cache handling behavior.
 */
ui.onappcache = function(manifest, resources) {

    if(ui.isMobile() && !ui.isFirefox()) {
        // On mobile devices, trigger usage of an application cache manifest
        // Except on mobile Firefox which fails to send cookies with cache manifest requests
        window.onappcachechecking = function(e) {
            debug('appcache checking', e);
            ui.working();
        };
        window.onappcacheerror = function(e) {
            debug('appcache error', e);
            ui.onlineStatus();
            ui.ready();
            return false;
        };
        window.onappcachenoupdate = function(e) {
            debug('appcache noupdate', e);
            ui.ready();
        };
        window.onappcachedownloading = function(e) {
            debug('appcache downloading', e);
            ui.working();
            ui.status('Updating');
        };
        window.onappcacheprogress = function(e) {
            debug('appcache progress', e);
            ui.working();
            ui.status('Updating');
        };
        window.onappcacheupdateready = function(e) {
            debug('appcache updateready', e);

            // Update offline resources in local storage and reload the page
            ui.status('Updating');
            applicationCache.swapCache();
            ui.delay(function swapappcache() {
                debug('appcache swapped', e);
                map(function(res) {
                    ui.appcache.remove(car(res));
                    ui.appcache.get(car(res), 'remote');
                }, resources);
                ui.status('Installed');
                ui.ready();

                debug('reloading');
                window.location.reload();
            });
        };
        window.onappcachecached = function(e) {
            debug('appcache cached', e);

            // Install offline resources in local storage
            ui.status('Installing');
            ui.delay(function installoffline() {
                map(function(res) {
                    ui.appcache.remove(car(res));
                    ui.appcache.get(car(res), 'remote');
                }, resources);
                ui.status('Installed');
                ui.ready();
            });
        };

        window.onloadappcache = function() {
            debug('appcache iframe loaded');
        };

        var installer = $('installer');
        installer.innerHTML = '<iframe src="' + manifest + '/" class="installer"></iframe>';

    } else {
        // On non-mobile devices, check for cache-manifest changes ourselves.
        ui.working();
        var lcmf = ui.appcache.get(manifest + '/cache-manifest.cmf', 'local');
        var rcmf = ui.appcache.get(manifest + '/cache-manifest.cmf', 'remote');
        if(lcmf == rcmf) {
            ui.ready();
            return true;
        }

        debug('cache-manifest changed, reloading');
        ui.status(isNull(lcmf)? 'Installing' : 'Updating');
        ui.delay(function reloadapp() {
            map(function(res) {
                ui.appcache.remove(car(res));
                ui.appcache.get(car(res), 'remote');
            }, resources);
            ui.ready();
            if(!isNull(lcmf)) {
                ui.status('Installed');
                ui.ready();

                debug('reloading');
                window.location.reload();
            }
        });
    }
};

/**
 * Default page load behavior.
 */
ui.filler = null;
ui.onload = function() {
    debug('ui.onload');

    // Initialize status and working elements
    ui.initStatus();
    ui.initWorking();

    // Set orientation change handler
    document.body.onorientationchange = function(e) {
        return ui.onorientationchange(e);
    };

    // Handle network offline/online events.
    window.addEventListener('offline', function(e) {
        debug('going offline');
        ui.status('Offline');
    }, false);
    window.addEventListener('online', function(e) {
        debug('going online');
        ui.status('Online');
    }, false);

    // Add a filler div to make sure we can scroll
    if (ui.isMobile()) {
        ui.filler = document.createElement('div');
        ui.filler.id = 'filler';
        ui.setClass(ui.filler, 'filler');
        ui.setStyle(ui.filler, 'height', ui.pxpos(window.orientation == 0? Math.floor(window.innerHeight * 1.5) : Math.floor(window.innerHeight * 1.5)));
        document.body.appendChild(ui.filler);
    } else {
        // Style scroll bars
        var h = nodeList(document.getElementsByTagName('html'));
        if (!isNull(h)) {
            ui.setClass(car(h), car(h).className? car(h).classname + ' flatscrollbars' : 'flatscrollbars');
        }
    }

    // Scroll to hide the address bar
    ui.setStyle(document.body, 'display', 'block');
    document.body.scrollTop = 0;

    // Set unload handler
    window.onunload = function() {
        document.body.scrollTop = 0;
        return true;
    };

    return true;
};

/**
 * Default orientation change behavior.
 */
ui.onorientationchange = function(e) {
    debug('ui.onorientationchange');

    // Adjust filler height
    if (!isNull(ui.filler))
        ui.setStyle(ui.filler, 'height', ui.pxpos(window.orientation == 0? Math.floor(window.innerHeight * 1.5) : Math.floor(window.innerHeight * 1.5)));

    // Scroll to refresh the page
    document.body.scrollTop = document.body.scrollTop + 1;
    document.body.scrollTop = document.body.scrollTop - 1;
    return true;
};

/**
 * Navigate to a new document.
 */
ui.navigate = function(url, win) {
    debug('ui.navigate', url, win);
    if (url == '' || url == '#')
        return false;

    function cleanup() {
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

        // Cancel any cancelable HTTP requests
        if (typeof HTTPBindingClient != 'undefined')
            HTTPBindingClient.cancelRequests();

        // Automatically cancel unimportant timers
        for (var d in ui.unimportant)
            ui.cancelDelay(d);
        return true;
    }

    // Open a new window
    if (win == '_blank') {
        debug('window.open', url, win);
        window.top.open(url, win);
        return false;
    }

    // Open a new document in the current window
    if (win == '_self') {
        cleanup();
        debug('window.open', url, win);
        window.top.open(url, win);
        return false;
    }

    // Reload the current window
    if (win == '_reload') {
        cleanup();
        debug('window.reload', url);
        window.top.location = url;
        window.top.location.reload();
        return false;
    }

    // Let the current top window handle the navigation
    if (win == '_view') {
        cleanup();

        if (!window.top.onnavigate) {
            debug('window.open', url, '_self');
            window.top.open(url, '_self');
            return false;
        }
        window.top.onnavigate(url);
        return false;
    }

    debug('window.open', url, win);
    window.top.open(url, win);
    return false;
}

/**
 * Bind a click handler to a widget.
 */
ui.ontouchstart = function(widget, e) {
    debug('ui.ontouchstart', widget.id);
    widget.down = true;
    widget.moved = false;
    var t = e.touches[0];
    widget.moveX = t.clientX;
    widget.moveY = t.clientY;
};

ui.ontouchmove = function(widget, e) {
    //debug('ontouchmove');
    var t = e.touches[0];
    if (t.clientX != widget.moveX) {
        widget.moveX = t.clientX;
        widget.moved = true;
    }
    if (t.clientY != widget.moveY) {
        widget.moveY = t.clientY;
        widget.moved = true;
    }
};

ui.ontouchend = function(widget, e) {
    debug('ui.ontouchend', widget.id);
    widget.down = false;
    if (!widget.moved) {
        e.preventDefault();
        debug('ui.fastonclick', widget.id);
        return widget.onclick(e);
    }
};

ui.onclick = function(widget, handler) {
    if (ui.isMobile()) {
        widget.ontouchstart = function(e) {
            return ui.ontouchstart(widget, e);
        };
        widget.ontouchmove = function(e) {
            return ui.ontouchmove(widget, e);
        };
        widget.ontouchend = function(e) {
            return ui.ontouchend(widget, e);
        };
    }
    widget.onclick = function(e) {
        debug('ui.onclick', widget.id);
        return handler(e);
    };
    return widget;
};

/**
 * Build a portable <a href> tag.
 */
ui.href = function(id, loc, target, clazz, html) {
    if (target == '_blank')
        return '<a href="' + loc + '" target="_blank"><span id="' + id + '" class="' + clazz + '">' + html + '</span></a>';
    return '<a href="' + loc + '" ' +
        (ui.isMobile()? 'ontouchstart="return ui.ontouchstart(this, event);" ontouchmove="return ui.ontouchmove(this, event);" ontouchend="return ui.ontouchend(this, event);" ' : '') +
        'onclick="return ui.navigate(\'' + loc + '\', \'' + target + '\');"><span id="' + id + '" class="' + clazz + '">' + html + '</span></a>';
};

/**
 * Update a <a href> tag.
 */
ui.updateHref = function(item, loc, target) {
    if (!isNull(loc) && !isNull(target)) {
        var link = item.parentNode;
        if (target == '_blank') {
            link.href = loc;
            link.target = '_blank';
        } else {
            link.href = loc;
            link.setAttribute('onclick', 'return ui.navigate(\'' + loc + '\', \'' + target + '\');');
        }
    }
    return item;
};

/**
 * Build a menu bar.
 */ 
ui.menuItem = function(id, name, href, target, hilight) {
    function Menu() {
        this.content = function() {
            if (hilight == true)
                return ui.href(id, href, target, 'tbarsmenu', name);
            else if (hilight == false)
                return ui.href(id, href, target, 'tbaramenu', name);
            else
                return ui.href(id, href, target, hilight, name);
        };
    }
    return new Menu();
};

ui.textItem = function(id, name, hilight) {
    function Item() {
        this.content = function() {
            return name;
        };
    }
    return new Item();
};

ui.menuFunc = function(id, name, fun, hilight) {
    function Menu() {
        this.content = function() {
            function href(id, fun, clazz, html) {
                return '<a href="/" ' +
                    (ui.isMobile()? 'ontouchstart="return ui.ontouchstart(this, event);" ontouchmove="return ui.ontouchmove(this, event);" ontouchend="return ui.ontouchend(this, event);" ' : '') +
                    'onclick="' + fun + '"><span id="' + id + '" class="' + clazz + '">' + html + '</span></a>';
            }

            if (hilight == true)
                return href(id, fun, 'tbarsmenu', name);
            else if (hilight == false)
                return href(id, fun, 'tbaramenu', name);
            else
                return href(id, fun, hilight, name);
        };
    }
    return new Menu();
};

ui.menuBar = function(left, center, right) {
    return '<span class="tbartitle">' +
        reduce(function(bar, item) {
            return bar + '<span class="tbarcenter">' + item.content() + '</span>';
        }, '', center) +
        '</span><span class="tbaritems">' +
        reduce(function(bar, item) {
            return bar + '<span class="tbarleft">' + item.content() + '</span>';
        }, '', left) +
        reduce(function(bar, item) {
            return bar + '<span class="tbarright">' + item.content() + '</span>';
        }, '', right) +
        '</span>';
};

ui.cmenuBar = function(items) {
    return reduce(function(bar, item) {
            return bar + '<span class="tbarcenter">' + item.content() + '</span>';
        }, '', items);
};
 
/**
 * Update a menu item.
 */
ui.updateMenuItem = function(item, name, href, target, hilight) {
    if (!isNull(name)) {
        if (item.innerHTML != name)
            item.innerHTML = name;
    }
    if (!isNull(hilight)) {
        if (hilight == true)
            ui.setClass(item, 'tbarsmenu');
        else if (hilight == false)
            ui.setClass(item, 'tbaramenu');
        else
            ui.setClass(item, hilight);
    }
    if (!isNull(href) && !isNull(target)) {
        var link = item.parentNode;
        if (target == '_blank') {
            link.href = href;
            link.target = '_blank';
        } else {
            link.href = href;
            link.setAttribute('onclick', 'return ui.navigate(\'' + href + '\', \'' + target + '\');');
        }
    }
    return item;
};

/**
 * Convert a list of elements to an HTML table.
 */
ui.dataTable = function(l) {
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
            if (!isList(v))
                return '<tr><td class="datatdl">' + indent(i) + elementName(e).substring(1) + '</td>' +
                    '<td class="datatdr tdw">' + (v != null? v : '') + '</td></tr>' + rows(cdr(l), i);
            return rows(expandElementValues(elementName(e), v), i) + rows(cdr(l), i);
        }

        // Generate table row for an element with children
        return '<tr><td class="datatdl">' + indent(i) + elementName(e).substring(1) + '</td>' +
            '<td class="datatdr tdw">' + '</td></tr>' + rows(elementChildren(e), i + 1) + rows(cdr(l), i);
    }
    return '<table class="datatable ' + (window.name == 'dataFrame'? ' databg' : '') + '" style="width: 100%;">' + rows(l, 0) + '</table>';
};

/**
 * Convert a list of elements to an HTML single column table.
 */
ui.dataList = function(l) {
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
            if (!isList(v))
                return '<tr><td class="datatd tdw">' + (v != null? v : '') + '</td></tr>' + rows(cdr(l), i);
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
ui.readFile = function(file, onerror, onprogress, onload) {
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
 * Draw an image on a canvas and convert it to a data URL.
 */
ui.drawImage = function(img, onload, width, height, crop) {
    // Rotate an image
    function rotate(icanvas, onload) {
        debug('ui.drawImage.rotate');
        var img = document.createElement('img');
        img.onload = function() {
            var canvas = document.createElement('canvas');
            canvas.width = icanvas.height;
            canvas.height = icanvas.width;
            var ctx = canvas.getContext('2d');
            ctx.setTransform(0, 1, -1, 0, icanvas.height, 0);
            ctx.drawImage(img, 0, 0, icanvas.width, icanvas.height);
            onload(canvas);
        };
        img.src = icanvas.toDataURL('image/png')
        return true;
    }

    // Draw the image on a canvas and convert it to a JPEG data URL
    function draw(img, onload, sx, sy, swidth, sheight, tx, ty, twidth, theight) {
        debug('ui.drawImage.draw', sx, sy, swidth, sheight, tx, ty, twidth, theight);
        var canvas = document.createElement('canvas');
        canvas.width = twidth;
        canvas.height = theight;
        var ctx = canvas.getContext('2d');
        ctx.drawImage(img, sx, sy, swidth, sheight, tx, ty, twidth, theight);
        return onload(canvas);
    }

    // Resize and optionally crop an image
    function resize(img, onload, width, height, oncrop) {
        debug('ui.drawImage.resize');
        var iwidth = img.width;
        var iheight = img.height;
        if (width || height || crop) {
            if (crop) {
                // Crop to fit target canvas size
                var tratio = width / height;
                var oratio = iwidth / iheight;
                if (tratio > oratio) {
                    var scale = width / iwidth;
                    var cwidth = iwidth;
                    var cheight = height / scale;
                    var cut = (iheight - cheight) / 2;
                    // Crop top and bottom edges, then resize
                    return draw(img, onload, 0, cut, cwidth, cut + cheight, 0, 0, width, height);
                } else if (tratio < oratio) {
                    var scale = height / iheight;
                    var cwidth = width / scale;
                    var cheight = iheight;
                    var cut = (iwidth - cwidth) / 2;
                    // Crop left and right edges, then resize
                    return draw(img, onload, cut, 0, cut + cwidth, cheight, 0, 0, width, height);
                } else {
                    // Just resize
                    return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, width, height);
                }
            } else {
                // Resize to make the image fit
                if (iwidth <= width && iheight == height) {
                    return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, iwidth, iheight);
                } else {
                    var tratio = width / height;
                    var oratio = iwidth / iheight;
                    if (tratio > oratio) {
                        // Resize to make height fit
                        var scale = height / iheight;
                        var swidth = iwidth * scale;
                        var sheight = height;
                        return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, swidth, sheight);
                    } else if (tratio < oratio) {
                        // Resize to make width fit
                        var scale = width / iwidth;
                        var swidth = width;
                        var sheight = iheight * scale;
                        return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, swidth, sheight);
                    } else {
                        // Resize to make both width and height fit
                        return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, width, height);
                    }
                }
            }
        } else {
            // Draw image as is
            return draw(img, onload, 0, 0, iwidth, iheight, 0, 0, iwidth, iheight);
        }
    }

    // Draw the image, optionally rotate, scale and crop it
    (function drawImage() {
        var iwidth = img.width;
        var iheight = img.height;
        document.body.removeChild(img);
        var nwidth = img.width;
        var nheight = img.height;
        debug('ui.drawImage', 'img.width', iwidth, 'img.height', iheight, 'nwidth', nwidth, 'nheight', nheight, 'width', width, 'height', height, 'crop', crop);

        if (iwidth != iheight && iwidth == nheight)
            // Rotate and resize the image
            return resize(img, function(canvas) {
                return rotate(canvas, function(canvas) {
                    return onload(canvas.toDataURL('image/jpeg', 0.95));
                });
            }, height, width, crop);
        else {
            // Just resize the image
            return resize(img, function(canvas) {
                return onload(canvas.toDataURL('image/jpeg', 0.95));
            }, width, height, crop);
        }
    })();
}

/**
 * Read an image url and convert it to a data url.
 */
ui.readImageURL = function(url, onerror, onprogress, onload, width, height, crop) {
    if(!width && !height && !crop && url.substring(0, 5) == 'data:') {
        // Just use the given data URL if we're not resizing the image
        debug('ui.readImageURL', 'original url');
        onprogress(90);
        ui.delay(function() {
            return onload(url);
        });
        return true;
    }

    // Create an image
    var img = document.createElement('img');
    ui.setStyle(img, 'visibility', 'hidden');
    document.body.appendChild(img);
    img.onerror = function(e) {
        document.body.removeChild(img);
        return onerror();
    };
    img.onload = function() {
        // Draw the image
        debug('ui.readImageURL', 'new data url');
        return ui.drawImage(img, onload, width, height, crop);
    };

    // Load the image
    onprogress(90);
    img.src = url;
    return true;
};

/**
 * Read an image file or url and convert it to a data url.
 */
ui.readImageFile = function(img, onerror, onprogress, onload, width, height, crop) {
    if (isString(img))
        return ui.readImageURL(img, onerror, onprogress, onload, width, height, crop);
    return ui.readFile(img, onerror, onprogress, function onfile(url) {
            return ui.readImageURL(url, onerror, onprogress, onload, width, height, crop);
        });
};

/**
 * Read an image and convert it to a data url.
 */
ui.readImage = function(img, onload, width, height, crop) {
    if(!width && !height && img.src.substring(0, 5) == 'data:') {
        // Just use the given data URL if we're not resizing the image
        return onload(img.src);
    }

    // Draw the image
    return ui.drawImage(img, onload, width, height, crop);
};

