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
 * SVG and VML component rendering functions.
 */

var graph = new Object();

/**
 * Detect browser VML support.
 */
graph.supportsVML = function() {
    if (typeof graph.supportsVML.supported != 'undefined')
        return graph.supportsVML.supported;
    graph.supportsVML.supported = navigator.appName == 'Microsoft Internet Explorer';
    return graph.supportsVML.supported;
};

/**
 * Detect browser SVG support.
 */
graph.supportsSVG = function() {
    if (typeof graph.supportsSVG.supported != 'undefined')
        return graph.supportsSVG.supported;
    graph.supportsSVG.supported = navigator.appName != 'Microsoft Internet Explorer';
    return graph.supportsSVG.supported;
};

/**
 * Basic colors
 */
graph.red = 'red';
graph.green = 'green';
graph.blue = 'blue';
graph.yellow = 'yellow';
graph.orange = '#ffa500';
graph.gray = 'gray'

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
var graph;

/**
 * VML rendering.
 */
if (graph.supportsVML()) {

    graph.vmlns='urn:schemas-microsoft-com:vml';

    /**
     * Make a shape path.
     */
    graph.mkpath = function() {
        function Path() {
            this.BasePath = graph.BasePath;
            this.BasePath();

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
     * Make a title element.
     */
    graph.mktitle = function(t) {
        var title = document.createElement('v:textbox');
        title.style.left = '40';
        title.style.top = '30';
        title.style.position = 'absolute';
        var tnode = document.createTextNode(t);
        title.appendChild(tnode);
        return title;
    };

    /**
     * Return the width of a title.
     */
    graph.titlewidth = function(t) {
        graph.textWidthDiv.innerHTML = t;
        var twidth = textWidthDiv.offsetWidth + 10;
        graph.textWidthDiv.innerHTML = '';
        return twidth;
    };

    /**
     * Make a component shape.
     */
    graph.mkcompshape = function(name, color, tsvcs, lsvcs, brefs, rrefs) {
        var title = graph.mktitle(name);
        var twidth = graph.titlewidth(name);

        var d = graph.mkcomppath(twidth, tsvcs, lsvcs, brefs, rrefs).str();

        var shape = document.createElement('v:shape');
        shape.style.width = 500;
        shape.style.height = 500;
        shape.coordsize = '500,500';
        shape.path = d;
        shape.fillcolor = color;
        shape.stroked = 'false';

        var contour = document.createElement('v:shape');
        contour.style.width = 500;
        contour.style.height = 500;
        contour.coordsize = '500,500';
        contour.setAttribute('path', d);
        contour.filled = 'false';
        contour.strokecolor = graph.gray;
        contour.strokeweight = '3';
        contour.style.top = 1;
        contour.style.left = 1;
        var stroke = document.createElement('v:stroke');
        stroke.opacity = '20%';
        contour.appendChild(stroke);

        var g = document.createElement('v:group');
        g.style.width = 500;
        g.style.height = 500;
        g.coordsize = '500,500';
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    };

    /**
     * Make a graph.
     */
    graph.mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'vmldiv';
        document.body.appendChild(div);

        graph.textWidthDiv = document.createElement('span');
        graph.textWidthDiv.style.visibility = 'hidden'
        div.appendChild(graph.textWidthDiv);

        var vmlg = document.createElement('v:group');
        vmlg.style.width = 500;
        vmlg.style.height = 500;
        vmlg.coordsize = '500,500';
        div.appendChild(vmlg);

        graph.dragging = null;

        function draggable(n) {
            if (n == vmlg)
                return null;
            if (n.nodeName == 'group')
                return n;
            return draggable(n.parentNode);
        }

        vmlg.onmousedown = function() {
            window.event.returnValue = false;
            graph.dragging = draggable(window.event.srcElement);
            if (graph.dragging == null)
                return false;
            graph.dragging.parentNode.appendChild(graph.dragging);
            graph.dragX = window.event.clientX;
            graph.dragY = window.event.clientY;
            vmlg.setCapture();
            return false;
        };

        vmlg.onmouseup = function() {
            if (graph.dragging == null)
                return false;
            graph.dragging = null;
            vmlg.releaseCapture();
            return false;
        };

        vmlg.onmousemove = function() {
            if (graph.dragging == null)
                return false;
            var origX = graph.dragging.coordorigin.X;
            var origY = graph.dragging.coordorigin.Y;
            var newX = origX - (window.event.clientX - graph.dragX);
            var newY = origY - (window.event.clientY - graph.dragY);
            graph.dragX = window.event.clientX;
            graph.dragY = window.event.clientY;
            graph.dragging.setAttribute('coordorigin', newX + ' ' + newY);
            return false;
        };

        graph.g = vmlg;
        return vmlg;
    };

    document.write('<xml:namespace ns="urn:schemas-microsoft-com:vml" prefix="v" />');
}

/**
 * SVG rendering.
 */
if (graph.supportsSVG()) {

    graph.svgns='http://www.w3.org/2000/svg';

    /**
     * Make a shape path.
     */
    graph.mkpath = function() {
        function Path() {
            this.BasePath = graph.BasePath;
            this.BasePath();

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
     * Make a title element.
     */
    graph.mktitle = function(t) {
        var title = document.createElementNS(graph.svgns, 'text');
        title.setAttribute('text-anchor', 'start');
        title.setAttribute('x', 40);
        title.setAttribute('y', 50);
        title.appendChild(document.createTextNode(t));
        graph.g.appendChild(title);
        return title;
    };

    /**
     * Make a component shape.
     */
    graph.mkcompshape = function(name, color, tsvcs, lsvcs, brefs, rrefs) {
        var title = graph.mktitle(name);
        var twidth = title.getBBox().width;

        var d = graph.mkcomppath(twidth, tsvcs, lsvcs, brefs, rrefs).str();

        var shape = document.createElementNS(graph.svgns, 'path');
        shape.setAttribute('d', d);
        shape.setAttribute('fill', color);

        var contour = document.createElementNS(graph.svgns, 'path');
        contour.setAttribute('d', d);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', graph.gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        var g = document.createElementNS(graph.svgns, 'g');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    };

    /**
     * Make a graph.
     */
    graph.mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'svgdiv';
        document.body.appendChild(div);

        var svg = document.createElementNS(graph.svgns, 'svg');
        svg.style.height = '100%';
        svg.style.width = '100%';
        div.appendChild(svg);

        graph.dragging = null;

        function draggable(n) {
            if (n == svg)
                return null;
            if (n.nodeName == 'g')
                return n;
            return draggable(n.parentNode);
        }

        svg.onmousedown = function(e) {
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue= false;
            graph.dragging = draggable(e.target);
            if (graph.dragging == null)
                return false;
            graph.dragging.parentNode.appendChild(graph.dragging);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            graph.dragX = pos.clientX;
            graph.dragY = pos.clientY;
            return false;
        };

        svg.ontouchstart = svg.onmousedown;

        svg.onmouseup = function(e) {
            if (graph.dragging == null)
                return false;
            graph.dragging = null;
            return false;
        };

        svg.ontouchend = svg.onmouseup;

        svg.onmousemove = function(e) {
            if (graph.dragging == null)
                return false;
            var matrix = graph.dragging.getCTM();
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = Number(matrix.e) + (pos.clientX - graph.dragX);
            var newY = Number(matrix.f) + (pos.clientY - graph.dragY);
            graph.dragX = pos.clientX;
            graph.dragY = pos.clientY;
            graph.dragging.setAttribute('transform', 'translate(' + newX + ',' + newY + ')');
            return false;
        };

        svg.ontouchmove = svg.onmousemove;

        graph.g = svg;
        return svg;
    };
}

/**
 * Make a reference shape path, positioned to the right of a component shape.
 */
graph.mkrrefpath = function(path) {
    return path.rline(0,10).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rline(0,20).rcurve(0,5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,5).rline(0,10);
};

/**
 * Make a reference shape path, positioned at the bottom of a component shape.
 */
graph.mkbrefpath = function(path) {
    return path.rline(-10,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rline(-20,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rline(-10,0);
};

/**
 * Make a service shape path, positioned to the left of a component shape.
 */
graph.mklsvcpath = function(path) {
    return path.rline(0,-10).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rline(0,-20).rcurve(0,-5,5,0).rcurve(5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,-5).rline(0,-10);
};

/**
 * Make a service shape path, positioned at the top of a component shape.
 */
graph.mktsvcpath = function(path) {
    return path.rline(10,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rline(20,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rline(10,0);
};


/**
* Make a component shape path.
*/
graph.mkcomppath = function(twidth, tsvcs, lsvcs, brefs, rrefs) {
    var height = Math.max(lsvcs, rrefs) * 40 + 60;
    var width = Math.max(Math.max(tsvcs, brefs) * 40 + 60, twidth + 20);

    var path = graph.mkpath().rmove(40,30).rline(20,0);
    for (var s = 0; s < tsvcs; s++)
        path = graph.mktsvcpath(path);

    path = path.line(20 + width,path.ypos()).rcurve(10,0,0,10).rline(0,20);
    for (var r = 0; r < rrefs; r++)
        path = graph.mkrrefpath(path);

    var boffset = 10 + 30 + (brefs * 40);
    path = path.line(path.xpos(),20 + height).rcurve(0,10,-10,0).line(20 + boffset, path.ypos());
    for (var r = 0; r < brefs; r++)
        path = graph.mkbrefpath(path);

    var loffset = 10 + 30 + (lsvcs * 40);
    path = path.line(40,path.ypos()).rcurve(-10,0,0,-10).line(path.xpos(), 20 + loffset);
    for (var s = 0; s < lsvcs; s++)
        path = graph.mklsvcpath(path);

    path = path.line(30,40).rcurve(0,-10,10,0);
    return path.end();
};

