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

/**
 * Detect browser VML support.
 */
function supportsVML() {
    if (typeof supportsVML.supported != 'undefined')
        return supportsVML.supported;
    supportsVML.supported = navigator.appName == 'Microsoft Internet Explorer';
    return supportsVML.supported;
}

/**
 * Detect browser SVG support.
 */
function supportsSVG() {
    if (typeof supportsSVG.supported != 'undefined')
        return supportsSVG.supported;
    supportsSVG.supported = navigator.appName != 'Microsoft Internet Explorer';
    return supportsSVG.supported;
}

/**
 * Basic colors
 */
var red = 'red';
var green = 'green';
var blue = 'blue';
var yellow = 'yellow';
var orange = '#ffa500';
var gray = 'gray'

/**
 * Base path class.
 */
function BasePath() {
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
}

/**
 * Rendering functions that work both with VML and SVG.
 */
var mkgraph;
var mkcompshape;
var graph;

/**
 * VML rendering.
 */
if (supportsVML()) {

    var vmlns='urn:schemas-microsoft-com:vml';

    /**
     * Make a shape path.
     */
    var mkpath = function() {
        function Path() {
            this.BasePath = BasePath;
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
    }

    /**
     * Make a title element.
     */
    var mktitle = function(t) {
        var title = document.createElement('v:textbox');
        title.style.left = '40';
        title.style.top = '30';
        title.style.position = 'absolute';
        var tnode = document.createTextNode(t);
        title.appendChild(tnode);
        return title;
    }

    /**
     * Return the width of a title.
     */
    var textWidthDiv;

    var titlewidth = function(t) {
        textWidthDiv.innerHTML = t;
        var twidth = textWidthDiv.offsetWidth + 10;
        textWidthDiv.innerHTML = '';
        return twidth;
    }

    /**
     * Make a component shape.
     */
    mkcompshape = function(name, color, tsvcs, lsvcs, brefs, rrefs) {
        var title = mktitle(name);
        var twidth = titlewidth(name);

        var d = mkcomppath(twidth, tsvcs, lsvcs, brefs, rrefs).str();

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
        contour.strokecolor = gray;
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
    }

    /**
     * Drag and drop support.
     */
    var dragX;
    var dragY;
    var dragging = null;

    /**
     * Make a graph.
     */
    mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'vmldiv';
        document.body.appendChild(div);

        textWidthDiv = document.createElement('span');
        textWidthDiv.style.visibility = 'hidden'
        div.appendChild(textWidthDiv);

        var vmlg = document.createElement('v:group');
        vmlg.style.width = 500;
        vmlg.style.height = 500;
        vmlg.coordsize = '500,500';
        div.appendChild(vmlg);

        function draggable(n) {
            if (n == vmlg)
                return null;
            if (n.nodeName == 'group')
                return n;
            return draggable(n.parentNode);
        }

        vmlg.onmousedown = function() {
            window.event.returnValue = false;
            dragging = draggable(window.event.srcElement);
            if (dragging == null)
                return false;
            dragging.parentNode.appendChild(dragging);
            dragX = window.event.clientX;
            dragY = window.event.clientY;
            vmlg.setCapture();
            return false;
        };

        vmlg.onmouseup = function() {
            if (dragging == null)
                return false;
            dragging = null;
            vmlg.releaseCapture();
            return false;
        };

        vmlg.onmousemove = function() {
            if (dragging == null)
                return false;
            var origX = dragging.coordorigin.X;
            var origY = dragging.coordorigin.Y;
            var newX = origX - (window.event.clientX - dragX);
            var newY = origY - (window.event.clientY - dragY);
            dragX = window.event.clientX;
            dragY = window.event.clientY;
            dragging.setAttribute('coordorigin', newX + ' ' + newY);
            return false;
        };

        graph = vmlg;
        return vmlg;
    }

    document.write('<xml:namespace ns="urn:schemas-microsoft-com:vml" prefix="v" />');
}

/**
 * SVG rendering.
 */
