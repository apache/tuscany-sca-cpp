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
 * SVG and VML composite rendering functions.
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

graph.colors.orange1 = '#ffbb00';
graph.colors.green1 = '#96d333';
graph.colors.blue1 = '#00c3c9';
graph.colors.red1 = '#d03f41';
graph.colors.yellow1 = '#fcee21';
graph.colors.magenta1 = '#c0688a';
graph.colors.cyan1 = '#d5dcf9';
graph.colors.lightgray1 = '#dcdcdc'

/**
 * Default positions and sizes.
 */
var palcx = 2500;
var proxcx = 20;
var proxcy = 20;
var buttoncx = 65;
var buttoncy = 30;
var curvsz = 6;
var tabsz = 2;
var fontsz = '11px';

/**
 * Base path class.
 */
graph.BasePath = function() {
    this.path = '';
    this.x = 0;
    this.y = 0;

    this.pos = function(x, y) {
        this.x = x;
        this.y = y;
        return this;
    };

    this.xpos = function() {
        return this.x;
    };

    this.ypos = function() {
        return this.y;
    };

    this.rmove = function(x, y) {
        return this.move(this.x + x, this.y + y);
    };

    this.rline = function(x, y) {
        return this.line(this.x + x, this.y + y);
    };

    this.rcurve = function(x1, y1, x, y) {
        return this.curve(this.x + x1, this.y + y1, this.x + x1 + x, this.y + y1 + y);
    };
    
    this.str = function() {
        return this.path;
    };
};

/**
 * SVG rendering functions.
 */

graph.svgns='http://www.w3.org/2000/svg';

/**
 * Make an SVG graph.
 */
