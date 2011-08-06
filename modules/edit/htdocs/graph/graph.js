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
 * SVG composite rendering functions.
 */

var graph = {};

/**
 * Basic colors
 */
graph.colors = {};
graph.colors.black = '#000000';
graph.colors.blue = '#0000ff';
graph.colors.cyan = '#00ffff';
graph.colors.gray = '#808080'
graph.colors.lightgray = '#dcdcdc'
graph.colors.green = '#00ff00';
graph.colors.magenta = '#ff00ff';
graph.colors.orange = '#ffa500';
graph.colors.pink = '#ffc0cb';
graph.colors.purple = '#800080';
graph.colors.red = '#ff0000';
graph.colors.white = '#ffffff';
graph.colors.yellow = '#ffff00';
graph.colors.link = '#598edd';

graph.colors.orange1 = '#ffd666';
graph.colors.green1 = '#bbe082';
graph.colors.blue1 = '#66dbdf';
graph.colors.yellow1 = '#fdf57a';
graph.colors.cyan1 = '#e6eafb';
graph.colors.lightgray1 = '#eaeaea'
graph.colors.pink1 = '#ffd9e0';
graph.colors.red1 = '#d03f41';
graph.colors.white1 = '#ffffff';

graph.colors.orange2 = '#ffbb00';
graph.colors.green2 = '#96d333';
//graph.colors.blue2 = '#0d7cc1';
graph.colors.blue2 = '#00c3c9';
graph.colors.red2 = '#d03f41';
graph.colors.yellow2 = '#fcee21';
graph.colors.magenta2 = '#c0688a';
graph.colors.cyan2 = '#d5dcf9';
graph.colors.lightgray2 = '#dcdcdc'
graph.colors.pink2 = '#ffc0cb';
graph.colors.white2 = '#ffffff';

graph.colors.orange3 = '#ffc700';
graph.colors.green3 = '#92e120';
graph.colors.blue3 = '#008fd1';
graph.colors.yellow3 = '#fdf400';
graph.colors.cyan3 = '#b4d3fd';
graph.colors.lightgray3 = '#e3e3e3'
graph.colors.pink3 = '#da749b';
graph.colors.red3 = '#ed3f48';
graph.colors.white3 = '#ffffff';

/**
 * Default positions and sizes.
 */
var palcx = 2500;
var proxcx = 20;
var proxcy = 20;
var buttoncx = 55;
var buttoncy = 23;
var curvsz = 4;
var tabsz = 2;
var titlex = 4;
var titley = 11;
var titlesp = 3;
var titlew = ui.isMobile()? -2 : 0;

/**
 * SVG rendering functions.
 */

graph.svgns='http://www.w3.org/2000/svg';

/**
 * Make an SVG graph.
 */
