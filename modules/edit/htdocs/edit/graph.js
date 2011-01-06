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
graph.colors = new Object();
graph.colors.black = '#000000';
graph.colors.blue = '#0000ff';
graph.colors.cyan = '#00ffff';
graph.colors.gray = '#808080'
graph.colors.green = '#008000';
graph.colors.magenta = '#ff00ff';
graph.colors.orange = '#ffa500';
graph.colors.pink = '#ffc0cb';
graph.colors.purple = '#800080';
graph.colors.red = '#ff0000';
graph.colors.white = '#ffffff';
graph.colors.yellow = '#ffff00';

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
if (graph.supportsVML()) {

    graph.vmlns='urn:schemas-microsoft-com:vml';
    document.write('<xml:namespace ns="urn:schemas-microsoft-com:vml" prefix="v" />');

    /**
     * Make a graph.
     */
    graph.mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'vmldiv';
        document.body.appendChild(div);

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

        graph.textWidthDiv = document.createElement('span');
        graph.textWidthDiv.style.visibility = 'hidden'
        div.appendChild(graph.textWidthDiv);

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
     * Return an element representing the title of a component.
     */
    graph.title = function(comp) {
        var t = scdl.name(comp);
        var tsvcs = graph.tsvcs(comp);
        var lsvcs = graph.lsvcs(comp);
        var title = document.createElement('v:textbox');
        title.style.left = '' + (isNil(lsvcs)? 5 : 25);
        title.style.top = '' + (isNil(tsvcs)? 5 : 25);
        title.style.position = 'absolute';
        var tnode = document.createTextNode(t);
        title.appendChild(tnode);
        return title;
    };

    /**
     * Return the width of the title of a component.
     */
    graph.titlewidth = function(comp) {
        var t = scdl.name(comp);
        graph.textWidthDiv.innerHTML = t;
        var twidth = graph.textWidthDiv.offsetWidth;
        graph.textWidthDiv.innerHTML = '';
        return twidth;
    };

    /**
     * Return a shape representing a component.
     */
    graph.compshape = function(comp, cassoc, pos) {
        var title = graph.title(comp);

        var d = graph.comppath(comp, cassoc).str();

        var shape = document.createElement('v:shape');
        shape.style.width = 500;
        shape.style.height = 500;
        shape.coordsize = '500,500';
        shape.path = d;
        shape.fillcolor = graph.color(comp);
        shape.stroked = 'false';

        var contour = document.createElement('v:shape');
        contour.style.width = 500;
        contour.style.height = 500;
        contour.coordsize = '500,500';
        contour.setAttribute('path', d);
        contour.filled = 'false';
        contour.strokecolor = graph.colors.gray;
        contour.strokeweight = '2';
        contour.style.left = 1;
        contour.style.top = 1;
        var stroke = document.createElement('v:stroke');
        stroke.opacity = '20%';
        contour.appendChild(stroke);

        var g = document.createElement('v:group');
        g.id = scdl.name(comp);
        g.style.width = 500;
        g.style.height = 500;
        g.coordsize = '500,500';
        g.style.left = pos.xpos();
        g.style.top = pos.ypos();
        g.appendChild(shape);
        shape.appendChild(title);
        g.appendChild(contour)
        return g;
    };
}

/**
 * SVG rendering.
 */
if (graph.supportsSVG()) {

    graph.svgns='http://www.w3.org/2000/svg';

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
            var pmatrix = graph.dragging.parentNode.getCTM();
            var matrix = graph.dragging.getCTM();
            var curX = pmatrix != null? (Number(matrix.e) - Number(pmatrix.e)): Number(matrix.e);
            var curY = pmatrix != null? (Number(matrix.f) - Number(pmatrix.f)): Number(matrix.f);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = curX + (pos.clientX - graph.dragX);
            var newY = curY + (pos.clientY - graph.dragY);
            graph.dragX = pos.clientX;
            graph.dragY = pos.clientY;
            graph.dragging.setAttribute('transform', 'translate(' + newX + ',' + newY + ')');
            return false;
        };

        svg.ontouchmove = svg.onmousemove;

        graph.textWidthSvg = document.createElementNS(graph.svgns, 'svg');
        graph.textWidthSvg.style.visibility = 'hidden';
        graph.textWidthSvg.style.height = '0px';
        graph.textWidthSvg.style.width = '0px';
        div.appendChild(graph.textWidthSvg);

        return svg;
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
     * Return an element representing the title of a component.
     */
    graph.title = function(comp) {
        var t = scdl.name(comp);
        var title = document.createElementNS(graph.svgns, 'text');
        title.setAttribute('text-anchor', 'start');
        title.setAttribute('x', 5);
        title.setAttribute('y', 15);
        title.appendChild(document.createTextNode(t));
        graph.textWidthSvg.appendChild(title);
        return title;
    };

    /**
     * Return the width of the title of a component.
     */
    graph.titlewidth = function(comp) {
        var title = graph.title(comp);
        var width = title.getBBox().width;
        graph.textWidthSvg.removeChild(title);
        return width;
    };

    /**
     * Return a shape representing a component.
     */
    graph.compshape = function(comp, cassoc, pos) {
        var title = graph.title(comp);

        var d = graph.comppath(comp, cassoc).str();

        var shape = document.createElementNS(graph.svgns, 'path');
        shape.setAttribute('d', d);
        shape.setAttribute('fill', graph.color(comp));

        var contour = document.createElementNS(graph.svgns, 'path');
        contour.setAttribute('d', d);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', graph.colors.gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        var g = document.createElementNS(graph.svgns, 'g');
        g.id = scdl.name(comp);
        g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    };
}