graph.mkgraph = function(cdiv, pos, cvalue, cadd, cdelete) {

    // Create a div element to host the graph
    var div = document.createElement('div');
    div.id = 'svgdiv';
    div.style.position = 'absolute';
    div.style.left = ui.pixpos(pos.xpos() + cdiv.offsetLeft);
    div.style.top = ui.pixpos(pos.ypos() + cdiv.offsetTop);
    div.style.overflow = 'hidden';
    cdiv.appendChild(div);

    // Create SVG element
    var svg = document.createElementNS(graph.svgns, 'svg');
    svg.style.height = ui.pixpos(5000);
    svg.style.width = ui.pixpos(5000);
    div.appendChild(svg);

    // Track element dragging and selection
    graph.dragging = null;
    graph.selected = null;
    cvalue.disabled = true;
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
     * Handle a mouse down event.
     */
    div.onmousedown = function(e) {

        // On mouse controlled devices, engage the click component selection
        // logic right away
        if (typeof e.touches == 'undefined')
            div.onclick(e);

        // Find draggable component
        var dragging = draggable(e.target);
        if (dragging == null || dragging != graph.selected)
            return true;
        graph.dragging = dragging;

        // Remember current mouse position
        var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
        graph.dragX = pos.screenX;
        graph.dragY = pos.screenY;

        if (e.preventDefault)
            e.preventDefault();
        else
            e.returnValue = false;
        return true;
    };

    // Support touch devices
    div.ontouchstart = div.onmousedown;

    /**
     * Handle a mouse up event.
     */
    div.onmouseup = function(e) {
        if (graph.dragging == null)
            return true;

        if (graph.dragging.parentNode == svg && graph.dragging.id.substring(0, 8) != 'palette:') {
            var gpos = graph.relpos(graph.dragging);

            // Add new dragged component to the composite
            if (isNil(graph.dragging.compos)) {
                var compos = scdl.composite(svg.compos);
                setElement(compos, graph.sortcompos(graph.addcomp(graph.dragging.comp, compos)));
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
                graph.move(graph.dragging, graph.mkpath().move(graph.gridsnap(gpos.xpos()), graph.gridsnap(gpos.ypos())));
                setElement(graph.dragging.comp, graph.movecomp(graph.dragging.comp, graph.abspos(graph.dragging, svg)));
            }
        }

        // Forget current dragged component
        graph.dragging = null;

        // Refresh the composite
        var nodes = graph.refresh(svg);

        // Reselected the previously selected component
        graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
        graph.compselect(graph.selected, true, cvalue, cdelete);

        // Trigger component select event
        svg.oncompselect(graph.selected);

        // Trigger composite change event
        svg.oncomposchange(false);
        return true;
    };

    // Support touch devices
    div.ontouchend = div.onmouseup;

    // Handle a mouse click event.
    div.onclick = svg.onclick = function(e) {

        // Find selected component
        var selected = draggable(e.target);
        if (selected == null) {
            if (graph.selected != null) {

                // Reset current selection
                graph.compselect(graph.selected, false, cvalue, cdelete);
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
        graph.compselect(graph.selected, false, cvalue, cdelete);

        // Clone component from the palette
        if (selected.id.substring(0, 8) == 'palette:') {
            var compos = scdl.composite(svg.compos);
            graph.selected = graph.clonepalette(selected, compos, svg);
            setElement(compos, graph.sortcompos(graph.addcomp(graph.selected.comp, compos)));
            graph.selected.compos = svg.compos;

            // Move into the editing area and hide the palette
            var gpos = graph.relpos(graph.selected);
            graph.move(graph.selected, graph.mkpath().move(gpos.xpos() + palcx, gpos.ypos()));
            div.style.left = ui.pixpos(palcx * -1);

            // Update component position
            setElement(graph.selected.comp, graph.movecomp(graph.selected.comp, graph.abspos(graph.selected, svg)));

            // Refresh the composite
            var nodes = graph.refresh(svg);

            // Reselect the previously selected component
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);

            // Trigger composite change event
            svg.oncomposchange(true);

        } else {
            graph.selected = selected;

            // Select the component
            graph.compselect(graph.selected, true, cvalue, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);
        }

        if (e.preventDefault)
            e.preventDefault();
        else
            e.returnValue = false;
        return true;
    }

    /**
     * Handle a mouse move event.
     */
    window.onmousemove = function(e) {
        if (graph.dragging == null)
            return true;

        // Get the mouse position
        var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
        if (pos.screenX == graph.dragX && pos.screenY == graph.dragY)
            return true;

        // Cut wire to component
        if (graph.dragging.parentNode != svg) {
            var compos = scdl.composite(svg.compos);
            setElement(compos, graph.sortcompos(graph.cutwire(graph.dragging, compos, svg)));

            // Bring component to the top
            graph.bringtotop(graph.dragging, svg);
        }

        // Calculate new position of dragged element
        var gpos = graph.relpos(graph.dragging);
        var newX = gpos.xpos() + (pos.screenX - graph.dragX);
        var newY = gpos.ypos() + (pos.screenY - graph.dragY);
        if (newX >= palcx)
            graph.dragX = pos.screenX;
        else
            newX = palcx;
        if (newY >= 0)
            graph.dragY = pos.screenY;
        else
            newY = 0;

        // Move the dragged element
        graph.move(graph.dragging, graph.mkpath().move(newX, newY));

        return true;
    };

    // Support touch devices
    div.ontouchmove = window.onmousemove;

    /**
     * Handle field on change events.
     */
    cvalue.onchange = function() {
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
            var nodes = graph.refresh(svg);

            // Reselected the previously selected component
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, cdelete);

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
            var nodes = graph.refresh(svg);

            // Reselected the previously selected component
            graph.selected = graph.findcompnode(scdl.name(graph.selected.comp), nodes);
            graph.compselect(graph.selected, true, cvalue, cdelete);

            // Trigger component select event
            svg.oncompselect(graph.selected);

            // Trigger composite change event
            svg.oncomposchange(true);
            return false;
        }

        return graph.hasproperty(graph.selected.comp)? changeprop() : changename();
    };
    
    // Handle delete event
    cdelete.onclick = function() {
        if (graph.selected == null)
            return false;
        if (graph.selected.id.substring(0, 8) != 'palette:') {

            // Remove selected component
            var compos = scdl.composite(svg.compos);
            if (isNil(graph.selected.compos))
                setElement(compos, graph.sortcompos(graph.cutwire(graph.selected, compos, svg)));
            setElement(compos, graph.sortcompos(graph.clonerefs(graph.gcollect(graph.removecomp(graph.selected.comp, compos)))));

            // Reset current selection
            graph.compselect(graph.selected, false, cvalue, cdelete);
            graph.selected = null;

            // Refresh the composite
            graph.refresh(svg);

            // Trigger component select event
            svg.oncompselect(null);

            // Trigger composite change event
            svg.oncomposchange(true);
        }
        return false;
    };

    // Handle add event
    cadd.onclick = function() {

        // Show the palette
        div.style.left = ui.pixpos(0);
        return false;
    };

    // Create a hidden SVG element to help compute the width
    // of component and reference titles
    graph.titlewidthsvg = document.createElementNS(graph.svgns, 'svg');
    graph.titlewidthsvg.style.visibility = 'hidden';
    graph.titlewidthsvg.style.height = ui.pixpos(0);
    graph.titlewidthsvg.style.width = ui.pixpos(0);
    div.appendChild(graph.titlewidthsvg);

    return svg;
};