graph.mkgraph = function(cdiv, pos, cvalue, cadd, ccopy, cdelete) {

    // Create a div element to host the graph
    var div = document.createElement('div');
    div.id = 'svgdiv';
    div.style.position = 'absolute';
    div.style.left = ui.pixpos(pos.xpos() + cdiv.offsetLeft);
    div.style.top = ui.pixpos(pos.ypos() + cdiv.offsetTop);
    //div.style.overflow = 'hidden';
    cdiv.appendChild(div);

    // Create SVG element
    var svg = document.createElementNS(graph.svgns, 'svg');
    svg.style.height = ui.pixpos(5000);
    svg.style.width = ui.pixpos(5000);
    div.appendChild(svg);

    // Track element dragging and selection
    graph.dragging = null;
    graph.dragged = false;
    graph.moverenderer = null;
    graph.selected = null;
    cvalue.disabled = true;
    ccopy.disabled = true;
    cdelete.disabled = true;

    /**
     * Find the first draggable element in a hierarchy of elements.
     */
    function draggable(n) {
        if (n == div || n == svg || n == null)
            return null;
        if (n.nodeName == 'g' && !isNil(n.id) && n.id != '')
            return n;
        return draggable(n.parentNode);
    }

    /**
     * Handle a mouse down or touch start event.
     */
    function onmousedown(e) {

        // Remember mouse or touch position
        var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
        graph.downX = pos.screenX;
        graph.downY = pos.screenY;
        graph.moveX = pos.screenX;
        graph.moveY = pos.screenY;

        // Engage the click component selection right away
        // on mouse controlled devices
        if (typeof e.touches == 'undefined')
            onclick(e);

        // Find and remember draggable component
        var dragging = draggable(e.target);
        if (dragging == null || dragging != graph.selected)
            return true;
        graph.dragging = dragging;
        graph.dragged = false;

        // Remember current drag position
        graph.dragX = pos.screenX;
        graph.dragY = pos.screenY;

        e.preventDefault();
        return true;
    };

    if (!ui.isMobile()) {
        div.onmousedown = function(e) {
            //log('onmousedown');
            var suspend = svg.suspendRedraw(10);
            var r = onmousedown(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    } else {
        div.ontouchstart = function(e) {
            //log('ontouchstart');

            // Clear current move renderer if it's running
            if (!isNil(graph.moverenderer)) {
                clearInterval(graph.moverenderer);
                graph.moverenderer = null;
            }

            var suspend = svg.suspendRedraw(10);
            var r = onmousedown(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    }

    /**
     * Handle a mouse up or touch end event.
     */
    function onmouseup(e) {

        // Engage the click component selection now on touch devices
        if (ui.isMobile()) {
            if (!graph.dragged && graph.moveX == graph.downX && graph.moveY == graph.downY)
                return onclick(e);
        }

        // Stop here if the component was not dragged
        if (graph.dragging == null)
            return true;
        if (!graph.dragged) {
            graph.dragging = null;
            return true;
        }

        if (graph.dragging.parentNode == svg && graph.dragging.id.substring(0, 8) != 'palette:') {

            // Add new dragged component to the composite
            if (isNil(graph.dragging.compos)) {
                var compos = scdl.composite(svg.compos);
                setElement(compos, graph.sortcompos(graph.addcomps(mklist(graph.dragging.comp), compos)));
                graph.dragging.compos = svg.compos;
            }

            // Update component position
            setElement(graph.dragging.comp, graph.movecomp(graph.dragging.comp, graph.abspos(graph.dragging, svg)));

            // Wire component to neighboring reference
            if (!isNil(graph.dragging.svcpos)) {
                var compos = scdl.composite(svg.compos);
                setElement(compos, graph.sortcompos(graph.clonerefs(graph.wire(graph.dragging, compos, svg))));
            }

            // Snap top level component position to grid
            if (graph.dragging.parentNode == svg) {
                var gpos = graph.relpos(graph.dragging);
                setElement(graph.dragging.comp, graph.movecomp(graph.dragging.comp, graph.mkpath().pos(graph.gridsnap(gpos.xpos()), graph.gridsnap(gpos.ypos()))));
            }
        }

        // Forget current dragged component
        graph.dragging = null;
        graph.dragged = false;

        // Refresh the composite
        //log('onmouseup refresh');
        var nodes = graph.refresh(svg);

        // Reselected the previously selected component
        if (!isNil(graph.selected)) {
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);
        }

        // Trigger composite change event
        svg.oncomposchange(false);
        return true;
    };

    if (!ui.isMobile()) {
        div.onmouseup = function(e) {
            //log('onmouseup');
            var suspend = svg.suspendRedraw(10);
            var r = onmouseup(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    } else {
        div.ontouchend = function(e) {
            //log('ontouchend');

            // Clear current move renderer if it's running
            if (!isNil(graph.moverenderer)) {
                clearInterval(graph.moverenderer);
                graph.moverenderer = null;
            }

            var suspend = svg.suspendRedraw(10);
            var r = onmouseup(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    }

    /**
     * Handle a mouse or touch click event.
     */
    function onclick(e) {
        //log('onclick logic');

        // Find selected component
        var selected = draggable(e.target);
        if (selected == null) {
            if (graph.selected != null) {

                // Reset current selection
                graph.compselect(graph.selected, false, cvalue, ccopy, cdelete);
                graph.selected = null;

                // Trigger component select event
                svg.oncompselect(null);
            }

            // Dismiss the palette
            if (e.target == div || e.target == svg && ui.numpos(div.style.left) != (palcx * -1))
            	div.style.left = ui.pixpos(palcx * -1);

            return true;
        }

        // Ignore multiple click events
        if (selected == graph.selected)
            return true;
        if (selected.id.substring(0, 8) == 'palette:' && ui.numpos(div.style.left) != 0)
            return true;

        // Deselect previously selected component
        graph.compselect(graph.selected, false, cvalue, ccopy, cdelete);

        // Clone component from the palette
        if (selected.id.substring(0, 8) == 'palette:') {
            var compos = scdl.composite(svg.compos);
            var comp = graph.clonepalette(selected, compos, svg);
            setElement(compos, graph.sortcompos(graph.addcomps(mklist(comp), compos)));

            // Move into the editing area and hide the palette
            div.style.left = ui.pixpos(palcx * -1);

            // Refresh the composite
            //log('onclick refresh');
            var nodes = graph.refresh(svg);

            // Reselect the previously selected component
            graph.selected = graph.findcompnode(scdl.name(comp), nodes);
            graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);

            // Trigger composite change event
            svg.oncomposchange(true);

        } else {
            graph.selected = selected;

            // Select the component
            graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);
        }

        //log('comp selected');

        if (e.preventDefault)
            e.preventDefault();
        else
            e.returnValue = false;
        return true;
    }

    if (!ui.isMobile()) {
        div.onclick = function(e) {
            //log('div onclick');
            var suspend = svg.suspendRedraw(10);
            var r = onclick(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
        svg.onclick = function(e) {
            //log('svg onclick');
            var suspend = svg.suspendRedraw(10);
            var r = onclick(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    }

    /**
     * Handle a mouse or touch move event.
     */
    function onmousemove(e) {
        if (graph.dragging == null)
            return true;

        // Ignore duplicate  mouse move events
        if (graph.moveX == graph.dragX && graph.moveY == graph.dragY)
            return true;

        // Remember that the component was dragged
        graph.dragged = true;

        // Cut wire to component
        if (graph.dragging.parentNode != svg) {
            var compos = scdl.composite(svg.compos);
            setElement(compos, graph.sortcompos(graph.cutwire(graph.dragging, compos, svg)));

            // Bring component to the top
            graph.bringtotop(graph.dragging, svg);
        }

        // Calculate new position of dragged element
        var gpos = graph.relpos(graph.dragging);
        var newX = gpos.xpos() + (graph.moveX - graph.dragX);
        var newY = gpos.ypos() + (graph.moveY - graph.dragY);
        if (newX >= palcx)
            graph.dragX = graph.moveX
        else
            newX = palcx;
        if (newY >= 0)
            graph.dragY = graph.moveY;
        else
            newY = 0;

        // Detach child elements to speedup rendering
        graph.compoutline(graph.dragging, true);

        // Move the dragged element
        graph.move(graph.dragging, graph.mkpath().pos(newX, newY));

        return false;
    };

    if (!ui.isMobile()) {
        window.onmousemove = function(e) {
            //log('onmousemove');

            // Remember mouse position
            graph.moveX = e.screenX;
            graph.moveY = e.screenY;

            var suspend = svg.suspendRedraw(10);
            var r = onmousemove(e);
            svg.unsuspendRedraw(suspend);
            return r;
        }
    } else {
        div.ontouchmove = function(e) {
            //log('ontouchmove');
            
            // Remember touch position
            var pos = e.touches[0];
            if (graph.moveX == pos.screenX && graph.moveY == pos.screenY)
                return true;
            graph.moveX = pos.screenX;
            graph.moveY = pos.screenY;
            if (graph.moveX == graph.dragX && graph.moveY == graph.dragY)
                return true;

            // Start async move renderer
            if (graph.moverenderer == null) {
                graph.moverenderer = setInterval(function() {
                    var suspend = svg.suspendRedraw(10);
                    onmousemove(e);
                    svg.unsuspendRedraw(suspend);
                }, 10);
            }
            return true;
        }
    }

    /**
     * Handle field on change events.
     */
    function onvaluechange() {
        if (graph.selected == null)
            return false;
        if (g.parentNode.style.visibility == 'hidden')
            return false;

        // Change component name and refactor references to it
        function changename() {
            var compos = scdl.composite(svg.compos);
            cvalue.value = graph.ucid(cvalue.value, compos, false);
            graph.selected.id = cvalue.value;
            setElement(compos, graph.sortcompos(graph.renamecomp(graph.selected.comp, compos, cvalue.value)));

            // Refresh the composite
            //log('onchangename refresh');
            var nodes = graph.refresh(svg);

            // Reselected the previously selected component
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);

            // Trigger composite change event
            svg.oncomposchange(true);
            return false;
        }

        // Change the component property value
        function changeprop() {
            graph.setproperty(graph.selected.comp, cvalue.value);
            cvalue.value = graph.property(graph.selected.comp);
            cvalue.disabled = !graph.hasproperty(graph.selected.comp);

            // Refresh the composite
            //log('onchangeprop refresh');
            var nodes = graph.refresh(svg);

            // Reselected the previously selected component
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);

            // Trigger composite change event
            svg.oncomposchange(true);
            return false;
        }

        return graph.hasproperty(graph.selected.comp)? changeprop() : changename();
    };

    cvalue.onchange = function() {
        var suspend = svg.suspendRedraw(10);
        var r = onvaluechange();
        svg.unsuspendRedraw(suspend);
        return r;
    }
    
    // Handle delete event
    function ondeleteclick() {
        if (graph.selected == null)
            return false;
        if (graph.selected.id.substring(0, 8) != 'palette:') {

            // Remove selected component
            var compos = scdl.composite(svg.compos);
            if (isNil(graph.selected.compos))
                setElement(compos, graph.sortcompos(graph.cutwire(graph.selected, compos, svg)));
            setElement(compos, graph.sortcompos(graph.clonerefs(graph.gcollect(graph.removecomp(graph.selected.comp, compos)))));

            // Reset current selection
            graph.compselect(graph.selected, false, cvalue, ccopy, cdelete);
            graph.selected = null;

            // Refresh the composite
            //log('ondelete refresh');
            graph.refresh(svg);

            // Trigger component select event
            svg.oncompselect(null);

            // Trigger composite change event
            svg.oncomposchange(true);
        }
        return false;
    };

    cdelete.onclick = function() {
        var suspend = svg.suspendRedraw(10);
        var r = ondeleteclick();
        svg.unsuspendRedraw(suspend);
        return r;
    };

    // Handle copy event
    function oncopyclick() {
        if (graph.selected == null)
            return false;
        if (graph.selected.id.substring(0, 8) == 'palette:')
            return false;

        // Clone the selected component
        var compos = scdl.composite(svg.compos);
        var comps = graph.clonecomp(graph.selected, compos, svg);
        setElement(compos, graph.sortcompos(graph.addcomps(comps, compos)));

        // Refresh the composite
        //log('onclick refresh');
        var nodes = graph.refresh(svg);

        // Select the component clone
        graph.selected = graph.findcompnode(scdl.name(car(comps)), nodes);
        graph.compselect(graph.selected, true, cvalue, ccopy, cdelete);

        // Trigger component select event
        svg.oncompselect(graph.selected);

        // Trigger composite change event
        svg.oncomposchange(true);

        return false;
    };

    ccopy.onclick = function() {
        var suspend = svg.suspendRedraw(10);
        var r = oncopyclick();
        svg.unsuspendRedraw(suspend);
        return r;
    };

    // Handle add event
    cadd.onclick = function() {

        // Show the palette
        div.style.left = ui.pixpos(0);
        return false;
    };

    // Create a hidden SVG element to help compute the width
    // of component and reference titles
    graph.svgtitles = document.createElementNS(graph.svgns, 'svg');
    graph.svgtitles.style.visibility = 'hidden';
    graph.svgtitles.style.height = ui.pixpos(0);
    graph.svgtitles.style.width = ui.pixpos(0);
    div.appendChild(graph.svgtitles);

    return svg;
};

/**
 * Point class.
 */
graph.Point = function(x, y) {
    this.x = x;
    this.y = y;
};
graph.Point.prototype.xpos = function() {
    return this.x;
};
graph.Point.prototype.ypos = function() {
    return this.y;
};

graph.mkpoint = function(x, y) {
    return new graph.Point(x, y);
};

/**
 * Path class.
 */
graph.Path = function() {
    this.path = '';
    this.x = 0;
    this.y = 0;
}
graph.Path.prototype.pos = function(x, y) {
    this.x = x;
    this.y = y;
    return this;
};
graph.Path.prototype.xpos = function() {
    return this.x;
};
graph.Path.prototype.ypos = function() {
    return this.y;
};
graph.Path.prototype.rmove = function(x, y) {
    return this.move(this.x + x, this.y + y);
};
graph.Path.prototype.rline = function(x, y) {
    return this.line(this.x + x, this.y + y);
};
graph.Path.prototype.rcurve = function(x1, y1, x, y) {
    return this.curve(this.x + x1, this.y + y1, this.x + x1 + x, this.y + y1 + y);
};
graph.Path.prototype.str = function() {
    return this.path;
};
graph.Path.prototype.clone = function() {
    return graph.mkpath().pos(this.xpos(), this.ypos());
};
graph.Path.prototype.move = function(x, y) {
    this.path += 'M' + x + ',' + y + ' '; 
    return this.pos(x, y);
};
graph.Path.prototype.line = function(x, y) {
    this.path += 'L' + x + ',' + y + ' ';
    return this.pos(x, y);
};
graph.Path.prototype.curve = function(x1, y1, x, y) {
    this.path += 'Q' + x1 + ',' + y1 + ' ' + x + ',' + y + ' ';
    return this.pos(x, y);
};
graph.Path.prototype.end = function() {
    this.path += 'Z';
    return this;
};

graph.mkpath = function() {
    return new graph.Path();
};

/**
 * Return an element representing a title.
 */
graph.mktitle = function(t, x, y) {
    var title = document.createElementNS(graph.svgns, 'text');
    title.setAttribute('x', x);
    title.setAttribute('y', y);
    title.setAttribute('class', 'svgtitle');
    title.setAttribute('pointer-events', 'none');
    title.appendChild(document.createTextNode(t));
    graph.svgtitles.appendChild(title);
    return title;
};

/**
 * Return an element representing the title of a component.
 */
graph.comptitle = function(comp) {
    return memo(comp, 'title', function() {
    	var ct = graph.title(comp);
    	var pt = graph.propertytitle(comp);
    	if (ct == '' && pt == '')
    	    return null;
        return graph.mktitle((ct != '' && pt != '')? ct + ' ' + pt : ct + pt, titlex, titley);
    });
};

/**
 * Return the width of the title of a component.
 */
graph.comptitlewidth = function(comp) {
    var title = graph.comptitle(comp);
    if (isNil(title))
        return 0;
    return title.getBBox().width + titlew;
};

/**
 * Draw a component shape selection.
 */
graph.compselect = function(g, s, cvalue, ccopy, cdelete) {
    if (isNil(g) || !s) {
        cvalue.value = '';
        cvalue.disabled = true;
        ccopy.disabled = true;
        cdelete.disabled = true;
        if (isNil(g))
            return true;
        g.shape.setAttribute('stroke', graph.colors.gray);
        g.shape.setAttribute('stroke-width', '1');
        return true;
    }

    cvalue.value = graph.hasproperty(g.comp)? graph.property(g.comp) : g.id;
    cvalue.disabled = false;
    ccopy.disabled = false;
    cdelete.disabled = false;

    g.shape.setAttribute('stroke', graph.colors.link);
    g.shape.setAttribute('stroke-width', '2');
    g.parentNode.appendChild(g);
    return true;
};

/**
 * Draw a palette shape selection.
 */
graph.paletteselect = function(g, s) {
    if (isNil(g))
        return true;
    if (!s) {
        g.shape.setAttribute('stroke', graph.colors.gray);
        g.shape.setAttribute('stroke-width', '1');
        return true;
    }

    g.shape.setAttribute('stroke', graph.colors.link);
    g.shape.setAttribute('stroke-width', '2');
    g.parentNode.appendChild(g);
    return true;
};

/**
 * Draw a component outline for faster rendering.
 */
graph.compoutline = function(g, s) {
    if (s == (isNil(g.outlined)? false : g.outlined))
        return true;
    g.outlined = s;

    if (s) {
        g.shape.setAttribute('fill', 'none');
        if (!isNil(g.title))
            g.removeChild(g.title);
    } else {
        g.shape.setAttribute('fill', graph.color(g.comp));
        if (!isNil(g.title))
            g.appendChild(g.title);
    }

    map(function(r) {
            var n = caddr(r);
            if (isNil(n))
                return r;
            graph.compoutline(n, s);
            return r;
        }, g.refpos);
    return true;
};

/**
 * Return a node representing a component.
 */
graph.compnode = function(comp, cassoc, pos, parentg) {

    // Make the component title element
    var title = graph.comptitle(comp);

    // Compute the path of the component shape
    var path = graph.comppath(comp, cassoc);

    // Create the main component shape
    var shape = document.createElementNS(graph.svgns, 'path');
    shape.setAttribute('d', path.str());
    shape.setAttribute('fill', graph.color(comp));
    //shape.setAttribute('fill-opacity', '0.6');
    shape.setAttribute('stroke', graph.colors.gray);
    shape.setAttribute('stroke-width', '1');
    shape.setAttribute('pointer-events', 'visible');

    // Create an svg group and add the shape and title to it
    var g = document.createElementNS(graph.svgns, 'g');
    g.comp = comp;
    g.id = scdl.name(comp);
    g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    g.pos = pos.clone();
    g.appendChild(shape);
    g.shape = shape;
    if (!isNil(title)) {
        g.appendChild(title);
        g.title = title;
    }

    // Store the the positions of the services and references
    g.refpos = reverse(path.refpos);
    g.svcpos = reverse(path.svcpos);

    // Handle onclick events
    g.onclick = parentg.onclick;

    return g;
};

/**
 * Find the node representing a component.
 */
graph.findcompnode = function(name, nodes) {
    if (isNil(nodes))
        return null;
    if (isNil(car(nodes).comp))
        return graph.findcompnode(name, cdr(nodes));
    if (name == scdl.name(car(nodes).comp))
        return car(nodes);
    var node = graph.findcompnode(name, nodeList(car(nodes).childNodes));
    if (!isNil(node))
        return node;
    return graph.findcompnode(name, cdr(nodes));
}

/**
 * Return a graphical group.
 */
graph.mkgroup = function(pos) {
    var g = document.createElementNS(graph.svgns, 'g');
    g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    g.pos = pos.clone();
    return g;
};

/**
 * Return a node representing a button.
 */
graph.mkbutton = function(t, pos) {

    // Make the button title
    var title = graph.mktitle(t, titlex, titley);

    // Compute the path of the button shape
    var path = graph.buttonpath().str();

    // Create the main button shape
    var shape = document.createElementNS(graph.svgns, 'path');
    shape.setAttribute('d', path);
    shape.setAttribute('fill', graph.colors.lightgray1);
    //shape.setAttribute('fill-opacity', '0.6');
    shape.setAttribute('stroke', graph.colors.gray);
    shape.setAttribute('stroke-width', '1');
    shape.setAttribute('pointer-events', 'visible');

    // Create a group and add the button shape to it
    var g = document.createElementNS(graph.svgns, 'g');
    g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    g.pos = pos.clone();
    g.appendChild(shape);
    g.appendChild(title);

    // Store the button shape in the group
    g.shape = shape;

    return g;
};

/**
 * Return the relative position of a node.
 */
graph.relpos = function(e) {
    var pmatrix = e.parentNode != null? e.parentNode.getCTM() : null;
    var matrix = e.getCTM();
    var curX = pmatrix != null? (Number(matrix.e) - Number(pmatrix.e)): Number(matrix.e);
    var curY = pmatrix != null? (Number(matrix.f) - Number(pmatrix.f)): Number(matrix.f);
    return graph.mkpath().pos(curX, curY);
};

/**
 * Move a node.
 */
graph.move = function(e, pos) {
    e.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    e.pos = pos.clone();
};

/**
 * Return the absolute position of a component node.
 */
graph.abspos = function(e, g) {
    if (e == g)
        return graph.mkpath();
    var gpos = graph.relpos(e);
    var pgpos = graph.abspos(e.parentNode, g);
    return graph.mkpath().pos(gpos.xpos() + pgpos.xpos(), gpos.ypos() + pgpos.ypos());
};

/**
 * Bring a component node to the top.
 */
graph.bringtotop = function(n, g) {
    if (n == g)
        return null;
    graph.move(n, graph.abspos(n, g));
    g.appendChild(n);
}

/**
 * Return the title of a SCDL element.
 */
graph.title = function(e) {
    var t = scdl.title(e);
    if (t != null) {
        if (t == 'gt')
            return '>'
        if (t == 'lt')
            return '<';
        if (t.indexOf('{propval}') != -1)
            return '';
        if (t.indexOf('{compname}') == -1)
            return t;
        return t.replace('{compname}', scdl.name(e));
    }
    return scdl.name(e);
};

/**
 * Return the property value of a SCDL component.
 */
graph.property = function(e) {
    var p = scdl.properties(e);
    if (isNil(p))
        return '';
    if (scdl.visible(car(p)) == 'false')
        return '';
    var pv = scdl.propertyValue(car(p));
    return pv;
};

/**
 * Return the title of a property of a SCDL component.
 */
graph.propertytitle = function(e) {
    var pv = graph.property(e);
    var t = scdl.title(e);
    if (t.indexOf('{propval}') == -1)
        return pv;
    return t[0] == ' '? t.substr(1).replace('{propval}', pv) : t.replace('{propval}', pv);
};

/**
 * Return true if a SCDL component has a property.
 */
graph.hasproperty = function(e) {
    var p = scdl.properties(e);
    if (isNil(p))
        return false;
    if (scdl.visible(car(p)) == 'false')
        return false;
    return true;
};

/**
 * Change the property value of a SCDL component.
 */
graph.setproperty = function(e, value) {
    var p = scdl.properties(e);
    if (isNil(p))
        return '';
    if (scdl.visible(car(p)) == 'false')
        return '';
    var name = scdl.name(car(p));
    setElement(car(p), mklist(element, "'property", mklist(attribute, "'name", name != null? name : "property"), value));
    return value;
};

/**
 * Return the color of a SCDL component.
 */
graph.color = function(comp) {
    return memo(comp, 'color', function() {
        var c = scdl.color(comp);
        return c == null? graph.colors.blue1 : graph.colors[c];
    });
};

/**
 * Return the services on the left side of a component.
 */
graph.lsvcs = function(comp) {
    return memo(comp, 'lsvcs', function() {
        var svcs = scdl.services(comp);
        if (isNil(svcs))
            return mklist(mklist("'element","'service","'attribute","'name",scdl.name(comp)));
        var l = filter(function(s) {
                var a = scdl.align(s);
                var v = scdl.visible(s);
                return (a == null || a == 'left') && v != 'false';
            }, svcs);
        if (isNil(l))
            return mklist();
        return mklist(car(l));
    });
};

/**
 * Return the references on the right side of a component.
 */
graph.rrefs = function(comp) {
    return memo(comp, 'rrefs', function() {
        return filter(function(r) {
            var a = scdl.align(r);
            var v = scdl.visible(r);
            return (a == null || a == 'right') && v != 'false';
        }, scdl.references(comp));
    });
};

/**
 * Return the height of a reference on the right side of a component.
 */
graph.rrefheight = function(ref, cassoc) {
    return memo(ref, 'rheight', function() {
        var target = assoc(scdl.target(ref), cassoc);
        if (isNil(target))
            return tabsz * 8;
        return graph.compclosureheight(cadr(target), cassoc);
    });
};

/**
 * Return the total height of the references on the right side of a component.
 */
graph.rrefsheight = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return graph.rrefheight(car(refs), cassoc) + graph.rrefsheight(cdr(refs), cassoc);
};

/**
 * Return the height of a component node.
 */
graph.compheight = function(comp, cassoc) {
    return memo(comp, 'height', function() {
        var lsvcs = graph.lsvcs(comp);
        var lsvcsh = Math.max(1, length(lsvcs)) * (tabsz * 8) + (tabsz * 4);
        var rrefs = graph.rrefs(comp);
        var rrefsh = graph.rrefsheight(rrefs, cassoc) + (tabsz * 2);
        return Math.max(lsvcsh, rrefsh);
    });
};

/**
 * Return the height of a component and the components wired to its bottom side.
 */
graph.compclosureheight = function(comp, cassoc) {
    return memo(comp, 'closureheight', function() {
        return graph.compheight(comp, cassoc);
    });
};

/**
 * Return the max width of the references on the right side of a component.
 */
graph.rrefswidth = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return Math.max(graph.rrefwidth(car(refs), cassoc), graph.rrefswidth(cdr(refs), cassoc));
};

/**
 * Return the width of a component.
 */
graph.compwidth = function(comp, cassoc) {
    return memo(comp, 'width', function() {
        var ctw = graph.comptitlewidth(comp);
        var rrefsw = (isNil(graph.rrefs(comp))? 0 : (tabsz * 4));
        var twidth = (titlex * 2) + ctw + rrefsw;
        var width = Math.max(twidth, (tabsz * 8) + (tabsz * 4));
        return width;
    });
};

/**
 * Return a path representing a reference positioned to the right of a component.
 */
graph.rrefpath = function(ref, cassoc, path, maxheight) {
    var height = graph.rrefheight(ref, cassoc);

    // Record reference position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.refpos = cons(mklist(ref, graph.mkpath().pos(xpos, ypos + (tabsz * 5))), path.refpos);

    // Compute the reference path
    return path.rline(0,tabsz * 2).rcurve(0,tabsz,-tabsz,0).rcurve(-tabsz,0,0,-tabsz/2.0).rcurve(0,-tabsz/2.0,-tabsz,0).rcurve(-tabsz,0,0,tabsz/2.0).rline(0,tabsz * 3).rcurve(0,tabsz/2.0,tabsz,0).rcurve(tabsz,0,0,-tabsz/2.0).rcurve(0,-tabsz/2.0,tabsz,0).rcurve(tabsz,0,0,tabsz).line(path.xpos(), Math.min(ypos + height, maxheight));
};

/**
 * Return a path representing a service positioned to the left of a component.
 */
graph.lsvcpath = function(svc, cassoc, path, minheight) {
    var height = tabsz * 8;

    // Record service position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.svcpos = cons(mklist(svc, graph.mkpath().pos(xpos, ypos - (tabsz * 6))), path.svcpos);

    // Compute the service path
    return path.rline(0, -(tabsz * 2)).rcurve(0,-tabsz,-tabsz,0).rcurve(-tabsz,0,0,tabsz/2.0).rcurve(0,tabsz/2.0,-tabsz,0).rcurve(-tabsz,0,0,-tabsz/2.0).rline(0,-(tabsz * 3)).rcurve(0,-tabsz/2.0,tabsz,0).rcurve(tabsz,0,0,tabsz/2.0).rcurve(0,tabsz/2.0,tabsz,0).rcurve(tabsz,0,0,-tabsz).line(path.xpos(), Math.max(ypos - height, minheight));
};

/**
 * Return a path representing a component node.
 */
graph.comppath = function(comp, cassoc) {

    // Calculate the width and height of the component node
    var width = graph.compwidth(comp, cassoc);
    var height = graph.compheight(comp, cassoc);

    /**
     * Apply a path rendering function to a list of services or references.
     */
    function renderpath(x, f, cassoc, path, height) {
        if (isNil(x))
            return path;
        return renderpath(cdr(x), f, cassoc, f(car(x), cassoc, path, height), height);
    }

    var path = graph.mkpath().move(curvsz,0);

    // Store the positions of services and references in the path
    path.refpos = mklist();
    path.svcpos = mklist();

    // Render the references on the right side of the component
    var rrefs = graph.rrefs(comp);
    path = path.line(width - curvsz,path.ypos()).rcurve(curvsz,0,0,curvsz);
    path = renderpath(rrefs, graph.rrefpath, cassoc, path, height - curvsz);

    // Render the references on the bottom side of the component
    var boffset = curvsz;
    path = path.line(path.xpos(),height - curvsz).rcurve(0,curvsz,curvsz * -1,0).line(boffset, path.ypos());

    // Render the services on the left side of the component
    var lsvcs = graph.lsvcs(comp);
    var loffset = curvsz + (length(lsvcs) * (tabsz * 8));
    path = path.line(curvsz,path.ypos()).rcurve(curvsz * -1,0,0,curvsz * -1).line(path.xpos(), loffset);
    path = renderpath(lsvcs, graph.lsvcpath, cassoc, path, curvsz);

    // Close the component node path
    path = path.line(0,curvsz).rcurve(0,curvsz * -1,curvsz,0);

    return path.end();
};

/**
 * Return the position of a component.
 */
graph.comppos = function(comp, pos) {
    var x = scdl.x(comp);
    var y = scdl.y(comp);
    return graph.mkpath().pos(x != null? Number(x) + palcx : pos.xpos(), y != null? Number(y) : pos.ypos());
};

/**
 * Return a path representing a button node.
 */
graph.buttonpath = function(t) {
    var path = graph.mkpath().move(curvsz,0);
    path = path.line(buttoncx - curvsz,path.ypos()).rcurve(curvsz,0,0,curvsz);
    path = path.line(path.xpos(),buttoncy - curvsz).rcurve(0,curvsz,-curvsz,0).line(curvsz, path.ypos());
    path = path.line(curvsz,path.ypos()).rcurve(-curvsz,0,0,-curvsz).line(path.xpos(), curvsz);
    path = path.line(0,curvsz).rcurve(0,-curvsz,curvsz,0);
    return path.end();
};

/**
 * Render a SCDL composite into a list of component nodes.
 */
graph.composite = function(compos, pos, aspalette, g) {
    var name = scdl.name(scdl.composite(compos));
    var comps = scdl.components(compos);
    var cassoc = scdl.nameToElementAssoc(comps);
    var proms = scdl.promotions(compos);

    // Unmemoize any memoized info about components and their references.
    map(function(c) {
            unmemo(c);
            map(function(r) {
                    unmemo(r);
                }, scdl.references(c));
        }, comps);

    /**
     * Render a component.
     */
    function rendercomp(comp, cassoc, pos) {

        /**
         * Render the references on the right side of a component.
         */
        function renderrrefs(refs, cassoc, pos, gcomp) {

            /**
             * Render a reference on the right side of a component.
             */
            function renderrref(ref, cassoc, pos, gcomp) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return null;

                // Render the component target of the reference
                return rendercomp(cadr(target), cassoc, pos);
            }

            /**
             * Move the rendering cursor down below a reference.
             */
            function rendermove(ref, cassoc, pos) {
                return pos.clone().rmove(0, graph.rrefheight(ref, cassoc));
            }

            if (isNil(refs))
                return mklist();

            // Return list of (ref, comp rendering) pairs
            var grefcomp = renderrref(car(refs), cassoc, pos, gcomp);
            return cons(mklist(car(refs), grefcomp), renderrrefs(cdr(refs), cassoc, rendermove(car(refs), cassoc, pos), gcomp));
        }

        // Compute the component shape
        var gcomp = graph.compnode(comp, cassoc, pos, g);

        // Render the components wired to the component references
        var rrefs = graph.rrefs(comp);
        var rpos = graph.mkpath().rmove(graph.compwidth(comp, cassoc), 0);
        var grrefs = renderrrefs(rrefs, cassoc, rpos, gcomp);

        // Store list of (ref, pos, component rendering) triplets in the component
        function refposgcomp(refpos, grefs) {
            if (isNil(refpos))
                return mklist();

            // Append component rendering to component
            var gref = cadr(car(grefs));
            if (gref != null)
                gcomp.appendChild(gref);
            return cons(mklist(car(car(refpos)), cadr(car(refpos)), gref), refposgcomp(cdr(refpos), cdr(grefs)));
        }

        gcomp.refpos = refposgcomp(gcomp.refpos, grrefs);

        return gcomp;
    }

    /**
     * Render a list of promoted service components.
     */
    function renderproms(svcs, cassoc, pos) {

        /**
         * Return the component promoted by a service.
         */
        function promcomp(svc, cassoc) {
            var c = assoc(scdl.promote(svc), cassoc);
            if (isNil(c))
                return mklist();
            return cadr(c);
        }

        /**
         * Move the rendering cursor down below a component.
         */
        function rendermove(comp, cassoc, pos) {
            return pos.clone().rmove(0, graph.compclosureheight(comp, cassoc) + Math.max((tabsz * 2), 8));
        }

        if (isNil(svcs))
            return mklist();

        // Render the first promoted component in the list
        // then recurse to render the rest of the list
        var comp = promcomp(car(svcs), cassoc);
        if (isNil(comp))
            return renderproms(cdr(svcs), cassoc, rendermove(car(svcs), cassoc, pos));

        var cpos = graph.comppos(comp, pos);
        return cons(rendercomp(comp, cassoc, cpos), renderproms(cdr(svcs), cassoc, rendermove(comp, cassoc, cpos)));
    }

    // Render the promoted service components
    var rproms = renderproms(proms, cassoc, pos.clone().rmove(tabsz * 4, tabsz * 4));

    if (aspalette) {

        // Prefix ids of palette component elements with 'palette:' and
        // move them to the palette area
        return map(function(r) {
                r.id = 'palette:' + r.id;
                var gpos = r.pos;
                graph.move(r, graph.mkpath().pos(gpos.xpos() - palcx, gpos.ypos()));
                return r;
            }, rproms);

    } else {

        // Link app component elements to the containing composite
        return map(function(r) { r.compos = compos; return r; }, rproms);
    }
};

/**
 * Return a component unique id.
 */
graph.ucid = function(prefix, compos1, compos2, clone) {

    // Build an assoc list keyed by component name
    var comps = map(function(c) { return mklist(scdl.name(c), c); }, append(namedElementChildren("'component", compos1), namedElementChildren("'component", compos2)));

    if (!clone && isNil(assoc(prefix, comps)))
        return prefix;

    /**
     * Find a free component id.
     */
    function ucid(p, id) {
        if (isNil(assoc(p + id, comps)))
            return p + id;
        return ucid(p, id + 1);
    }

    /**
     * Remove trailing digits from a prefix.
     */
    function untrail(p) { 
        if (p.length < 2 || p[p.length - 1] < '0' || p[p.length - 1] > '9')
            return p;
        return untrail(p.substring(0, p.length - 1));
    }

    return ucid(prefix == ''? 'comp' : (clone? untrail(prefix) : prefix), 1);
};

/**
 * Clone a palette component node.
 */
graph.clonepalette = function(e, compos, g) {

    // Clone the SCDL component and give it a unique name
    var wcomp = append(mklist(element, "'component", mklist(attribute, "'name", graph.ucid(scdl.name(e.comp), compos, compos, true))),
                filter(function(c) { return !(isAttribute(c) && attributeName(c) == "'name")}, elementChildren(e.comp)));
    var x = '<composite>' + writeXML(mklist(wcomp), false) + '</composite>';
    var rcompos = scdl.composite(readXML(mklist(x)));
    var comp = car(scdl.components(mklist(rcompos)));

    // Update component position
    setElement(comp, graph.movecomp(comp, graph.abspos(e, g).rmove(palcx, 0)));

    return comp;
};

/**
 * Move a SCDL component to the given position.
 */
graph.movecomp = function(comp, pos) {
    if (isNil(pos))
        return append(mklist(element, "'component"),
                filter(function(e) { return !(isAttribute(e) && (attributeName(e) == "'x" || attributeName(e) == "'y")); }, elementChildren(comp)));
    return append(mklist(element, "'component", mklist(attribute, "'x", '' + (pos.xpos() - palcx)), mklist(attribute, "'y", '' + pos.ypos())),
            filter(function(e) { return !(isAttribute(e) && (attributeName(e) == "'x" || attributeName(e) == "'y")); }, elementChildren(comp)));
};

/**
 * Align a pos along a 10pixel grid.
 */
graph.gridsnap = function(x) {
    return Math.round(x / 10) * 10;
}

/**
 * Clone a component node and all the components it references.
 */
graph.clonecomp = function(e, compos, g) {

    // Write the component and the components it references to XML
    function collectcomp(e) {
        function collectrefs(refpos) {
            if (isNil(refpos))
                return mklist();
            var r = car(refpos);
            var n = caddr(r);
            if (isNil(n))
                return collectrefs(cdr(refpos));
            return append(collectcomp(n), collectrefs(cdr(refpos)));
        }

        return cons(e.comp, collectrefs(e.refpos));
    }

    var allcomps = collectcomp(e);
    var ls = map(function(e) { return writeXML(mklist(e), false); }, allcomps);
    var x = '<composite>' + writeStrings(ls) + '</composite>';

    // Read them back from XML to clone them
    var rcompos = scdl.composite(readXML(mklist(x)));
    var comps = scdl.components(mklist(rcompos));

    // Give them new unique names
    map(function(e) {

        // Rename each component
        var oname = scdl.name(e);
        var name = graph.ucid(oname, compos, rcompos, true);
        setElement(e, append(mklist(element, "'component", mklist(attribute, "'name", name)),
                        filter(function(c) { return !(isAttribute(c) && attributeName(c) == "'name")}, elementChildren(e))));

        // Refactor references to the component
        map(function(c) { return graph.refactorrefs(scdl.references(c), oname, name); }, comps);
    }, comps);

    // Update the top component position
    var comp = car(comps);
    setElement(comp, graph.movecomp(comp, graph.abspos(e, g).rmove(10, 10)));

    return comps;
};

/**
 * Sort elements of a composite.
 */
graph.sortcompos = function(compos) {
    return append(mklist(element, "'composite"), elementChildren(compos).sort(function(a, b) {

        // Sort attributes, place them at the top
        var aa = isAttribute(a);
        var ba = isAttribute(b);
        if (aa && !ba) return -1;
        if (!aa && ba) return 1;
        if (aa && ba) {
            var aan = attributeName(a);
            var ban = attributeName(b);
            if (aan < ban) return -1;
            if (aan > ban) return 1;
            return 0;
        }

        // Sort elements, place services before components
        var aen = elementName(a);
        var ben = elementName(b);
        if (aen == "'service" && ben == "'component") return -1;
        if (aen == "'component" && ben == "'service") return 1;
        var an = scdl.name(a);
        var bn = scdl.name(b);
        if (an < bn) return -1;
        if (an > bn) return 1;
        return 0;
    }));
}

/**
 * Add a list of components to a SCDL composite. The first
 * component in the list is a promoted component.
 */
graph.addcomps = function(comps, compos) {
    var comp = car(comps);
    var name = scdl.name(comp);
    var prom = mklist(element, "'service", mklist(attribute, "'name", name), mklist(attribute, "'promote", name));
    return append(mklist(element, "'composite"), append(elementChildren(compos), cons(prom, comps)));
};

/**
 * Remove a component from a SCDL composite.
 */
graph.removecomp = function(comp, compos) {
    var name = scdl.name(comp);
    return append(mklist(element, "'composite"),
            filter(function(c) { return !(isElement(c) && scdl.name(c) == name); }, elementChildren(compos)));
};

/**
 * Garbage collect components not referenced or promoted.
 */
graph.gcollect = function(compos) {

    // List the promoted components
    var proms = map(function(s) { return mklist(scdl.promote(s), true); }, scdl.promotions(mklist(compos)));

    // List the referenced components
    var refs = reduce(function(a, comp) {
                return append(a,
                    map(function(ref) { return mklist(scdl.target(ref), true); }, filter(function(ref) { return scdl.target(ref) != null; }, scdl.references(comp))));
            }, mklist(), scdl.components(mklist(compos)));

    // Filter out the unused components
    var used = append(proms, refs);
    return append(mklist(element, "'composite"),
            filter(function(c) { return !(isElement(c) && elementName(c) == "'component" && isNil(assoc(scdl.name(c), used))); }, elementChildren(compos)));
}

/**
 * Clone and cleanup clonable references.
 */
graph.clonerefs = function(compos) {
    return append(mklist(element, "'composite"),
            map(function(c) {
                if (elementName(c) != "'component")
                    return c;

                // If the references are clonable
                var refs = scdl.references(c);
                if (isNil(refs))
                    return c;
                if (scdl.clonable(car(refs)) != 'true')
                    return c;
                    
                // Filter out the unwired references and add a fresh unwired
                // reference at the end of the list
                var cc = append(
                    filter(function(e) { return !(elementName(e) == "'reference" && scdl.target(e) == null); }, elementChildren(c)),
                    mklist(mklist(element, "'reference", mklist(attribute, "'name", scdl.name(car(refs))), mklist(attribute, "'clonable", "true"))));
                return append(mklist(element, "'component"), cc);
            
            }, elementChildren(compos)));
}

/**
 * Refactor references to a component.
 */
graph.refactorrefs = function(refs, oname, nname) {
    if (isNil(refs))
        return true;
    var ref = car(refs);
    if (scdl.target(ref) != oname)
        return graph.refactorrefs(cdr(refs), oname, nname);

    // Change the reference's target attribute
    setElement(ref, append(mklist(element, "'reference"),
        append(filter(function(e) { return !(isAttribute(e) && attributeName(e) == "'target"); }, elementChildren(ref)),
            mklist(mklist(attribute, "'target", nname)))));

    return graph.refactorrefs(cdr(refs), oname, nname);
};

/**
 * Rename a component.
 */
graph.renamecomp = function(comp, compos, name) {

    // Refactor all the references to the renamed component
    var oname = scdl.name(comp);
    map(function(c) { return graph.refactorrefs(scdl.references(c), oname, name); }, namedElementChildren("'component", compos));

    // Rename the SCDL promoted service and component
    var proms = filter(function(s) { return scdl.name(s) == oname }, scdl.services(compos));
    if (!isNil(proms))
        setElement(car(proms), mklist(element, "'service", mklist(attribute, "'name", name), mklist(attribute, "'promote", name)));
    setElement(comp, append(mklist(element, "'component"),
        cons(mklist(attribute, "'name", name),
            filter(function(e) { return !(isAttribute(e) && attributeName(e) == "'name"); }, elementChildren(comp)))));

    return append(mklist(element, "'composite"), elementChildren(compos));
};

/**
 * Cut the wire to a component node and make that node a
 * top level component node.
 */
graph.cutwire = function(node, compos, g) {

    /**
     * Find the reference wired to a node and cut its wire.
     */
    function cutref(refs, node) {
        if (isNil(refs))
            return true;
        var ref = car(refs);
        if (caddr(ref) == node) {
            setlist(ref, mklist(car(ref), cadr(ref), null));
            setElement(car(ref),
                append(mklist(element, "'reference"),
                    filter(function(e) { return !(isAttribute(e) && attributeName(e) == "'target"); }, elementChildren(car(ref)))));
        }
        return cutref(cdr(refs), node);
    }

    // Cut any reference wire, if found
    cutref(node.parentNode.refpos, node);

    // Make the component node a top level node.
    node.compos = g.compos;

    // Update the SCDL composite, add a promote element for
    // that component
    var comp = node.comp;
    var name = scdl.name(comp);
    var prom = mklist(element, "'service", mklist(attribute, "'name", name), mklist(attribute, "'promote", name));
    return append(mklist(element, "'composite"),
            append(mklist(prom), filter(function(c) { return !(isElement(c) && elementName(c) == "'service" && scdl.name(c) == name); }, elementChildren(compos))));
}

/**
 * Wire a component to the closest neighbor reference.
 */
graph.wire = function(n, compos, g) {

    // Compute position of the component's service node
    var spos = cadr(car(n.svcpos));
    var aspos = graph.abspos(n, g).rmove(spos.xpos(), spos.ypos());

    /**
     * Find closest unwired reference node among all the references
     * of all the components.
     */
    function closecomprefs(nodes, spos, cref) {

        /**
         * Find the closest unwired reference node among all the
         * references of a node.
         */
        function closerefs(npos, refs, spos, cref) {
            if (isNil(refs))
                return cref;
            var fdist = cadddr(cref);
            var ref = car(refs);

            // Skip wired reference
            if (!isNil(filter(function(n) { return isAttribute(n) && attributeName(n) == "'target"; }, car(ref))))
                return closerefs(npos, cdr(refs), spos, cref);

            // Compute distance between service node and reference node
            var rpos = cadr(ref).clone().rmove(npos.xpos(), npos.ypos());
            var dx = Math.pow(rpos.xpos() - spos.xpos(), 2);
            var dy = Math.pow(rpos.ypos() - spos.ypos(), 2);

            // Check for proximity threshold
            var rdist = (dx < (proxcx * proxcx) && dy < (proxcy * proxcy))? Math.sqrt(dx + dy) : 25000000;

            // Go through all the references in the component
            return closerefs(npos, cdr(refs), spos, fdist < rdist? cref : mklist(car(ref), cadr(ref), caddr(ref), rdist));
        }

        if (isNil(nodes))
            return cref;

        // Skip non-component nodes
        var node = car(nodes);
        if (isNil(node.comp))
            return closecomprefs(cdr(nodes), spos, cref);

        // Compute the component absolute position
        var npos = graph.abspos(node, g);

        // Go through all the components and their references
        return closecomprefs(append(nodeList(node.childNodes), cdr(nodes)), spos, closerefs(npos, node.refpos, spos, cref));
    }

    // Find closest reference node
    var cref = closecomprefs(nodeList(g.childNodes), aspos, mklist(null, graph.mkpath(), null, 25000000));
    if (car(cref) == null)
        return compos;
    if (cadddr(cref) == 25000000)
        return compos;

    // Wire component to that reference, un-promote it, and
    // update the SCDL reference and composite
    setElement(n.comp, graph.movecomp(graph.dragging.comp, null));
    n.compos = null;
    setElement(car(cref), append(mklist(element, "'reference", mklist(attribute, "'target", scdl.name(n.comp))), elementChildren(car(cref))));
    var name = scdl.name(n.comp);
    return append(mklist(element, "'composite"),
            filter(function(c) { return !(isElement(c) && elementName(c) == "'service" && scdl.name(c) == name); }, elementChildren(compos)));
}

/**
 * Display a list of graphical nodes.
 */
graph.display = function(nodes, g, svg) {
    var suspend = svg.suspendRedraw(10);

    // Append the nodes to the graphical canvas
    appendNodes(nodes, g);
    
    svg.unsuspendRedraw(suspend);
    return nodes;
};

/**
 * Hide a graph.
 */
graph.hide = function(g) {

    // Remove nodes from the graph
    map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { g.removeChild(n); } return n; }, nodeList(g.childNodes));
    return g;
};

/**
 * Refresh a graph.
 */
graph.refresh = function(g) {
    //log('refresh');

    // Remove existing nodes from the graph
    map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { g.removeChild(n); } return n; }, nodeList(g.childNodes));

    // Redisplay the composite associated with the graph
    var nodes = graph.composite(g.compos, graph.mkpath().pos(palcx,0), false, g);
    appendNodes(nodes, g);
    return nodes;
};

/**
 * Display and enable editing of a composite and the graphical
 * nodes that represent it.
 */
graph.edit = function(appname, compos, nodes, onchange, onselect, g) {
    var suspend = g.suspendRedraw(10);

    // Store the appname and composite in the graphical canvas
    g.appname = appname;
    g.compos = compos;

    // Sort the composite elements now to allow for change detection later
    var scompos = scdl.composite(g.compos);
    setElement(scompos, graph.sortcompos(scompos));

    // Store event listeners
    g.oncomposchange = onchange;
    g.oncompselect = onselect;

    // Remove existing nodes from the graph
    map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { g.removeChild(n); } return n; }, nodeList(g.childNodes));

    // Display the composite nodes
    appendNodes(nodes, g);

    g.unsuspendRedraw(suspend);
    return nodes;
};

