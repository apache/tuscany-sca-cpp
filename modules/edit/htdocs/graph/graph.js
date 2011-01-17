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

var graph = new Object();

/**
 * Basic colors
 */
graph.colors = new Object();
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
     * Make a graph.
     */
    graph.mkgraph = function() {
        var div = document.createElement('div');
        div.id = 'vmldiv';
        document.body.appendChild(div);

        var vmlg = document.createElement('v:group');
        vmlg.style.width = 5000;
        vmlg.style.height = 5000;
        vmlg.coordsize = '5000,5000';
        div.appendChild(vmlg);

        graph.dragging = null;

        function draggable(n) {
            if (n == vmlg)
                return null;
            if (n.nodeName == 'group' && n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            if (n == vmlg)
                return null;
            n.parentNode.appendChild(n);
            return bringtotop(n.parentNode);
        }

        vmlg.onmousedown = function() {
            window.event.returnValue = false;
            graph.dragging = draggable(window.event.srcElement);
            if (graph.dragging == null)
                return false;
            bringtotop(graph.dragging);
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

            if (graph.dragging.id.substring(0, 8) == 'palette:') {
                // Clone an element dragged from the palette
                var clone = graph.compshape(graph.dragging.comp, mklist(), graph.mkpath().move(ui.posn(graph.dragging.style.left), ui.posn(graph.dragging.style.top)));
                graph.dragging.parentNode.appendChild(clone);
                graph.dragging = clone;
            }
            graph.dragging.setAttribute('coordorigin', newX + ' ' + newY);
            return false;
        };

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
     * Return a shape representing a component.
     */
    graph.compshape = function(comp, cassoc, pos) {
        var title = graph.comptitle(comp);

        var d = graph.comppath(comp, cassoc).str();

        var shape = document.createElement('v:shape');
        shape.style.width = 5000;
        shape.style.height = 5000;
        shape.coordsize = '5000,5000';
        shape.path = d;
        shape.fillcolor = graph.color(comp);
        shape.stroked = 'false';

        var contour = document.createElement('v:shape');
        contour.style.width = 5000;
        contour.style.height = 5000;
        contour.coordsize = '5000,5000';
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
        g.style.width = 5000;
        g.style.height = 5000;
        g.coordsize = '5000,5000';
        g.style.left = pos.xpos();
        g.style.top = pos.ypos();
        g.appendChild(shape);
        shape.appendChild(title);
        g.appendChild(contour)

        // Store the component in the shape
        g.comp = comp;

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
     * Return a shape representing a button.
     */
    graph.mkbutton = function(t, pos) {
        var title = graph.mktitle(t, true, pos);
        var d = graph.buttonpath().str();

        var shape = document.createElement('v:shape');
        shape.style.width = 5000;
        shape.style.height = 5000;
        shape.coordsize = '5000,5000';
        shape.path = d;
        shape.fillcolor = graph.colors.blue;
        shape.stroked = 'false';

        var contour = document.createElement('v:shape');
        contour.style.width = 5000;
        contour.style.height = 5000;
        contour.coordsize = '5000,5000';
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

} else {

    /**
     * SVG rendering.
     */
    graph.svgns='http://www.w3.org/2000/svg';

    /**
     * Make a graph.
     */
    graph.mkgraph = function(pos) {
        var div = document.createElement('div');
        div.id = 'svgdiv';
        div.style.position = 'absolute';
        div.style.left = pos.xpos();
        div.style.top = pos.ypos();
        // -webkit-user-select: none;
        document.body.appendChild(div);

        var svg = document.createElementNS(graph.svgns, 'svg');
        svg.style.height = 5000;
        svg.style.width = 5000;
        div.appendChild(svg);

        graph.dragging = null;

        function draggable(n) {
            if (n == svg)
                return null;
            if (n.nodeName == 'g' && n.id != '')
                return n;
            return draggable(n.parentNode);
        }

        function bringtotop(n) {
            if (n == svg)
                return null;
            n.parentNode.appendChild(n);
            return bringtotop(n.parentNode);
        }

        svg.onmousedown = function(e) {
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;
            graph.dragging = draggable(e.target);
            if (graph.dragging == null)
                return false;
            bringtotop(graph.dragging);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            graph.dragX = pos.screenX;
            graph.dragY = pos.screenY;
            return false;
        };

        svg.ontouchstart = svg.onmousedown;

        window.onmouseup = function(e) {
            if (graph.dragging == null)
                return false;
            graph.dragging = null;
            return false;
        };

        window.top.onmouseup = window.onmouseup;
        window.ontouchend = window.onmouseup;
        window.gestureend = window.onmouseup;
        window.top.gestureend = window.onmouseup;
        window.top.ontouchend = window.onmouseup;
        window.ontouchcancel = window.onmouseup;
        window.top.ontouchcancel = window.onmouseup;

        window.onmousemove = function(e) {
            if (graph.dragging == null)
                return false;
            if (e.preventDefault)
                e.preventDefault();
            else
                e.returnValue = false;
            var pmatrix = graph.dragging.parentNode.getCTM();
            var matrix = graph.dragging.getCTM();
            var curX = pmatrix != null? (Number(matrix.e) - Number(pmatrix.e)): Number(matrix.e);
            var curY = pmatrix != null? (Number(matrix.f) - Number(pmatrix.f)): Number(matrix.f);
            var pos = typeof e.touches != "undefined" ? e.touches[0] : e;
            var newX = curX + (pos.screenX - graph.dragX);
            var newY = curY + (pos.screenY - graph.dragY);
            if (newX >= 0)
                graph.dragX = pos.screenX;
            else
                newX = 0;
            if (newY >= 0)
                graph.dragY = pos.screenY;
            else
                newY = 0;

            if (graph.dragging.id.substring(0, 8) == 'palette:') {
                // Clone an element dragged from the palette
                var clone = graph.compshape(graph.dragging.comp, mklist(), graph.mkpath());
                graph.dragging.parentNode.appendChild(clone);
                graph.dragging = clone;
            }
            graph.dragging.setAttribute('transform', 'translate(' + newX + ',' + newY + ')');
            return false;
        };

        window.top.onmousemove = window.onmousemove;
        window.ontouchmove = window.onmousemove;
        window.top.ontouchmove = window.onmousemove;

        graph.titlewidthsvg = document.createElementNS(graph.svgns, 'svg');
        graph.titlewidthsvg.style.visibility = 'hidden';
        graph.titlewidthsvg.style.height = 0;
        graph.titlewidthsvg.style.width = 0;
        div.appendChild(graph.titlewidthsvg);

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
     * Return a shape representing a component.
     */
    graph.compshape = function(comp, cassoc, pos) {
        var title = graph.comptitle(comp);

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

        // Store the component in the shape.
        g.comp = comp;

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
     * Return a shape representing a button.
     */
    graph.mkbutton = function(t, pos) {
        var title = graph.mktitle(t, true);
        var d = graph.buttonpath().str();

        var shape = document.createElementNS(graph.svgns, 'path');
        shape.setAttribute('d', d);
        shape.setAttribute('fill', graph.colors.blue);

        var contour = document.createElementNS(graph.svgns, 'path');
        contour.setAttribute('d', d);
        contour.setAttribute('fill', 'none');
        contour.setAttribute('stroke', graph.colors.gray);
        contour.setAttribute('stroke-width', '4');
        contour.setAttribute('stroke-opacity', '0.20');
        contour.setAttribute('transform', 'translate(1,1)');

        var g = document.createElementNS(graph.svgns, 'g');
        g.setAttribute('transform', 'translate(' + pos.xpos() + ',' + pos.ypos() + ')');
        g.appendChild(shape);
        g.appendChild(contour);
        g.appendChild(title);
        return g;
    };
}

/**
 * Return the title of a SCDL element.
 */
graph.title = function(e) {
    var d = scdl.documentation(e);
    return d != null? d : scdl.name(e);
};

/**
 * Return the services and references of a component.
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

graph.brefs = function(comp) {
    return memo(comp, 'brefs', function() {
        return filter(function(r) { return scdl.align(r) == 'bottom'; }, scdl.references(comp));
    });
};

graph.rrefs = function(comp) {
    return memo(comp, 'rrefs', function() {
        return filter(function(r) { var a = scdl.align(r); return a == null || a == 'right'; }, scdl.references(comp));
    });
};

/**
 * Return the color of a component.
 */
graph.color = function(comp) {
    return memo(comp, 'color', function() {
        var c = scdl.color(comp);
        return c == null? graph.colors.blue : graph.colors[c];
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
 * Return the height of a component.
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
    var ypos = path.ypos();
    return path.rline(0,10).rline(0,10).rcurve(0,5,-5,0).rcurve(-5,0,0,-5).rcurve(0,-5,-5,0).rcurve(-5,0,0,5).rline(0,20).rcurve(0,5,5,0).rcurve(5,0,0,-5).rcurve(0,-5,5,0).rcurve(5,0,0,5).line(path.xpos(),ypos + height);
};

/**
 * Return a path representing a reference positioned at the bottom of a component.
 */
graph.brefpath = function(ref, cassoc, path) {
    var width = graph.brefwidth(ref, cassoc);
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

    var brefs = reverse(graph.brefs(comp));
    var boffset = 10 + graph.brefswidth(brefs, cassoc);
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
 * Return a path representing a button.
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
 * Render a composite.
 */
graph.composite = function(compos, pos) {
    var name = scdl.name(scdl.composite(compos));
    var comps = scdl.components(compos);
    var cassoc = scdl.nameToElementAssoc(comps);
    var proms = scdl.promotions(compos);

    function rendercomp(comp, cassoc, pos) {
        function renderrrefs(refs, cassoc, pos) {
            function renderrref(ref, cassoc, pos) {
                var target = assoc(scdl.target(ref), cassoc);
                if (isNil(target))
                    return mklist();
                return rendercomp(cadr(target), cassoc, pos);
            }

            function rendermove(ref, cassoc, pos) {
                return pos.clone().rmove(0, graph.rrefheight(ref, cassoc));
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

            function rendermove(ref, cassoc, pos) {
                return pos.clone().rmove(graph.brefwidth(ref, cassoc), 0);
            }

            if (isNil(refs))
                return mklist();
            return append(renderbref(car(refs), cassoc, pos), renderbrefs(cdr(refs), cassoc, rendermove(car(refs), cassoc, pos)));
        }

        var gcomp = graph.compshape(comp, cassoc, pos);

        var rrefs = graph.rrefs(comp);
        var rpos = graph.mkpath().rmove(graph.compwidth(comp, cassoc), 0);
        appendNodes(renderrrefs(rrefs, cassoc, rpos), gcomp);

        var brefs = graph.brefs(comp);
        var bpos = graph.mkpath().rmove(0 , graph.compheight(comp, cassoc));
        appendNodes(renderbrefs(brefs, cassoc, bpos), gcomp);

        return mklist(gcomp);
    }

    function renderproms(svcs, cassoc, pos) {
        function promcomp(svc, cassoc) {
            var c = assoc(scdl.promote(svc), cassoc);
            if (isNil(c))
                return mklist();
            return cadr(c);
        }

        function comppos(comp, pos) {
            var x = scdl.x(comp);
            var y = scdl.y(comp);
            return graph.mkpath().move(x != null? x : pos.xpos(), y != null? y : pos.ypos());
        }

        function rendermove(comp, cassoc, pos) {
            return pos.clone().rmove(0, graph.compclosureheight(comp, cassoc) + 40);
        }

        if (isNil(svcs))
            return mklist();

        var comp = promcomp(car(svcs), cassoc);
        if (isNil(comp))
            return renderproms(cdr(svcs), cassoc, rendermove(car(svcs), cassoc, pos));

        var cpos = comppos(comp, pos);
        return append(rendercomp(comp, cassoc, cpos), renderproms(cdr(svcs), cassoc, rendermove(comp, cassoc, cpos)));
    }

    var rproms = renderproms(proms, cassoc, pos.clone().rmove(20,20));

    if (name == 'palette')
        return map(function(r) {
                r.id = 'palette:' + r.id;
                return r;
            }, rproms);
    return rproms;
};