/**
 * Make a path.
 */
graph.mkpath = function() {
    function Path() {
        this.BasePath = graph.BasePath;
        this.BasePath();

        this.clone = function() {
            return graph.mkpath().pos(this.xpos(), this.ypos());
        };

        this.move = function(x, y) {
            this.path += 'M' + x + ',' + y + ' '; 
            return this.pos(x, y);
        };

        this.line = function(x, y) {
            this.path += 'L' + x + ',' + y + ' ';
            return this.pos(x, y);
        };

        this.curve = function(x1, y1, x, y) {
            this.path += 'Q' + x1 + ',' + y1 + ' ' + x + ',' + y + ' ';
            return this.pos(x, y);
        };

        this.end = function() {
            this.path += 'Z';
            return this;
        };
    }

    return new Path();
};

/**
 * Return an element representing a title.
 */
graph.mktitle = function(t, style) {
    var title = document.createElementNS(graph.svgns, 'text');
    title.setAttribute('x', 5);
    title.setAttribute('y', 15);
    title.setAttribute('text-anchor', 'start');
    if (style != '')
        title.style.cssText = style;
    if (fontsz != '')
        title.style.fontSize = fontsz;
    title.style.cursor = 'default';
    title.appendChild(document.createTextNode(t));
    return title;
};

/**
 * Return an element representing the title of a component.
 */
graph.comptitle = function(comp) {
    return graph.mktitle(graph.title(comp), graph.compstyle(comp));
};

/**
 * Return the width of the title of a component.
 */
graph.comptitlewidth = function(comp) {
    var title = graph.comptitle(comp);
    graph.titlewidthsvg.appendChild(title);
    var width = title.getBBox().width + 2;
    graph.titlewidthsvg.removeChild(title);
    return width;
};

/**
 * Return an element representing the title of a reference.
 */
graph.reftitle = function(ref) {
    return graph.mktitle(graph.title(ref), graph.refstyle(ref));
};

/**
 * Return the width of the title of a reference.
 */
graph.reftitlewidth = function(ref) {
    var title = graph.reftitle(ref);
    graph.titlewidthsvg.appendChild(title);
    var width = title.getBBox().width;
    graph.titlewidthsvg.removeChild(title);
    return width;
};

/**
 * Return an element representing the value of a property.
 */
graph.proptitle = function(comp) {
    var title = graph.mktitle(graph.propertytitle(comp), graph.propstyle(comp));
    title.setAttribute('x', graph.comptitlewidth(comp) + 7);
    return title;
};

/**
 * Return the width of the title of a property.
 */
graph.proptitlewidth = function(comp) {
    var title = graph.proptitle(comp);
    graph.titlewidthsvg.appendChild(title);
    var width = title.getBBox().width + 4;
    graph.titlewidthsvg.removeChild(title);
    return width;
};

/**
 * Draw a component shape selection.
 */