/**
 * Return the services and references of a component.
 */
graph.tsvcs = function(comp) {
    return filter(function(s) { return scdl.align(s) == 'top'; }, scdl.services(comp));
};

graph.lsvcs = function(comp) {
    var svcs = scdl.services(comp);
    if (isNil(svcs))
        return mklist("'element","'service","'attribute","'name",scdl.name(comp));
    var l = filter(function(s) { var a = scdl.align(s); return a == null || a == 'left'; }, scdl.services(comp));
    return l;
};

graph.brefs = function(comp) {
    return filter(function(r) { return scdl.align(r) == 'bottom'; }, scdl.references(comp));
};

graph.rrefs = function(comp) {
    return filter(function(r) { var a = scdl.align(r); return a == null || a == 'right'; }, scdl.references(comp));
};

/**
 * Return the color of a component.
 */
graph.color = function(comp) {
    var c = scdl.color(comp);
    return c == null? graph.colors.blue : graph.colors[c];
}

/**
 * Return the height of a reference.
 */
graph.refheight = function(ref, cassoc) {
    var target = assoc(scdl.target(ref), cassoc);
    if (isNil(target))
        return 60;
    return graph.compheight(cadr(target), cassoc);
}

/**
 * Return the total height of a list of references.
 */
graph.refsheight = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return graph.refheight(car(refs), cassoc) + graph.refsheight(cdr(refs), cassoc);
}

/**
 * Return the height of a component.
 */
graph.compheight = function(comp, cassoc) {
    var lsvcs = graph.lsvcs(comp);
    var lsvcsh = Math.max(1, length(lsvcs)) * 60 + 20;
    var rrefs = graph.rrefs(comp);
    var rrefsh = graph.refsheight(rrefs, cassoc) + 20;
    var height = Math.max(lsvcsh, rrefsh);
    return height;
};

/**
 * Return the width of a reference.
 */
graph.refwidth = function(ref, cassoc) {
    var target = assoc(scdl.target(ref), cassoc);
    if (isNil(target))
        return 60;
    return graph.compwidth(cadr(target), cassoc);
}

/**
 * Return the total width of a list of references.
 */
graph.refswidth = function(refs, cassoc) {
    if (isNil(refs))
        return 0;
    return graph.refwidth(car(refs), cassoc) + graph.refswidth(cdr(refs), cassoc);
}

/**
 * Return the width of a component.
 */
graph.compwidth = function(comp, cassoc) {
    var twidth = graph.titlewidth(comp) + 20;
    var tsvcs = graph.tsvcs(comp);
    var tsvcsw = Math.max(1, length(tsvcs)) * 60 + 20;
    var brefs = graph.brefs(comp);
    var brefsw = graph.refswidth(brefs, cassoc) + 20;
    var width = Math.max(twidth, Math.max(tsvcsw, brefsw));
    return width;
};

/**
 * Return a path representing a reference positioned to the right of a component.
 */
graph.rrefpath = function(ref, cassoc, path) {
    var height = graph.refheight(ref, cassoc);
    var ypos = path.ypos();
    return path.rline(0,10).rline(0,10).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rline(0,20).rcurve(0,5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,5).line(path.xpos(),ypos + height);
};

/**
 * Return a path representing a reference positioned at the bottom of a component.
 */