if (supportsSVG()) {

    var svgns='http://www.w3.org/2000/svg';

    /**
     * Make a shape path.
     */
    var mkpath = function() {
        function Path() {
            this.BasePath = BasePath;
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
    }

    /**
     * Make a title element.
     */
    var mktitle = function(t) {
        var title = document.createElementNS(svgns, 'text');
        title.setAttribute('text-anchor', 'start');
        title.setAttribute('x', 40);
        title.setAttribute('y', 50);
        title.appendChild(document.createTextNode(t));
        graph.appendChild(title);
        return title;
    }

    /**
     * Make a component shape.
     */
    mkcompshape = function(name, color, tsvcs, lsvcs, brefs, rrefs) {
        var title = mktitle(name);
        var twidth = title.getBBox().width;

        var d = mkcomppath(twidth, tsvcs, lsvcs, brefs, rrefs).str();

        var shape = document.createElementNS(svgns, 'path');
        shape.setAttribute('d', d);
        shape.setAttribute('fill', color);

        var contour = document.createElementNS(svgns, 'path');
        contour.setAttribute('d', d);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        var g = document.createElementNS(svgns, 'g');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    }

    /**
     * Drag and drop support.
     */
    var dragX;
    var dragY;
    var dragging = null;

    /**
     * Make a graph.
     */
    mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'svgdiv';
        document.body.appendChild(div);

        var svg = document.createElementNS(svgns, 'svg');
        svg.style.height = '100%';
        svg.style.width = '100%';
        div.appendChild(svg);

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
            dragging = draggable(e.target);
            if (dragging == null)
                return false;
            dragging.parentNode.appendChild(dragging);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            dragX = pos.clientX;
            dragY = pos.clientY;
            return false;
        };

        svg.ontouchstart = svg.onmousedown;

        svg.onmouseup = function(e) {
            if (dragging == null)
                return false;
            dragging = null;
            return false;
        };

        svg.ontouchend = svg.onmouseup;

        svg.onmousemove = function(e) {
            if (dragging == null)
                return false;
            var matrix = dragging.getCTM();
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = Number(matrix.e) + (pos.clientX - dragX);
            var newY = Number(matrix.f) + (pos.clientY - dragY);
            dragX = pos.clientX;
            dragY = pos.clientY;
            dragging.setAttribute('transform', 'translate(' + newX + ',' + newY + ')');
            return false;
        };

        svg.ontouchmove = svg.onmousemove;

        graph = svg;
        return svg;
    }
}

/**
 * Make a reference shape path, positioned to the right of a component shape.
 */
function mkrrefpath(path) {
    return path.rline(0,10).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rline(0,20).rcurve(0,5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,5).rline(0,10);
}

/**
 * Make a reference shape path, positioned at the bottom of a component shape.
 */
function mkbrefpath(path) {
    return path.rline(-10,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rline(-20,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rline(-10,0);
}

/**
 * Make a service shape path, positioned to the left of a component shape.
 */
function mklsvcpath(path) {
    return path.rline(0,-10).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rline(0,-20).rcurve(0,-5,5,0).rcurve(5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,-5).rline(0,-10);
}

/**
 * Make a service shape path, positioned at the top of a component shape.
 */
function mktsvcpath(path) {
    return path.rline(10,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rline(20,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rline(10,0);
}


/**
* Make a component shape path.
*/
function mkcomppath(twidth, tsvcs, lsvcs, brefs, rrefs) {
    var height = Math.max(lsvcs, rrefs) * 40 + 60;
    var width = Math.max(Math.max(tsvcs, brefs) * 40 + 60, twidth + 20);

    var path = mkpath().rmove(40,30).rline(20,0);
    for (var s = 0; s < tsvcs; s++)
        path = mktsvcpath(path);

    path = path.line(20 + width,path.ypos()).rcurve(10,0,0,10).rline(0,20);
    for (var r = 0; r < rrefs; r++)
        path = mkrrefpath(path);

    var boffset = 10 + 30 + (brefs * 40);
    path = path.line(path.xpos(),20 + height).rcurve(0,10,-10,0).line(20 + boffset,path.ypos());
    for (var r = 0; r < brefs; r++)
        path = mkbrefpath(path);

    var loffset = 10 + 30 + (lsvcs * 40);
    path = path.line(40,path.ypos()).rcurve(-10,0,0,-10).line(path.xpos(), 20 + loffset);
    for (var s = 0; s < lsvcs; s++)
        path = mklsvcpath(path);

    path = path.line(30,40).rcurve(0,-10,10,0);
    return path.end();
}