graph.compselect = function(g, s, cvalue, cdelete) {
    if (isNil(g) || !s) {
        if (!isNil(cvalue)) {
            cvalue.value = '';
            cvalue.disabled = true;
        }
        if (!isNil(cdelete))
            cdelete.disabled = true;
        if (isNil(g))
            return true;
        g.contour.setAttribute('stroke', graph.colors.gray);
        g.contour.setAttribute('stroke-opacity', '0.20');
        return true;
    }

    if (!isNil(cvalue)) {
        cvalue.value = graph.hasproperty(g.comp)? graph.property(g.comp) : g.id;
        cvalue.disabled = false;
    }
    if (!isNil(cdelete))
        cdelete.disabled = false;

    g.contour.setAttribute('stroke', graph.colors.link);
    g.contour.setAttribute('stroke-opacity', '0.80');
    g.parentNode.appendChild(g);
    return true;
};

/**
 * Return a node representing a component.
 */
graph.compnode = function(comp, cassoc, pos, parentg) {

    // Make the component and property title elements
    var title = graph.comptitle(comp);
    var prop = graph.proptitle(comp);

    // Compute the path of the component shape
    var path = graph.comppath(comp, cassoc);
    var d = path.str();

    // Create the main component shape
    var shape = document.createElementNS(graph.svgns, 'path');
    shape.setAttribute('d', d);
    shape.setAttribute('fill', graph.color(comp));
    shape.setAttribute('fill-opacity', '0.60');

    // Create an overlay contour shape
    var contour = document.createElementNS(graph.svgns, 'path');
    contour.setAttribute('d', d);
    contour.setAttribute('fill', 'none');
    contour.setAttribute('stroke', graph.colors.gray);
    contour.setAttribute('stroke-width', '3');
    contour.setAttribute('stroke-opacity', '0.20');
    contour.setAttribute('transform', 'translate(1,1)');

    // Create a group and add the component and contour shapes to it.
    var g = document.createElementNS(graph.svgns, 'g');
    g.id = scdl.name(comp);
    g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    g.appendChild(shape);
    g.appendChild(contour);
    g.appendChild(title);
    g.appendChild(prop);

    // Store the component and the positions of its services
    // and references in the component shape
    g.comp = comp;
    g.refpos = reverse(path.refpos);
    g.svcpos = reverse(path.svcpos);

    // Store the contour in the component shape
    g.contour = contour;

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
    return g;
};

/**
 * Return a node representing a button.
 */
graph.mkbutton = function(t, pos) {

    // Make the button title
    var title = graph.mktitle(t, '');

    // Compute the path of the button shape
    var path = graph.buttonpath().str();

    // Create the main button shape
    var shape = document.createElementNS(graph.svgns, 'path');
    shape.setAttribute('d', path);
    shape.setAttribute('fill', graph.colors.lightgray);
    shape.setAttribute('fill-opacity', '0.60');

    // Create an overlay contour shape
    var contour = document.createElementNS(graph.svgns, 'path');
    contour.setAttribute('d', path);
    contour.setAttribute('fill', 'none');
    contour.setAttribute('stroke', graph.colors.gray);
    contour.setAttribute('stroke-width', '3');
    contour.setAttribute('stroke-opacity', '0.20');
    contour.setAttribute('transform', 'translate(1,1)');

    // Create a group and add the button and contour shapes to it
    var g = document.createElementNS(graph.svgns, 'g');
    g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
    g.appendChild(shape);
    g.appendChild(contour);
    g.appendChild(title);

    // Store the contour in the button shape
    g.contour = contour;

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
    return graph.mkpath().move(curX, curY);
};

/**
 * Move a node.
 */
graph.move = function(e, pos) {
    e.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
};

/**
 * Return the absolute position of a component node.
 */