graph.brefpath = function(ref, cassoc, path) {
    var width = graph.refwidth(ref, cassoc);
    var xpos = path.xpos();
    return path.line(xpos - width + 60,path.ypos()).rline(-10,0).rline(-10,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rline(-20,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).line(xpos - width,path.ypos());
};

/**
 * Return a path representing a service positioned to the left of a component.
 */
graph.lsvcpath = function(svc, cassoc, path) {
    var height = 60;
    var ypos = path.ypos();
    return path.rline(0,-10).rline(0, -10).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rline(0,-20).rcurve(0,-5,5,0).rcurve(5,0,0,5).rcurve(0,5,5,0).rcurve(5,0,0,-5).line(path.xpos(), ypos - height);
};

/**
 * Return a path representing a service positioned at the top of a component.
 */
graph.tsvcpath = function(svc, cassoc, path) {
    var width = 60;
    var xpos = path.xpos();
    return path.rline(10,0).rline(10,0).rcurve(5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,5,0).rline(20,0).rcurve(5,0,0,5).rcurve(0,5,-5,0).rcurve(-5,0,0,5).rcurve(0,5,5,0).line(xpos + width,path.ypos());
};

/**
* Return a path representing a component.
*/
graph.comppath = function(comp, cassoc) {
    var height = graph.compheight(comp, cassoc);
    var width = graph.compwidth(comp, cassoc);

    function renderpath(x, f, cassoc, path) {
        if (isNil(x))
            return path;
        return renderpath(cdr(x), f, cassoc, f(car(x), cassoc, path));
    }

    var tsvcs = graph.tsvcs(comp);
    var path = graph.mkpath().move(10,0);
    path = renderpath(tsvcs, graph.tsvcpath, cassoc, path);

    var rrefs = graph.rrefs(comp);
    path = path.line(width - 10,path.ypos()).rcurve(10,0,0,10);
    path = renderpath(rrefs, graph.rrefpath, cassoc, path);

    var brefs = graph.brefs(comp);
    var boffset = 10 + graph.refswidth(brefs, cassoc);
    path = path.line(path.xpos(),height - 10).rcurve(0,10,-10,0).line(boffset, path.ypos());
    path = renderpath(brefs, graph.brefpath, cassoc, path);

    var lsvcs = graph.lsvcs(comp);
    var loffset = 10 + (length(lsvcs) * 60);
    path = path.line(10,path.ypos()).rcurve(-10,0,0,-10).line(path.xpos(), loffset);
    path = renderpath(lsvcs, graph.lsvcpath, cassoc, path);

    path = path.line(0,10).rcurve(0,-10,10,0);
    return path.end();
};

/**
 * Render a composite.
 */
graph.composite = function(compos) {
    var name = scdl.name(compos);
    var comps = scdl.components(compos);
    var cassoc = scdl.nameToElementAssoc(comps);
    var proms = scdl.promotions(compos);

    function rendercomp(comp, cassoc, pos) {
        function appendg(nodes, parent) {
            if (isNil(nodes))
                return parent;
            parent.appendChild(car(nodes));
            return appendg(cdr(nodes), parent);
        }

        function renderrrefs(refs, cassoc, pos) {
            function renderrref(ref, cassoc, pos) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return mklist();
                return rendercomp(cadr(target), cassoc, pos);
            }

            function rendermove(ref, cassoc, pos) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return pos;
                return pos.clone().rmove(0, graph.compheight(cadr(target), cassoc));
            }

            if (isNil(refs))
                return mklist();
            return append(renderrref(car(refs), cassoc, pos), renderrrefs(cdr(refs), cassoc, rendermove(car(refs), cassoc, pos)));
        }

        function renderbrefs(refs, cassoc, pos) {
            function renderbref(ref, cassoc, pos) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return mklist();
                return rendercomp(cadr(target), cassoc, pos);
            }

            if (isNil(refs))
                return mklist();
            return append(renderbref(car(refs), cassoc, pos), renderbrefs(cdr(refs), cassoc, pos));
        }

        var gcomp = graph.compshape(comp, cassoc, pos);
        var rrefs = graph.rrefs(comp);
        var rpos = graph.mkpath().rmove(graph.compwidth(comp, cassoc), 0);
        appendg(renderrrefs(rrefs, cassoc, rpos), gcomp);
        var brefs = graph.brefs(comp);
        appendg(renderbrefs(brefs, cassoc, rpos), gcomp);
        return mklist(gcomp);
    }

    function renderproms(svcs, cassoc, pos) {
        function renderprom(svc, cassoc, pos) {
            var comp = assoc(scdl.promote(svc), cassoc);
            if (isNil(comp))
                return mklist();
            return rendercomp(cadr(comp), cassoc, pos);
        }

        function rendermove(svc, cassoc, pos) {
            var comp = assoc(scdl.promote(svc), cassoc);
            if (isNil(comp))
                return pos;
            return pos.clone().rmove(0, graph.compheight(cadr(comp), cassoc) + 20);
        }

        if (isNil(svcs))
            return mklist();
        return append(renderprom(car(svcs), cassoc, pos), renderproms(cdr(svcs), cassoc, rendermove(car(svcs), cassoc, pos)));
    }

    var rcomps = renderproms(proms, cassoc, graph.mkpath().rmove(20,20));
    return rcomps;
};

