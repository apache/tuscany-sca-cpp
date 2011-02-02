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
graph.colors.green = '#00ff00';
graph.colors.magenta = '#ff00ff';
graph.colors.orange = '#ffa500';
graph.colors.pink = '#ffc0cb';
graph.colors.purple = '#800080';
graph.colors.red = '#ff0000';
graph.colors.white = '#ffffff';
graph.colors.yellow = '#ffff00';

graph.colors.orange1 = '#ffbb00';
graph.colors.green1 = '#96d333';
graph.colors.blue1 = '#00c3c9';
graph.colors.red1 = '#d03f41';
graph.colors.yellow1 = '#fcee21';
graph.colors.magenta1 = '#c0688a';

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
 * Rendering functions that work both with VML and SVG.
 */

/**
 * VML rendering.
 */
if (ui.isIE()) {

    graph.vmlns='urn:schemas-microsoft-com:vml';
    document.write('<xml:namespace ns="urn:schemas-microsoft-com:vml" prefix="v" />');

    /**
     * Make a VML graph.
     */
    graph.mkgraph = function(pos) {

        // Create div element to host the graph
        var div = document.createElement('div');
        div.id = 'vmldiv';
        div.style.position = 'absolute';
        div.style.left = pos.xpos();
        div.style.top = pos.ypos();
        document.body.appendChild(div);

        // Create a VML group
        var vmlg = document.createElement('v:group');
        vmlg.style.width = 5000;
        vmlg.style.height = 5000;
        vmlg.coordsize = '5000,5000';
        div.appendChild(vmlg);

        // Keep track of the current dragged element
        graph.dragging = null;

        /**
         * Find the first draggable element in a hierarchy of elements.
         */
        function draggable(n) {
            if (n == vmlg)
                return null;
            if (n.nodeName == 'group' && n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        /**
         * Handle a mousedown event.
         */
        vmlg.onmousedown = function() {
            window.event.returnValue = false;

            // Find draggable element
            graph.dragging = draggable(window.event.srcElement);
            if (graph.dragging == null)
                return false;

            // Clone component from the palette
            if (graph.dragging.id.substring(0, 8) == 'palette:')
                graph.dragging = graph.clonepalette(graph.dragging);

            // Cut wire to component
            if (graph.dragging.parentNode != vmlg) {
                var compos = scdl.composite(vmlg.compos);
                setElement(compos, graph.cutwire(graph.dragging, compos, vmlg));
            }

            // Bring component to the top
            graph.bringtotop(graph.dragging, vmlg);

            // Remember mouse position
            graph.dragX = window.event.clientX;
            graph.dragY = window.event.clientY;
            vmlg.setCapture();
            return false;
        };

        /**
         * Handle a mouseup event.
         */
        vmlg.onmouseup = function() {
            if (graph.dragging == null)
                return false;

            if (graph.dragging.parentNode == vmlg && graph.dragging.id.substring(0, 8) != 'palette:') {
                var gpos = graph.relpos(graph.dragging);
                if (pos.xpos() >= 350) {

                    // Add new dragged component to the composite
                    if (isNil(graph.dragging.compos)) {
                        var compos = scdl.composite(vmlg.compos);
                        setElement(compos, graph.addcomp(graph.dragging.comp, compos));
                        graph.dragging.compos = vmlg.compos;
                    }

                    // Update component position
                    setElement(graph.dragging.comp, graph.movecomp(graph.dragging.comp, graph.abspos(graph.dragging, vmlg)));

                    // Wire component to neighboring reference
                    if (!isNil(graph.dragging.svcpos)) {
                        var compos = scdl.composite(vmlg.compos);
                        setElement(compos, graph.wire(graph.dragging, compos, vmlg));
                    }

                } else {

                    // Discard component dragged out of composite
                    vmlg.removeChild(graph.dragging);
                    if (!isNil(graph.dragging.compos)) {
                        var compos = scdl.composite(vmlg.compos);
                        setElement(compos, graph.removecomp(graph.dragging.comp, compos));
                    }
                }
            }

            // Forget current dragged component
            graph.dragging = null;
            vmlg.releaseCapture();

            // Remove and refresh the composite
            map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { vmlg.removeChild(n); } return n; }, nodeList(vmlg.childNodes));
            graph.display(graph.composite(vmlg.compos, graph.mkpath().move(350,0)), vmlg);

            return false;
        };

        /**
         * Handle a mousemove event.
         */
        vmlg.onmousemove = function() {
            if (graph.dragging == null)
                return false;

            // Calculate new position of dragged element
            var gpos = graph.relpos(graph.dragging);
            var newX = gpos.xpos() + (window.event.clientX - graph.dragX);
            var newY = gpos.ypos() + (window.event.clientY - graph.dragY);
            if (newX >= 0)
                graph.dragX = window.event.clientX;
            else
                newX = 0;
            if (newY >= 0)
                graph.dragY = window.event.clientY;
            else
                newY = 0;

            // Move the dragged element
            graph.move(graph.dragging, graph.mkpath().move(newX, newY));

            return false;
        };

        // Create hidden spans to help compute the width of
        // component and reference titles
        graph.comptitlewidthdiv = document.createElement('span');
        graph.comptitlewidthdiv.style.visibility = 'hidden'
        graph.comptitlewidthdiv.style.fontWeight = 'bold'
        div.appendChild(graph.comptitlewidthdiv);

        graph.reftitlewidthdiv = document.createElement('span');
        graph.reftitlewidthdiv.style.visibility = 'hidden'
        div.appendChild(graph.reftitlewidthdiv);

        return vmlg;
    };

    /**
     * Make a shape path.
     */
    graph.mkpath = function() {
        function Path() {
            this.BasePath = graph.BasePath;
            this.BasePath();

            this.clone = function() {
                return graph.mkpath().pos(this.xpos(), this.ypos());
            };

            this.move = function(x, y) {
                this.path += 'M ' + x + ',' + y + ' '; 
                return this.pos(x, y);
            };

            this.line = function(x, y) {
                this.path += 'L ' + x + ',' + y + ' ';
                return this.pos(x, y);
            };

            this.curve = function(x1, y1, x, y) {
                this.path += 'QB ' + x1 + ',' + y1 + ',' + x + ',' + y + ' ';
                return this.pos(x, y);
            };

            this.end = function() {
                this.path += 'X E';
                return this;
            };
        }

        return new Path();
    };

    /**
     * Return an element representing a title.
     */
    graph.mktitle = function(t, bold, pos) {
        var title = document.createElement('v:textbox');
        title.style.left = pos.xpos();
        title.style.top = pos.ypos();
        title.style.position = 'absolute';
        if (bold)
            title.style.fontWeight = 'bold';
        var tnode = document.createTextNode(t);
        title.appendChild(tnode);
        return title;
    };

    /**
     * Return an element representing the title of a component.
     */
    graph.comptitle = function(comp) {
        var tsvcs = graph.tsvcs(comp);
        var lsvcs = graph.lsvcs(comp);
        var pos = graph.mkpath().move(isNil(lsvcs)? 5 : 25, isNil(tsvcs)? 5 : 25);
        return graph.mktitle(graph.title(comp), true, pos);
    };

    /**
     * Return the width of the title of a component.
     */
    graph.comptitlewidth = function(comp) {
        var t = graph.title(comp);
        graph.comptitlewidthdiv.innerHTML = t;
        var twidth = graph.comptitlewidthdiv.offsetWidth;
        graph.comptitlewidthdiv.innerHTML = '';
        return twidth;
    };

    /**
     * Return an element representing the title of a reference.
     */
    graph.reftitle = function(ref) {
        return graph.mktitle(graph.title(ref), false, graph.mkpath().move(25,25));
    };

    /**
     * Return the width of the title of a reference.
     */
    graph.reftitlewidth = function(ref) {
        var t = graph.title(ref);
        graph.reftitlewidthdiv.innerHTML = t;
        var twidth = graph.reftitlewidthdiv.offsetWidth;
        graph.reftitlewidthdiv.innerHTML = '';
        return twidth;
    };

    /**
     * Return a node representing a component.
     */
    graph.compnode = function(comp, cassoc, pos) {

        // Make the component title element
        var title = graph.comptitle(comp);

        // Compute the component shape path
        var path = graph.comppath(comp, cassoc);
        var d = path.str();

        // Create the main component shape
        var shape = document.createElement('v:shape');
        shape.style.width = 5000;
        shape.style.height = 5000;
        shape.coordsize = '5000,5000';
        shape.path = d;
        shape.fillcolor = graph.color(comp);
        shape.stroked = 'false';

        // Create an overlay contour shape
        var contour = document.createElement('v:shape');
        contour.style.width = 5000;
        contour.style.height = 5000;
        contour.coordsize = '5000,5000';
        contour.path = d;
        contour.filled = 'false';
        contour.strokecolor = graph.colors.gray;
        contour.strokeweight = '2';
        contour.style.left = 1;
        contour.style.top = 1;
        var stroke = document.createElement('v:stroke');
        stroke.opacity = '20%';
        contour.appendChild(stroke);

        // Create a group and add the component and contour shapes to it
        var g = document.createElement('v:group');
        g.id = scdl.name(comp);
        g.style.width = 5000;
        g.style.height = 5000;
        g.coordsize = '5000,5000';
        g.style.left = pos.xpos();
        g.style.top = pos.ypos();
        g.appendChild(shape);
        shape.appendChild(title);
        g.appendChild(contour)

        // Store the component and the positions of its services
        // and references in the component shape
        g.comp = comp;
        g.refpos = path.refpos;
        g.svcpos = path.svcpos;

        return g;
    };

    /**
     * Return a graphical group.
     */
    graph.mkgroup = function(pos) {
        var g = document.createElement('v:group');
        g.style.left = pos.xpos();
        g.style.top = pos.ypos();
        return g;
    };

    /**
     * Return a node representing a button.
     */
    graph.mkbutton = function(t, pos) {

        // Make the title element
        var title = graph.mktitle(t, true, pos);

        // Compute the path of the button shape
        var path = graph.buttonpath().str();

        // Create the main button shape
        var shape = document.createElement('v:shape');
        shape.style.width = 5000;
        shape.style.height = 5000;
        shape.coordsize = '5000,5000';
        shape.path = path;
        shape.fillcolor = graph.colors.blue1;
        shape.stroked = 'false';

        // Create an overlay contour shape
        var contour = document.createElement('v:shape');
        contour.style.width = 5000;
        contour.style.height = 5000;
        contour.coordsize = '5000,5000';
        contour.path = path;
        contour.filled = 'false';
        contour.strokecolor = graph.colors.gray;
        contour.strokeweight = '2';
        contour.style.left = 1;
        contour.style.top = 1;
        var stroke = document.createElement('v:stroke');
        stroke.opacity = '20%';
        contour.appendChild(stroke);

        // Create a group and add the button and contour shapes to it
        var g = document.createElement('v:group');
        g.style.width = 5000;
        g.style.height = 5000;
        g.coordsize = '5000,5000';
        g.style.left = pos.xpos();
        g.style.top = pos.ypos();
        g.appendChild(shape);
        shape.appendChild(title);
        g.appendChild(contour)
        return g;
    };

    /**
     * Return the relative position of a node.
     */
    graph.relpos = function(e) {
        var curX = ui.csspos(e.style.left);
        var curY = ui.csspos(e.style.top);
        return graph.mkpath().move(curX, curY);
    };

    /**
     * Move a node.
     */
    graph.move = function(e, pos) {
        e.style.left = pos.xpos();
        e.style.top = pos.ypos();
    };

} else {

    /**
     * SVG rendering.
     */
    graph.svgns='http://www.w3.org/2000/svg';

    /**
     * Make an SVG graph.
     */
    graph.mkgraph = function(pos) {

        // Create a div element to host the graph
        var div = document.createElement('div');
        div.id = 'svgdiv';
        div.style.position = 'absolute';
        div.style.left = pos.xpos();
        div.style.top = pos.ypos();
        // -webkit-user-select: none;
        document.body.appendChild(div);

        // Create SVG element
        var svg = document.createElementNS(graph.svgns, 'svg');
        svg.style.height = 5000;
        svg.style.width = 5000;
        div.appendChild(svg);

        // Keep track of current dragged element
        graph.dragging = null;

        /**
         * Find the first draggable element in a hierarchy of elements.
         */
        function draggable(n) {
            if (n == svg)
                return null;
            if (n.nodeName == 'g' && n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        /**
         * Handle a mouse down event.
         */
        svg.onmousedown = function(e) {
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;

            // Find draggable component
            graph.dragging = draggable(e.target);
            if (graph.dragging == null)
                return false;

            // Clone component from the palette
            if (graph.dragging.id.substring(0, 8) == 'palette:')
                graph.dragging = graph.clonepalette(graph.dragging);

            // Cut wire to component
            if (graph.dragging.parentNode != svg) {
                var compos = scdl.composite(svg.compos);
                setElement(compos, graph.cutwire(graph.dragging, compos, svg));
            }

            // Bring component to the top
            graph.bringtotop(graph.dragging, svg);

            // Remember current mouse position
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            graph.dragX = pos.screenX;
            graph.dragY = pos.screenY;
            return false;
        };

        // Support touch devices
        svg.ontouchstart = svg.onmousedown;

        /**
         * Handle a mouse up event.
         */
        window.onmouseup = function(e) {
            if (graph.dragging == null)
                return false;

            if (graph.dragging.parentNode == svg && graph.dragging.id.substring(0, 8) != 'palette:') {
                var gpos = graph.relpos(graph.dragging);
                if (gpos.xpos() >= 350) {

                    // Add new dragged component to the composite
                    if (isNil(graph.dragging.compos)) {
                        var compos = scdl.composite(svg.compos);
                        setElement(compos, graph.addcomp(graph.dragging.comp, compos));
                        graph.dragging.compos = svg.compos;
                    }

                    // Update component position
                    setElement(graph.dragging.comp, graph.movecomp(graph.dragging.comp, graph.abspos(graph.dragging, svg)));

                    // Wire component to neighboring reference
                    if (!isNil(graph.dragging.svcpos)) {
                        var compos = scdl.composite(svg.compos);
                        setElement(compos, graph.wire(graph.dragging, compos, svg));
                    }

                } else {

                    // Discard component dragged out of composite
                    svg.removeChild(graph.dragging);
                    if (!isNil(graph.dragging.compos)) {
                        var compos = scdl.composite(svg.compos);
                        setElement(compos, graph.removecomp(graph.dragging.comp, compos));
                    }
                }
            }

            // Forget current dragged component
            graph.dragging = null;

            // Remove and refresh the composite
            map(function(n) { if (!isNil(n.comp) && n.id.substr(0, 8) != 'palette:') { svg.removeChild(n); } return n; }, nodeList(svg.childNodes));
            graph.display(graph.composite(svg.compos, graph.mkpath().move(350,0)), svg);

            return false;
        };

        // Support touch devices
        window.top.onmouseup = window.onmouseup;
        window.ontouchend = window.onmouseup;
        window.gestureend = window.onmouseup;
        window.top.gestureend = window.onmouseup;
        window.top.ontouchend = window.onmouseup;
        window.ontouchcancel = window.onmouseup;
        window.top.ontouchcancel = window.onmouseup;

        /**
         * Handle a mouse move event.
         */
        window.onmousemove = function(e) {
            if (graph.dragging == null)
                return false;
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;

            // Calculate new position of dragged element
            var gpos = graph.relpos(graph.dragging);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = gpos.xpos() + (pos.screenX - graph.dragX);
            var newY = gpos.ypos() + (pos.screenY - graph.dragY);
            if (newX >= 0)
                graph.dragX = pos.screenX;
            else
                newX = 0;
            if (newY >= 0)
                graph.dragY = pos.screenY;
            else
                newY = 0;

            // Move the dragged element
            graph.move(graph.dragging, graph.mkpath().move(newX, newY));

            return false;
        };

        // Support touch devices
        window.top.onmousemove = window.onmousemove;
        window.ontouchmove = window.onmousemove;
        window.top.ontouchmove = window.onmousemove;

        // Create a hidden SVG element to help compute the width
        // of component and reference titles
        graph.titlewidthsvg = document.createElementNS(graph.svgns, 'svg');
        graph.titlewidthsvg.style.visibility = 'hidden';
        graph.titlewidthsvg.style.height = 0;
        graph.titlewidthsvg.style.width = 0;
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
    graph.mktitle = function(t, bold) {
        var title = document.createElementNS(graph.svgns, 'text');
        title.setAttribute('text-anchor', 'start');
        title.setAttribute('x', 5);
        title.setAttribute('y', 15);
        if (bold)
            title.style.fontWeight = 'bold';
        title.appendChild(document.createTextNode(t));
        return title;
    };

    /**
     * Return an element representing the title of a component.
     */
    graph.comptitle = function(comp) {
        return graph.mktitle(graph.title(comp), true);
    };

    /**
     * Return the width of the title of a component.
     */
    graph.comptitlewidth = function(comp) {
        var title = graph.comptitle(comp);
        graph.titlewidthsvg.appendChild(title);
        var width = title.getBBox().width;
        graph.titlewidthsvg.removeChild(title);
        return width;
    };

    /**
     * Return an element representing the title of a reference.
     */
    graph.reftitle = function(ref) {
        return graph.mktitle(graph.title(ref), false);
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
     * Return a node representing a component.
     */
    graph.compnode = function(comp, cassoc, pos) {

        // Make the component title
        var title = graph.comptitle(comp);

        // Compute the path of the component shape
        var path = graph.comppath(comp, cassoc);
        var d = path.str();

        // Create the main component shape
        var shape = document.createElementNS(graph.svgns, 'path');
        shape.setAttribute('d', d);
        shape.setAttribute('fill', graph.color(comp));

        // Create an overlay contour shape
        var contour = document.createElementNS(graph.svgns, 'path');
        contour.setAttribute('d', d);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', graph.colors.gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        // Create a group and add the component and contour shapes to it.
        var g = document.createElementNS(graph.svgns, 'g');
        g.id = scdl.name(comp);
        g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);

        // Store the component and the positions of its services
        // and references in the component shape
        g.comp = comp;
        g.refpos = reverse(path.refpos);
        g.svcpos = reverse(path.svcpos);

        return g;
    };

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
        var title = graph.mktitle(t, true);

        // Compute the path of the button shape
        var path = graph.buttonpath().str();

        // Create the main button shape
        var shape = document.createElementNS(graph.svgns, 'path');
        shape.setAttribute('d', path);
        shape.setAttribute('fill', graph.colors.blue1);

        // Create an overlay contour shape
        var contour = document.createElementNS(graph.svgns, 'path');
        contour.setAttribute('d', path);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', graph.colors.gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        // Create a group and add the button and contour shapes to it
        var g = document.createElementNS(graph.svgns, 'g');
        g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    };

    /**
     * Return the relative position of a node.
     */
    graph.relpos = function(e) {
        var pmatrix = e.parentNode.getCTM();
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
    var d = scdl.documentation(e);
    return d != null? d : scdl.name(e);
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
        return filter(function(r) { return scdl.align(r) == 'bottom'; }, scdl.references(comp));
    });
};

/**
 * Return the references on the right side of a component.
 */
graph.rrefs = function(comp) {
    return memo(comp, 'rrefs', function() {
        return filter(function(r) { var a = scdl.align(r); return a == null || a == 'right'; }, scdl.references(comp));
    });
};

/**
 * Return the height of a reference on the right side of a component.
 */
graph.rrefheight = function(ref, cassoc) {
    return memo(ref, 'rheight', function() {
        var target = assoc(scdl.target(ref), cassoc);
        if (isNil(target))
            return 60;
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
        var lsvcsh = Math.max(1, length(lsvcs)) * 60 + 20;
        var rrefs = graph.rrefs(comp);
        var rrefsh = graph.rrefsheight(rrefs, cassoc) + 20;
        var height = Math.max(lsvcsh, rrefsh);
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
            return 60;
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
        var twidth = graph.comptitlewidth(comp) + 20;
        var tsvcs = graph.tsvcs(comp);
        var tsvcsw = Math.max(1, length(tsvcs)) * 60 + 20;
        var brefs = graph.brefs(comp);
        var brefsw = graph.brefswidth(brefs, cassoc) + 20;
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
    path.refpos = cons(mklist(ref, graph.mkpath().move(xpos, ypos + 30)), path.refpos);

    // Compute the reference path
    return path.rline(0,10).rline(0,10).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rline(0,20).rcurve(0,5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,5).line(path.xpos(),ypos + height);
};

/**
 * Return a path representing a reference positioned at the bottom of a component.
 */
graph.brefpath = function(ref, cassoc, path) {
    var width = graph.brefwidth(ref, cassoc);

    // Record reference position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.refpos = cons(mklist(ref, graph.mkpath().move(xpos - width + 30, ypos)), path.refpos);

    // Compute the reference path
    return path.line(xpos - width + 60,path.ypos()).rline(-10,0).rline(-10,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rline(-20,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).line(xpos - width,path.ypos());
};

/**
 * Return a path representing a service positioned to the left of a component.
 */
graph.lsvcpath = function(svc, cassoc, path) {
    var height = 60;

    // Record service position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.svcpos = cons(mklist(svc, graph.mkpath().move(xpos, ypos - 30)), path.svcpos);

    // Compute the service path
    return path.rline(0,-10).rline(0, -10).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rline(0,-20).rcurve(0,-5,5,0).rcurve(5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,-5).line(path.xpos(), ypos - height);
};

/**
 * Return a path representing a service positioned at the top of a component.
 */
graph.tsvcpath = function(svc, cassoc, path) {
    var width = 60;

    // Record service position in the path
    var xpos = path.xpos();
    var ypos = path.ypos();
    path.svcpos = cons(mklist(svc, graph.mkpath().move(xpos + 30, ypos)), path.svcpos);

    // Compute the service path
    return path.rline(10,0).rline(10,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rline(20,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).line(xpos + width,path.ypos());
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

    var path = graph.mkpath().move(10,0);

    // Store the positions of services and references in the path
    path.refpos = mklist();
    path.svcpos = mklist();

    // Render the services on the top side of the component
    var tsvcs = graph.tsvcs(comp);
    path = renderpath(tsvcs, graph.tsvcpath, cassoc, path);

    // Render the references on the right side of the component
    var rrefs = graph.rrefs(comp);
    path = path.line(width - 10,path.ypos()).rcurve(10,0,0,10);
    path = renderpath(rrefs, graph.rrefpath, cassoc, path);

    // Render the references on the bottom side of the component
    var brefs = reverse(graph.brefs(comp));
    var boffset = 10 + graph.brefswidth(brefs, cassoc);
    path = path.line(path.xpos(),height - 10).rcurve(0,10,-10,0).line(boffset, path.ypos());
    path = renderpath(brefs, graph.brefpath, cassoc, path);

    // Render the services on the left side of the component
    var lsvcs = graph.lsvcs(comp);
    var loffset = 10 + (length(lsvcs) * 60);
    path = path.line(10,path.ypos()).rcurve(-10,0,0,-10).line(path.xpos(), loffset);
    path = renderpath(lsvcs, graph.lsvcpath, cassoc, path);

    // Close the component node path
    path = path.line(0,10).rcurve(0,-10,10,0);

    return path.end();
};

/**
 * Return a path representing a button node.
 */
graph.buttonpath = function(t) {
    var height = 60;
    var width = 120;
    var path = graph.mkpath().move(10,0);
    path = path.line(width - 10,path.ypos()).rcurve(10,0,0,10);
    path = path.line(path.xpos(),height - 10).rcurve(0,10,-10,0).line(10, path.ypos());
    path = path.line(10,path.ypos()).rcurve(-10,0,0,-10).line(path.xpos(), 10);
    path = path.line(0,10).rcurve(0,-10,10,0);
    return path.end();
};

/**
 * Render a SCDL composite into a list of component nodes.
 */
graph.composite = function(compos, pos) {
    var name = scdl.name(scdl.composite(compos));
    var comps = scdl.components(compos);
    var cassoc = scdl.nameToElementAssoc(comps);
    var proms = scdl.promotions(compos);

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
        var gcomp = graph.compnode(comp, cassoc, pos);

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
            return graph.mkpath().move(x != null? Number(x) + 350 : pos.xpos(), y != null? Number(y) : pos.ypos());
        }

        /**
         * Move the rendering cursor down below a component.
         */
        function rendermove(comp, cassoc, pos) {
            return pos.clone().rmove(0, graph.compclosureheight(comp, cassoc) + 40);
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
    var rproms = renderproms(proms, cassoc, pos.clone().rmove(20,20));

    if (name == 'palette') {

        // Prefix ids of palette component elements with 'palette:'
        return map(function(r) { r.id = 'palette:' + r.id; return r; }, rproms);
    } else {

        // Link app component elements to the containing composite
        return map(function(r) { r.compos = compos; return r; }, rproms);
    }
};

/**
 * Clone a palette component node.
 */
graph.clones = 0;
graph.clonepalette = function(e) {

    // Clone the SCDL component and give it a unique name
    var comp = append(mklist(element, "'component", mklist(attribute, "'name", scdl.name(e.comp) + (++graph.clones))),
                filter(function(c) { return !(isAttribute(c) && attributeName(c) == "'name")}, elementChildren(e.comp)));

    // Make a component node
    var gcomp = graph.compnode(comp, mklist(), graph.mkpath());
    graph.move(gcomp, graph.relpos(e));
    e.parentNode.appendChild(gcomp);

    return gcomp;
};

/**
 * Move a SCDL component to the given position.
 */
graph.movecomp = function(comp, pos) {
    return append(mklist(element, "'component", mklist(attribute, "'t:x", '' + (pos.xpos() - 350)), mklist(attribute, "'t:y", '' + pos.ypos())),
            filter(function(e) { return !(isAttribute(e) && (attributeName(e) == "'t:x" || attributeName(e) == "'t:y")); }, elementChildren(comp)));
};

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
            setElement(car(ref), append(mklist(element, "'reference"), filter(function(e) { return !(isAttribute(e) && attributeName(e) == "'target"); }, elementChildren(car(ref)))));
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
    return append(mklist(element, "'composite"), append(filter(function(c) {return !(isElement(c) && scdl.name(c) == name); }, elementChildren(compos)), mklist(prom, comp)));
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

            // Proximity threshold is 40 x 40 pixels
            var rdist = (dx < 400 && dy < 400)? Math.sqrt(dx + dy) : 25000000;

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

    // Wire component to that reference, update the SCDL
    // reference and composite
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
 * Display and enable editing of a composite and the graphical
 * nodes that represent it.
 */
graph.edit = function(compos, nodes, g) {

    // Store the composite in the graphical canvas
    g.compos = compos;

    // Display the composite nodes
    return graph.display(nodes, g);
};