graph.abspos = function(e, g) {
    if (e == g)
        return graph.mkpath();
    var gpos = graph.relpos(e);
    var pgpos = graph.abspos(e.parentNode, g);
    return graph.mkpath().move(gpos.xpos() + pgpos.xpos(), gpos.ypos() + pgpos.ypos());
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
 * Return the display style of an SCDL component or reference.
 */
graph.compstyle = graph.refstyle = function(e) {
    var s = scdl.style(e);
    return isNil(s)? '' : s;
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
 * Return the display style of the property of an SCDL component.
 */
graph.propstyle = function(e) {
    var p = scdl.properties(e);
    if (isNil(p))
        return '';
    if (scdl.visible(car(p)) == 'false')
        return '';
    var s = scdl.style(car(p));
    return isNil(s)? '' : s;
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
 * Return the services on the top side of a component.
 */
graph.tsvcs = function(comp) {
    return memo(comp, 'tsvcs', function() {
        var svcs = scdl.services(comp);
        var l = filter(function(s) { return scdl.align(s) == 'top' && scdl.visible(s) != 'false'; }, svcs);
        if (isNil(l))
            return mklist();
        return mklist(car(l));
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
        if (!isNil(graph.tsvcs(comp)))
            return mklist();
        return mklist(car(l));
    });
};

/**
 * Return the references on the bottom side of a component.
 */
graph.brefs = function(comp) {
    return memo(comp, 'brefs', function() {
        return filter(function(r) { return scdl.align(r) == 'bottom' && scdl.visible(r) != 'false'; }, scdl.references(comp));
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
            return tabsz * 10;
        return graph.compclosureheight(cadr(target), cassoc);
    });
};

/**
 * Return the height of a reference on the bottom side of a component.
 */
graph.brefheight = function(ref, cassoc) {
    return memo(ref, 'bheight', function() {
        var target = assoc(scdl.target(ref), cassoc);
        if (isNil(target))
            return 0;
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
 * Return the max height of the references on the bottom side of a component.
 */
graph.brefsheight = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return Math.max(graph.brefheight(car(refs), cassoc), graph.brefsheight(cdr(refs), cassoc));
};

/**
 * Return the height of a component node.
 */
graph.compheight = function(comp, cassoc) {
    return memo(comp, 'height', function() {
        var lsvcs = graph.lsvcs(comp);
        var lsvcsh = Math.max(1, length(lsvcs)) * (tabsz * 10) + (tabsz * 4);
        var rrefs = graph.rrefs(comp);
        var rrefsh = graph.rrefsheight(rrefs, cassoc) + (tabsz * 4);
        var height = Math.max(lsvcsh, rrefsh);
        if (!isNil(graph.brefs(comp)))
            height = Math.max(height, (tabsz * 10) + (tabsz * 4) + (tabsz * 2));
        return height;
    });
};

/**
 * Return the height of a component and the components wired to its bottom side.
 */
graph.compclosureheight = function(comp, cassoc) {
    return memo(comp, 'closureheight', function() {
        var brefs = graph.brefs(comp);
        var height = graph.compheight(comp, cassoc) + graph.brefsheight(brefs, cassoc);
        return height;
    });
};

/**
 * Return the width of a reference on the bottom side of a component.
 */
graph.brefwidth = function(ref, cassoc) {
    return memo(ref, 'width', function() {
        var target = assoc(scdl.target(ref), cassoc);
        if (isNil(target))
            return tabsz * 10;
        return graph.compclosurewidth(cadr(target), cassoc);
    });
};

/**
 * Return the total width of the references on the bottom side of a component.
 */
graph.brefswidth = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return graph.brefwidth(car(refs), cassoc) + graph.brefswidth(cdr(refs), cassoc);
};

/**
 * Return the max width of the references on the right side of a component.
 */
graph.rrefswidth = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return Math.max(graph.brefwidth(car(refs), cassoc), graph.rrefswidth(cdr(refs), cassoc));
};

/**
 * Return the width of a component.
 */
graph.compwidth = function(comp, cassoc) {
    return memo(comp, 'width', function() {
        var twidth = graph.comptitlewidth(comp) + graph.proptitlewidth(comp) + (tabsz * 8);
        var tsvcs = graph.tsvcs(comp);
        var tsvcsw = Math.max(1, length(tsvcs)) * (tabsz * 10) + (tabsz * 4);
        var brefs = graph.brefs(comp);
        var brefsw = graph.brefswidth(brefs, cassoc) + (tabsz * 4);
        var width = Math.max(twidth, Math.max(tsvcsw, brefsw));
        return width;
    });
};

/**
 * Return the width of a component and all the components wired to its right side.
 */
graph.compclosurewidth = function(comp, cassoc) {
    return memo(comp, 'closurewidth', function() {
        var rrefs = graph.rrefs(comp);
        var width = graph.compwidth(comp, cassoc) + graph.rrefswidth(rrefs, cassoc);
        return width;
    });
};

/**
 * Return a path representing a reference positioned to the right of a component.
 */
graph.rrefpath = function(ref, cassoc, path) {
    var height = graph.rrefheight(ref, cassoc);

    // Record reference position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    //path.refpos = cons(mklist(ref, graph.mkpath().move(xpos, ypos + (tabsz * 6))), path.refpos);
    path.refpos = cons(mklist(ref, graph.mkpath().move(xpos, ypos + (tabsz * 5))), path.refpos);

    // Compute the reference path
    return path.rline(0,tabsz).rline(0,tabsz * 2).rcurve(0,tabsz,-tabsz,0).rcurve(-tabsz,0,0,-tabsz).rcurve(0,-tabsz,-tabsz,0).rcurve(-tabsz,0,0,tabsz).rline(0,tabsz * 4).rcurve(0,tabsz,tabsz,0).rcurve(tabsz,0,0,-tabsz).rcurve(0,-tabsz,tabsz,0).rcurve(tabsz,0,0,tabsz).line(path.xpos(),ypos + height);
};

/**
 * Return a path representing a reference positioned at the bottom of a component.
 */
graph.brefpath = function(ref, cassoc, path) {
    var width = graph.brefwidth(ref, cassoc);

    // Record reference position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.refpos = cons(mklist(ref, graph.mkpath().move(xpos - width + tabsz * 5, ypos)), path.refpos);

    // Compute the reference path
    return path.line(xpos - width + (tabsz * 10),path.ypos()).rline(-tabsz,0).rline(-(tabsz *2),0).rcurve(-tabsz,0,0,-tabsz).rcurve(0,-tabsz,tabsz,0).rcurve(tabsz,0,0,-tabsz).rcurve(0,-tabsz,-tabsz,0).rline(-(tabsz * 4),0).rcurve(-tabsz,0,0,tabsz).rcurve(0,tabsz,tabsz,0).rcurve(tabsz,0,0,tabsz).rcurve(0,tabsz,-tabsz,0).line(xpos - width,path.ypos());
};

/**
 * Return a path representing a service positioned to the left of a component.
 */
graph.lsvcpath = function(svc, cassoc, path) {
    var height = tabsz * 10;

    // Record service position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.svcpos = cons(mklist(svc, graph.mkpath().move(xpos, ypos - (tabsz * 6))), path.svcpos);

    // Compute the service path
    return path.rline(0,-tabsz).rline(0, -(tabsz * 2)).rcurve(0,-tabsz,-tabsz,0).rcurve(-tabsz,0,0,tabsz).rcurve(0,tabsz,-tabsz,0).rcurve(-tabsz,0,0,-tabsz).rline(0,-(tabsz * 4)).rcurve(0,-tabsz,tabsz,0).rcurve(tabsz,0,0,tabsz).rcurve(0,tabsz,tabsz,0).rcurve(tabsz,0,0,-tabsz).line(path.xpos(), ypos - height);
};

/**
 * Return a path representing a service positioned at the top of a component.
 */
graph.tsvcpath = function(svc, cassoc, path) {
    var width = tabsz * 10;

    // Record service position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.svcpos = cons(mklist(svc, graph.mkpath().move(xpos + (tabsz * 5), ypos)), path.svcpos);

    // Compute the service path
    return path.rline(tabsz,0).rline(tabsz * 2,0).rcurve(tabsz,0,0,-tabsz).rcurve(0,-tabsz,-tabsz,0).rcurve(-tabsz,0,0,-tabsz).rcurve(0,-tabsz,tabsz,0).rline(tabsz * 4,0).rcurve(tabsz,0,0,tabsz).rcurve(0,tabsz,-tabsz,0).rcurve(-tabsz,0,0,tabsz).rcurve(0,tabsz,tabsz,0).line(xpos + width,path.ypos());
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
    function renderpath(x, f, cassoc, path) {
        if (isNil(x))
            return path;
        return renderpath(cdr(x), f, cassoc, f(car(x), cassoc, path));
    }

    var path = graph.mkpath().move(curvsz,0);

    // Store the positions of services and references in the path
    path.refpos = mklist();
    path.svcpos = mklist();

    // Render the services on the top side of the component
    var tsvcs = graph.tsvcs(comp);
    path = renderpath(tsvcs, graph.tsvcpath, cassoc, path);

    // Render the references on the right side of the component
    var rrefs = graph.rrefs(comp);
    path = path.line(width - curvsz,path.ypos()).rcurve(curvsz,0,0,curvsz);
    path = renderpath(rrefs, graph.rrefpath, cassoc, path);

    // Render the references on the bottom side of the component
    var brefs = reverse(graph.brefs(comp));
    var boffset = curvsz + graph.brefswidth(brefs, cassoc);
    path = path.line(path.xpos(),height - curvsz).rcurve(0,curvsz,curvsz * -1,0).line(boffset, path.ypos());
    path = renderpath(brefs, graph.brefpath, cassoc, path);

    // Render the services on the left side of the component
    var lsvcs = graph.lsvcs(comp);
    var loffset = curvsz + (length(lsvcs) * (tabsz * 10));
    path = path.line(curvsz,path.ypos()).rcurve(curvsz * -1,0,0,curvsz * -1).line(path.xpos(), loffset);
    path = renderpath(lsvcs, graph.lsvcpath, cassoc, path);

    // Close the component node path
    path = path.line(0,curvsz).rcurve(0,curvsz * -1,curvsz,0);

    return path.end();
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

        /**
         * Render the references on the bottom side of a component.
         */
        function renderbrefs(refs, cassoc, pos, gcomp) {

            /**
             * Render a reference on the bottom side of a component.
             */
            function renderbref(ref, cassoc, pos, gcomp) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return null;

                // Render the component target of the reference
                return rendercomp(cadr(target), cassoc, pos);
            }

            /**
             * Move the rendering cursor to the right of a reference.
             */
            function rendermove(ref, cassoc, pos) {
                return pos.clone().rmove(graph.brefwidth(ref, cassoc), 0);
            }

            if (isNil(refs))
                return mklist();

            // Return list of (ref, comp rendering) pairs
            var grefcomp = renderbref(car(refs), cassoc, pos, gcomp);
            return cons(mklist(car(refs), grefcomp), renderbrefs(cdr(refs), cassoc, rendermove(car(refs), cassoc, pos), gcomp));
        }

        // Compute the component shape
        var gcomp = graph.compnode(comp, cassoc, pos, g);

        // Render the components wired to the component references
        var rrefs = graph.rrefs(comp);
        var rpos = graph.mkpath().rmove(graph.compwidth(comp, cassoc), 0);
        var grrefs = renderrrefs(rrefs, cassoc, rpos, gcomp);

        var brefs = graph.brefs(comp);
        var bpos = graph.mkpath().rmove(0 , graph.compheight(comp, cassoc));
        var gbrefs = renderbrefs(brefs, cassoc, bpos, gcomp);

        // Store list of (ref, pos, component rendering) triplets in the component
        function refposgcomp(refpos, grefs) {
            if (isNil(refpos))
                return mklist();

            // Append component rendering to component
            var gref = cadr(car(grefs));
            if (gref != null)
                appendNodes(mklist(gref), gcomp);
            return cons(mklist(car(car(refpos)), cadr(car(refpos)), gref), refposgcomp(cdr(refpos), cdr(grefs)));
        }

        gcomp.refpos = refposgcomp(gcomp.refpos, append(grrefs, gbrefs));

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
         * Return the position of a component.
         */
        function comppos(comp, pos) {
            var x = scdl.x(comp);
            var y = scdl.y(comp);
            return graph.mkpath().move(
                    x != null? Number(x) + palcx : pos.xpos(),
                    y != null? Number(y) : (isNil(graph.tsvcs(comp))? pos.ypos() : pos.ypos() + (tabsz * 4)));
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

        var cpos = comppos(comp, pos);
        return cons(rendercomp(comp, cassoc, cpos), renderproms(cdr(svcs), cassoc, rendermove(comp, cassoc, cpos)));
    }

    // Render the promoted service components
    var rproms = renderproms(proms, cassoc, pos.clone().rmove(tabsz * 4, tabsz * 4));

    if (aspalette) {

        // Prefix ids of palette component elements with 'palette:' and
        // move them to the palette area
        return map(function(r) {
                r.id = 'palette:' + r.id;
                var gpos = graph.relpos(r);
                graph.move(r, graph.mkpath().move(gpos.xpos() - palcx, gpos.ypos()));
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
graph.ucid = function(prefix, compos, clone) {

    // Build an assoc list keyed by component name
    var comps = map(function(c) { return mklist(scdl.name(c), c); }, namedElementChildren("'component", compos));

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

    return ucid(prefix == ''? 'comp' : prefix, 1);
};

/**
 * Clone a palette component node.
 */
graph.clonepalette = function(e, compos, g) {

         // Clone the SCDL component and give it a unique name
    var wcomp = append(mklist(element, "'component", mklist(attribute, "'name", graph.ucid(scdl.name(e.comp), compos, true))),
                filter(function(c) { return !(isAttribute(c) && attributeName(c) == "'name")}, elementChildren(e.comp)));
    var x = '<composite>' + writeXML(mklist(wcomp), false) + '</composite>';
    var rcompos = readXML(mklist(x));
    var comp = car(scdl.components(rcompos));

    // Make a component node
    var gcomp = graph.compnode(comp, mklist(), graph.mkpath(), g);
    graph.move(gcomp, graph.relpos(e));
    e.parentNode.appendChild(gcomp);

    return gcomp;
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
 * Add a component to a SCDL composite.
 */
graph.addcomp = function(comp, compos) {
    var name = scdl.name(comp);
    var prom = mklist(element, "'service", mklist(attribute, "'name", name), mklist(attribute, "'promote", name));
    return append(mklist(element, "'composite"), append(elementChildren(compos), mklist(prom, comp)));
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
 * Rename a component.
 */
graph.renamecomp = function(comp, compos, name) {

    /**
     * Refactor references to a component.
     */
    function refactorrefs(refs, oname, nname) {
        if (isNil(refs))
            return true;
        var ref = car(refs);
        if (scdl.target(ref) != oname)
            return refactorrefs(cdr(refs), oname, nname);

        // Change the reference's target attribute
        setElement(ref, append(mklist(element, "'reference"),
            append(filter(function(e) { return !(isAttribute(e) && attributeName(e) == "'target"); }, elementChildren(ref)),
                mklist(mklist(attribute, "'target", nname)))));

        return refactorrefs(cdr(refs), oname, nname);
    }

    // Refactor all the references to the renamed component
    var oname = scdl.name(comp);
    map(function(c) { return refactorrefs(scdl.references(c), oname, name); }, namedElementChildren("'component", compos));

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
graph.display = function(nodes, g) {

    // Append the nodes to the graphical canvas
    appendNodes(nodes, g);
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

    // Remove existing nodes from the graph
    map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { g.removeChild(n); } return n; }, nodeList(g.childNodes));

    // Redisplay the composite associated with the graph
    return graph.display(graph.composite(g.compos, graph.mkpath().move(palcx,0), false, g), g);
};

/**
 * Display and enable editing of a composite and the graphical
 * nodes that represent it.
 */
graph.edit = function(appname, compos, nodes, onchange, onselect, g) {

    // Store the appname and composite in the graphical canvas
    g.appname = appname;
    g.compos = compos;

    // Sort the composite elements now to allow for change detection later
    var scompos = scdl.composite(g.compos);
    setElement(scompos, graph.sortcompos(scompos));

    // Store event listeners
    g.oncomposchange = onchange;
    g.oncompselect = onselect;

    // Display the composite nodes
    return graph.display(nodes, g);
};

