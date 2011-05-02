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
function cons(car,cdr){var a=new Array();a.push(car);return a.concat(cdr);}
function car(l){return l[0];}
function first(l){return car(l);}
function cdr(l){return l.slice(1);}
function rest(l){return cdr(l);}
function cadr(l){return car(cdr(l));}
function cddr(l){return cdr(cdr(l));}
function caddr(l){return car(cddr(l));}
function cdddr(l){return cdr(cdr(cdr(l)));}
function cadddr(l){return car(cdddr(l));}
function append(a,b){return a.concat(b);}
function reverse(l){return l.slice(0).reverse();}
function range(a,b){var l=new Array();for(var x=a;x<b;x++)
l.push(x);return l;}
function isNil(v){if(v==null||typeof v=='undefined'||(v.constructor==Array&&v.length==0))
return true;return false;}
function isSymbol(v){if(typeof v=='string'&&v.slice(0,1)=="'")
return true;return false;}
function isString(v){if(typeof v=='string'&&v.slice(0,1)!="'")
return true;return false;}
function isList(v){if(v!=null&&typeof v!='undefined'&&v.constructor==Array)
return true;return false;}
function isTaggedList(v,t){if(isList(v)&&!isNil(v)&&car(v)==t)
return true;return false;}
var emptylist=new Array();function mklist(){if(arguments.length==0)
return emptylist;var a=new Array();for(i=0;i<arguments.length;i++)
a[i]=arguments[i];return a;}
function length(l){return l.length;}
function assoc(k,l){if(isNil(l))
return mklist();if(k==car(car(l)))
return car(l);return assoc(k,cdr(l));}
function map(f,l){if(isNil(l))
return l;return cons(f(car(l)),map(f,cdr(l)));}
function filter(f,l){if(isNil(l))
return l;if(f(car(l)))
return cons(car(l),filter(f,cdr(l)));return filter(f,cdr(l));}
function reduce(f,i,l){if(isNil(l))
return i;return reduce(f,f(i,car(l)),cdr(l));}
function tokens(path){return filter(function(s){return length(s)!=0;},path.split("/"));}
var rconsole;function log(v){try{var s='';for(i=0;i<arguments.length;i++){s=s+writeValue(arguments[i]);if(i<arguments.length)
s=s+' ';}
if(rconsole){try{rconsole.log(s);}catch(e){}}
try{console.log(s);}catch(e){}}catch(e){}
return true;}
function debug(o){try{for(f in o){try{log('debug '+f+'='+o[f]);}catch(e){}}}catch(e){}
return true;}
var config;if(isNil(config))
config={}
function AssertException(){}
AssertException.prototype.toString=function(){return'AssertException';};function assert(exp){if(!exp)
throw new AssertException();}
function writeStrings(l){if(isNil(l))
return'';return car(l)+writeStrings(cdr(l));}
function writeValue(v){function writePrimitive(p){if(isSymbol(p))
return''+p.substring(1);if(isString(p))
return'"'+p+'"';return''+p;}
function writeList(l){if(isNil(l))
return'';return' '+writeValue(car(l))+writeList(cdr(l));}
if(!isList(v))
return writePrimitive(v);if(isNil(v))
return'()';return'('+writeValue(car(v))+writeList(cdr(v))+')';}
function memo(obj,key,f){if(!obj[memo])
obj.memo={};if(obj.memo[key])
return obj.memo[key];return obj.memo[key]=f();}
function unmemo(obj){obj.memo={};return true;}
function properties(o){var a=new Array();for(p in o)
a.push(p);return a;}
function domainname(host){var h=reverse(host.split('.'));return reverse(mklist(car(h),cadr(h))).join('.');}
function issubdomain(host){return host.split('.').length>2;}
function hometitle(host){if(!isNil(config.hometitle))
return config.hometitle;var h=reverse(host.split('.'));var d=isNil(cdr(h))?car(h):cadr(h);return d.substr(0,1).toUpperCase()+d.substr(1);}
function windowtitle(host){if(!isNil(config.windowtitle))
return config.windowtitle;var h=reverse(host.split('.'));var d=isNil(cdr(h))?car(h):cadr(h);return d.substr(0,1).toUpperCase()+d.substr(1);}
function format(){var i=0;var s='';for(a in arguments){s=i==0?arguments[a]:s.replace('{'+a+'}',arguments[a]);i++;}
return s;}
function setcar(l,v){l[0]=v;return l;}
function setcadr(l,v){l[1]=v;return l;}
function setcaddr(l,v){l[2]=v;return l;}
function setappend(a,b){if(isNil(b))
return a;a.push(car(b));return setappend(a,cdr(b));}
function setcdr(a,b){a.length=1;return setappend(a,b);}
function setlist(a,b){if(b==a)
return b;a.length=0;return setappend(a,b);}
var element="'element"
var attribute="'attribute"
var atsign="'@"
function isElement(v){if(!isList(v)||isNil(v)||car(v)!=element)
return false;return true;}
function isAttribute(v){if(!isList(v)||isNil(v)||car(v)!=attribute)
return false;return true;}
function attributeName(l){return cadr(l);}
function attributeValue(l){return caddr(l);}
function elementName(l){return cadr(l);}
function elementHasChildren(l){return!isNil(cddr(l));}
function elementChildren(l){return cddr(l);}
function elementHasValue(l){r=reverse(l);if(isSymbol(car(r)))
return false;if(isList(car(r))&&!isNil(car(r))&&isSymbol(car(car(r))))
return false;return true;}
function elementValue(l){return car(reverse(l));}
function elementToValueIsList(v){if(!isList(v))
return false;return isNil(v)||!isSymbol(car(v));}
function elementToValue(t){if(isTaggedList(t,attribute))
return mklist(atsign+attributeName(t).substring(1),attributeValue(t));if(isTaggedList(t,element)){if(elementHasValue(t)){if(!elementToValueIsList(elementValue(t)))
return mklist(elementName(t),elementValue(t));return cons(elementName(t),mklist(elementsToValues(elementValue(t))));}
return cons(elementName(t),elementsToValues(elementChildren(t)));}
if(!isList(t))
return t;return elementsToValues(t);}
function elementToValueIsSymbol(v){if(!isList(v))
return false;if(isNil(v))
return false;if(!isSymbol(car(v)))
return false;return true;}
function elementToValueGroupValues(v,l){if(isNil(l)||!elementToValueIsSymbol(v)||!elementToValueIsSymbol(car(l)))
return cons(v,l);if(car(car(l))!=car(v))
return cons(v,l);if(!elementToValueIsList(cadr(car(l)))){var g=mklist(car(v),mklist(cdr(v),cdr(car(l))));return elementToValueGroupValues(g,cdr(l));}
var g=mklist(car(v),cons(cdr(v),cadr(car(l))));return elementToValueGroupValues(g,cdr(l));}
function elementsToValues(e){if(isNil(e))
return e;return elementToValueGroupValues(elementToValue(car(e)),elementsToValues(cdr(e)));}
function valueToElement(t){if(isList(t)&&!isNil(t)&&isSymbol(car(t))){var n=car(t);var v=isNil(cdr(t))?mklist():cadr(t);if(!isList(v)){if(n.substring(0,2)==atsign)
return mklist(attribute,"'"+n.substring(2),v);return mklist(element,n,v);}
if(isNil(v)||!isSymbol(car(v)))
return cons(element,cons(n,mklist(valuesToElements(v))));return cons(element,cons(n,valuesToElements(cdr(t))));}
if(!isList(t))
return t;return valuesToElements(t);}
function valuesToElements(l){if(isNil(l))
return l;return cons(valueToElement(car(l)),valuesToElements(cdr(l)));}
function selector(s){function evalSelect(s,v){if(isNil(s))
return true;if(isNil(v))
return false;if(car(s)!=car(v))
return false;return evalSelect(cdr(s),cdr(v));}
return function(v){return evalSelect(s,v);};}
function namedAttribute(name,l){return memo(l,name,function(){var f=filter(function(v){return isAttribute(v)&&attributeName(v)==name;},l);if(isNil(f))
return null;return car(f);});}
function namedAttributeValue(name,l){var a=namedAttribute(name,l);if(a==null)
return null
return attributeValue(a);}
function namedElementChildren(name,l){return memo(l,name,function(){return filter(function(v){return isElement(v)&&elementName(v)==name;},l);});}
function namedElementChild(name,l){var f=namedElementChildren(name,l);if(isNil(f))
return null;return car(f);}
function setElement(l,e){setlist(l,e);l.memo={};}
function nodeList(n){var l=new Array();if(isNil(n))
return l;for(var i=0;i<n.length;i++)
l[i]=n[i];return l;}
function appendNodes(nodes,p){if(isNil(nodes))
return p;p.appendChild(car(nodes));return appendNodes(cdr(nodes),p);};function childAttributes(e){return filter(function(n){return n.nodeType==2;},nodeList(e.attributes));}
function childElements(e){return filter(function(n){return n.nodeType==1;},nodeList(e.childNodes));}
function childText(e){function trim(s){return s.replace(/^\s*/,'').replace(/\s*$/,'');}
return filter(function(n){return n.nodeType==3&&trim(n.nodeValue)!='';},nodeList(e.childNodes));}
function readAttributes(p,a){if(isNil(a))
return a;var x=car(a);return cons(mklist(attribute,"'"+x.nodeName,x.nodeValue),readAttributes(p,cdr(a)));}
function readElement(e,childf){var l=append(append(mklist(element,"'"+e.nodeName),readAttributes(e,childf(e))),readElements(childElements(e),childf));var t=childText(e);if(isNil(t))
return l;return append(l,mklist(car(t).nodeValue));}
function readElements(l,childf){if(isNil(l))
return l;return cons(readElement(car(l),childf),readElements(cdr(l),childf));}
function isXML(l){if(isNil(l))
return false;return car(l).substring(0,5)=='<?xml';}
function parseXML(l){var s=writeStrings(l);if(window.DOMParser){var p=new DOMParser();return p.parseFromString(s,"text/xml");}
var doc;try{doc=new ActiveXObject("MSXML2.DOMDocument");}catch(e){doc=new ActiveXObject("Microsoft.XMLDOM");}
doc.async='false';doc.loadXML(s);return doc;}
function readXMLDocument(doc){var root=childElements(doc);if(isNil(root))
return mklist();return mklist(readElement(car(root),childAttributes));}
function readXHTMLElement(xhtml){function ieChildAttributes(e){var a=filter(function(n){if(n.nodeType!=2||isNil(n.nodeValue)||n.nodeValue=='')
return false;if(n.nodeName=='contentEditable'||n.nodeName=='maxLength'||n.nodeName=='loop'||n.nodeName=='start')
return false;return true;},nodeList(e.attributes));if(e.style.cssText=='')
return a;var sa=new Object();sa.nodeName='style';sa.nodeValue=e.style.cssText;return cons(sa,a);}
var childf=(typeof(XMLSerializer)!='undefined')?childAttributes:ieChildAttributes;return mklist(readElement(xhtml,childf));}
function readXML(l){return readXMLDocument(parseXML(l));}
function writeXMLDocument(doc){if(typeof(XMLSerializer)!='undefined')
return mklist(new XMLSerializer().serializeToString(doc));return mklist(doc.xml);}
function expandElementValues(n,l){if(isNil(l))
return l;return cons(cons(element,cons(n,car(l))),expandElementValues(n,cdr(l)));}
function writeList(l,node,doc){if(isNil(l))
return node;var token=car(l);if(isTaggedList(token,attribute)){node.setAttribute(attributeName(token).substring(1),''+attributeValue(token));}else if(isTaggedList(token,element)){function mkelem(tok,doc){function xmlns(l){if(isNil(l))
return null;var t=car(l);if(isTaggedList(t,attribute)){if(attributeName(t).substring(1)=='xmlns')
return attributeValue(t);}
return xmlns(cdr(l));}
var ns=xmlns(elementChildren(tok));if(ns==null||!doc.createElementNS)
return doc.createElement(elementName(tok).substring(1));return doc.createElementNS(ns,elementName(tok).substring(1));}
if(elementHasValue(token)){var v=elementValue(token);if(isList(v)){var e=expandElementValues(elementName(token),v);writeList(e,node,doc);}else{var child=mkelem(token,doc);writeList(elementChildren(token),child,doc);node.appendChild(child);}}else{var child=mkelem(token,doc);writeList(elementChildren(token),child,doc);node.appendChild(child);}}else
node.appendChild(doc.createTextNode(''+token));writeList(cdr(l),node,doc);return node;}
function mkXMLDocument(){if(document.implementation&&document.implementation.createDocument)
return document.implementation.createDocument('','',null);return new ActiveXObject("MSXML2.DOMDocument");}
function writeXML(l,xmlTag){var doc=mkXMLDocument();writeList(l,doc,doc);if(!xmlTag)
return writeXMLDocument(doc);return mklist('<?xml version="1.0" encoding="UTF-8"?>\n'+writeXMLDocument(doc)+'\n');}
var atom={};atom.entryElementValues=function(e){var lt=filter(selector(mklist(element,"'title")),e);var t=isNil(lt)?'':elementValue(car(lt));var li=filter(selector(mklist(element,"'id")),e);var i=isNil(li)?'':elementValue(car(li));var lc=filter(selector(mklist(element,"'content")),e);return append(mklist(element,"'entry",mklist(element,"'title",t),mklist(element,"'id",i)),isNil(lc)?mklist():mklist(mklist(element,"'content",elementValue(car(lc)))))};atom.entriesElementValues=function(e){if(isNil(e))
return e;return cons(atom.entryElementValues(car(e)),atom.entriesElementValues(cdr(e)));};atom.isATOMEntry=function(l){if(!isXML(l))
return false;return car(l).match('<entry')!=null&&car(l).match('<feed')==null&&car(l).match('="http://www.w3.org/2005/Atom"')!=null;};atom.readATOMEntryDocument=function(doc){var e=readXMLDocument(doc);if(isNil(e))
return mklist();return mklist(atom.entryElementValues(car(e)));};atom.readATOMEntry=function(l){return atom.readATOMEntryDocument(parseXML(l));};atom.isATOMFeed=function(l){if(!isXML(l))
return false;return car(l).match('<feed')!=null&&car(l).match('="http://www.w3.org/2005/Atom"')!=null;};atom.readATOMFeedDocument=function(doc){var f=readXMLDocument(doc);if(isNil(f))
return mklist();var t=filter(selector(mklist(element,"'title")),car(f));var i=filter(selector(mklist(element,"'id")),car(f));var e=filter(selector(mklist(element,"'entry")),car(f));return mklist(append(mklist(element,"'feed",mklist(element,"'title",elementValue(car(t))),mklist(element,"'id",elementValue(car(i)))),atom.entriesElementValues(e)));};atom.readATOMFeed=function(l){return atom.readATOMFeedDocument(parseXML(l));};atom.entryElement=function(l){var title=elementValue(namedElementChild("'title",l));var id=elementValue(namedElementChild("'id",l));var content=namedElementChild("'content",l);var text=isNil(content)?false:elementHasValue(content);return append(append(mklist(element,"'entry",mklist(attribute,"'xmlns","http://www.w3.org/2005/Atom"),mklist(element,"'title",mklist(attribute,"'type","text"),title),mklist(element,"'id",id)),isNil(content)?mklist():append(mklist(element,"'content",mklist(attribute,"'type",text?"text":"application/xml")),text?mklist(elementValue(content)):elementChildren(content))),mklist(element,"'link",mklist(attribute,"'href",id)));};atom.entriesElements=function(l){if(isNil(l))
return l;return cons(atom.entryElement(car(l)),atom.entriesElements(cdr(l)));};atom.writeATOMEntry=function(ll){var l=isNil(ll)?ll:car(ll);return writeXML(mklist(atom.entryElement(l)),true);};atom.writeATOMFeed=function(ll){var l=isNil(ll)?ll:car(ll);var lt=filter(selector(mklist(element,"'title")),l);var t=isNil(lt)?'':elementValue(car(lt));var li=filter(selector(mklist(element,"'id")),l);var i=isNil(li)?'':elementValue(car(li));var f=mklist(element,"'feed",mklist(attribute,"'xmlns","http://www.w3.org/2005/Atom"),mklist(element,"'title",mklist(attribute,"'type","text"),car(l)),mklist(element,"'id",cadr(l)));var le=filter(selector(mklist(element,"'entry")),l);if(isNil(le))
return writeXML(mklist(f),true);if(!isNil(le)&&!isNil(car(le))&&isList(car(caddr(car(le))))){var fe=append(f,atom.entriesElements(caddr(car(le))));return writeXML(mklist(fe),true);}
var fe=append(f,atom.entriesElements(le));return writeXML(mklist(fe),true);};var json={};json.Exception=function(code,message){this.name="JSONException";this.code=code;this.message=message;};json.Exception.prototype=new Error();json.Exception.prototype.toString=function(){return this.name+": "+this.message;};json.isJSArray=function(l){if(isNil(l))
return true;var v=car(l);if(isSymbol(v))
return false;if(isList(v))
if(!isNil(v)&&isSymbol(car(v)))
return false;return true;};json.jsPropertiesToValues=function(propertiesSoFar,o,i){if(isNil(i))
return propertiesSoFar;var p=car(i);var jsv=o[p];var v=json.jsValToValue(jsv);if(typeof p=='string'){var n=''+p;if(n.slice(0,1)=='@')
return json.jsPropertiesToValues(cons(mklist(attribute,"'"+n.slice(1),v),propertiesSoFar),o,cdr(i));if(isList(v)&&!json.isJSArray(v))
return json.jsPropertiesToValues(cons(cons(element,cons("'"+n,v)),propertiesSoFar),o,cdr(i));return json.jsPropertiesToValues(cons(mklist(element,"'"+n,v),propertiesSoFar),o,cdr(i));}
return json.jsPropertiesToValues(cons(v,propertiesSoFar),o,cdr(i));};json.jsValToValue=function(jsv){if(jsv==null)
return null;if(isList(jsv))
return json.jsPropertiesToValues(mklist(),jsv,reverse(range(0,jsv.length)));if(typeof jsv=='object')
return json.jsPropertiesToValues(mklist(),jsv,reverse(properties(jsv)));if(typeof jsv=='string')
return''+jsv;return jsv;}
json.isJSON=function(l){if(isNil(l))
return false;var s=car(l).slice(0,1);return s=="["||s=="{";};json.readJSON=function(l){var s=writeStrings(l);var obj;eval('obj = { \"val\": '+s+" }");return json.jsValToValue(obj.val);};json.valuesToJSElements=function(a,l,i){if(isNil(l))
return a;var pv=json.valueToJSVal(car(l));a[i]=pv
return json.valuesToJSElements(a,cdr(l),i+1);};json.valueToJSVal=function(v){if(!isList(v))
return v;if(json.isJSArray(v))
return json.valuesToJSElements(range(0,v.length),v,0);return json.valuesToJSProperties({},v);};json.valuesToJSProperties=function(o,l){if(isNil(l))
return o;var token=car(l);if(isTaggedList(token,attribute)){var pv=json.valueToJSVal(attributeValue(token));o['@'+attributeName(token).slice(1)]=pv;}else if(isTaggedList(token,element)){if(elementHasValue(token)){var pv=json.valueToJSVal(elementValue(token));o[elementName(token).slice(1)]=pv;}else{var child={};o[elementName(token).slice(1)]=child;json.valuesToJSProperties(child,elementChildren(token));}}
return json.valuesToJSProperties(o,cdr(l));};json.writeJSON=function(l){var jsv;if(json.isJSArray(l))
jsv=json.valuesToJSElements(range(0,l.length),l,0);else
jsv=json.valuesToJSProperties({},l);var s=json.toJSON(jsv);return mklist(s);}
json.jsonRequest=function(id,func,params){var r=mklist(mklist("'id",id),mklist("'method",func),mklist("'params",params));return json.writeJSON(valuesToElements(r));};json.jsonResult=function(id,val){return json.writeJSON(valuesToElements(mklist(mklist("'id",id),mklist("'result",val))));};json.jsonResultValue=function(s){var jsres=json.readJSON(s);var res=elementsToValues(jsres);var val=cadr(assoc("'result",res));if(isList(val)&&!json.isJSArray(val))
return mklist(val);return val;};json.escapeJSONChar=function(c){if(c=="\""||c=="\\")return"\\"+c;if(c=="\b")return"\\b";if(c=="\f")return"\\f";if(c=="\n")return"\\n";if(c=="\r")return"\\r";if(c=="\t")return"\\t";var hex=c.charCodeAt(0).toString(16);if(hex.length==1)return"\\u000"+hex;if(hex.length==2)return"\\u00"+hex;if(hex.length==3)return"\\u0"+hex;return"\\u"+hex;};json.escapeJSONString=function(s){var parts=s.split("");for(var i=0;i<parts.length;i++){var c=parts[i];if(c=='"'||c=='\\'||c.charCodeAt(0)<32||c.charCodeAt(0)>=128)
parts[i]=json.escapeJSONChar(parts[i]);}
return"\""+parts.join("")+"\"";};json.toJSON=function(o){if(o==null)
return"null";if(o.constructor==String)
return json.escapeJSONString(o);if(o.constructor==Number)
return o.toString();if(o.constructor==Boolean)
return o.toString();if(o.constructor==Date)
return'{javaClass: "java.util.Date", time: '+o.valueOf()+'}';if(o.constructor==Array){var v=[];for(var i=0;i<o.length;i++)
v.push(json.toJSON(o[i]));return"["+v.join(", ")+"]";}
var v=[];for(attr in o){if(o[attr]==null)
v.push("\""+attr+"\": null");else if(typeof o[attr]=="function")
;else
v.push(json.escapeJSONString(attr)+": "+json.toJSON(o[attr]));}
return"{"+v.join(", ")+"}";};var scdl={};scdl.composite=function(l){var cs=namedElementChildren("'composite",l);if(isNil(cs))
return cs;return car(cs);};scdl.components=function(l){var cs=namedElementChildren("'composite",l);if(isNil(cs))
return cs;return namedElementChildren("'component",car(cs));};scdl.promotions=function(l){var cs=namedElementChildren("'composite",l);if(isNil(cs))
return cs;return namedElementChildren("'service",car(cs));};scdl.promote=function(l){var puri=namedAttributeValue("'promote",l);if(isNil(puri))
return puri;return car(tokens(puri));};scdl.name=function(l){return namedAttributeValue("'name",l);};scdl.documentation=function(l){var d=namedElementChildren("'documentation",l);if(isNil(d))
return null;if(!elementHasValue(car(d)))
return null;var v=elementValue(car(d));return v;};scdl.title=function(l){return namedAttributeValue("'t:title",l);};scdl.style=function(l){return namedAttributeValue("'t:style",l);};scdl.color=function(l){return namedAttributeValue("'t:color",l);};scdl.x=function(l){return namedAttributeValue("'t:x",l);};scdl.y=function(l){return namedAttributeValue("'t:y",l);};scdl.implementation=function(l){function filterImplementation(v){return isElement(v)&&cadr(v).match("implementation.")!=null;}
var n=filter(filterImplementation,l);if(isNil(n))
return null;return car(n);};scdl.implementationType=function(l){return elementName(l).substring(1);};scdl.uri=function(l){return namedAttributeValue("'uri",l);};scdl.align=function(l){return namedAttributeValue("'t:align",l);};scdl.visible=function(l){return namedAttributeValue("'t:visible",l);};scdl.clonable=function(l){return namedAttributeValue("'t:clonable",l);};scdl.services=function(l){return namedElementChildren("'service",l);};scdl.references=function(l){return namedElementChildren("'reference",l);};scdl.bindings=function(l){function filterBinding(v){return isElement(v)&&cadr(v).match("binding.")!=null;}
return filter(filterBinding,l);};scdl.bindingType=function(l){return elementName(l).substring(1);};scdl.target=function(l){function targetURI(){function bindingsTarget(l){if(isNil(l))
return null;var u=scdl.uri(car(l));if(!isNil(u))
return u;return bindingsTarget(cdr(l));}
var t=namedAttributeValue("'target",l);if(!isNil(t))
return t;return bindingsTarget(scdl.bindings(l));}
var turi=targetURI();if(isNil(turi))
return turi;return car(tokens(turi));};scdl.properties=function(l){return namedElementChildren("'property",l);};scdl.propertyValue=function(l){if(!elementHasValue(l))
return'';return elementValue(l);};scdl.nameToElementAssoc=function(l){if(isNil(l))
return l;return cons(mklist(scdl.name(car(l)),car(l)),scdl.nameToElementAssoc(cdr(l)));};var ui={};ui.isIE=function(){if(typeof ui.isIE.detected!='undefined')
return ui.isIE.detected;ui.isIE.detected=navigator.appName=='Microsoft Internet Explorer';return ui.isIE.detected;};ui.ahref=function(loc,target,html){if(target=='_blank')
return'<a href="'+loc+'" target="_blank">'+html+'</a>';return'<a href="javascript:void(0)" onclick="window.open(\''+loc+'\', \''+target+'\');">'+html+'</a>';};ui.menu=function(name,href,target){function Menu(n,h,t){this.name=n;this.href=h;this.target=isNil(t)?'_parent':t;this.content=function(){function complete(uri){var q=uri.indexOf('?');if(q!=-1)
return complete(uri.substr(0,q));if(uri.match('.*\.html$'))
return uri;if(uri.match('.*/$'))
return uri+'index.html';return uri+'/index.html';}
if(complete(this.href)!=complete(window.top.location.pathname))
return ui.ahref(this.href,this.target,'<span class="amenu">'+this.name+'</span>');return ui.ahref(this.href,this.target,'<span class="smenu">'+this.name+'</span>');};}
return new Menu(name,href,target);};ui.menubar=function(left,right){var bar='<table cellpadding="0" cellspacing="0" width="100%" class="tbar"><tr>'+
'<td class="dtbar"><table border="0" cellspacing="0" cellpadding="0"><tr>';for(i in left)
bar=bar+'<td class="ltbar">'+left[i].content()+'</td>'
bar=bar+'</tr></table></td>'+
'<td class="dtbar"><table border="0" cellpadding="0" cellspacing="0" align="right"><tr>';for(i in right)
bar=bar+'<td class="'+(i==0?'dtbar':'rtbar')+'">'+right[i].content()+'</td>'
bar=bar+'</tr></table></td></tr></table>';return bar;};ui.selectSuggestion=function(node,value){for(;;){node=node.parentNode;if(node.tagName.toLowerCase()=='div')
break;}
node.selectSuggestion(value);};ui.hilightSuggestion=function(node,over){if(over)
node.className='suggestHilighted';node.className='suggestItem';};ui.suggest=function(input,suggestFunction){input.suggest=suggestFunction;input.selectSuggestion=function(value){this.hideSuggestDiv();this.value=value;}
input.hideSuggestDiv=function(){if(this.suggestDiv!=null){this.suggestDiv.style.visibility='hidden';}}
input.showSuggestDiv=function(){if(this.suggestDiv==null){this.suggestDiv=document.createElement('div');this.suggestDiv.input=this;this.suggestDiv.className='suggest';input.parentNode.insertBefore(this.suggestDiv,input);this.suggestDiv.style.visibility='hidden';this.suggestDiv.style.zIndex='99';this.suggestDiv.selectSuggestion=function(value){this.input.selectSuggestion(value);}}
var values=this.suggest();var items='';for(var i=0;i<values.length;i++){if(values[i].indexOf(this.value)==-1)
continue;if(items.length==0)
items+='<table class="suggestTable">';items+='<tr><td class="suggestItem" '+
'onmouseover="ui.hilightSuggestion(this, true)" onmouseout="ui.hilightSuggestion(this, false)" '+
'onmousedown="ui.selectSuggestion(this, \''+values[i]+'\')">'+values[i]+'</td></tr>';}
if(items.length!=0)
items+='</table>';this.suggestDiv.innerHTML=items;if(items.length!=0){var node=input;var left=0;var top=0;for(;;){left+=node.offsetLeft;top+=node.offsetTop;node=node.offsetParent;if(node.tagName.toLowerCase()=='body')
break;}
this.suggestDiv.style.left=left;this.suggestDiv.style.top=top+input.offsetHeight;this.suggestDiv.style.visibility='visible';}else
this.suggestDiv.style.visibility='hidden';}
input.onkeydown=function(event){this.showSuggestDiv();};input.onkeyup=function(event){this.showSuggestDiv();};input.onmousedown=function(event){this.showSuggestDiv();};input.onblur=function(event){setTimeout(function(){input.hideSuggestDiv();},50);};};ui.content=function(win){if(!isNil(win.document))
return win.document;if(!isNil(win.contentDocument))
return win.contentDocument;return null;};ui.elementByID=function(node,id){for(var i in node.childNodes){var child=node.childNodes[i];if(child.id==id)
return child;var gchild=ui.elementByID(child,id);if(gchild!=null)
return gchild;}
return null;};function $(id){if(id==document){if(!isNil(document.widget))
return document.widget;return document;}
return ui.elementByID($(document),id);};ui.queryParams=function(){var qp=new Array();var qs=window.location.search.substring(1).split('&');for(var i=0;i<qs.length;i++){var e=qs[i].indexOf('=');if(e>0)
qp[qs[i].substring(0,e)]=unescape(qs[i].substring(e+1));}
return qp;}
ui.widgets={};ui.onload={};ui.loadwidget=function(el,doc,cb){var f=el+'Frame';window.ui.widgets[f]=el;window.ui.onload[f]=cb;var div=document.createElement('div');div.id=f+'Div';div.innerHTML='<iframe id="'+f+'" class="widgetframe" scrolling="no" frameborder="0" src="'+doc+'" onload="window.ui.onload[this.id]()"></iframe>';document.body.appendChild(div);return f;};ui.showbody=function(){document.body.style.visibility='visible';};ui.reload=function(){window.open(window.location,'_self');return true;};ui.installwidget=function(){if(isNil(window.parent)||isNil(window.parent.ui)||isNil(window.parent.ui.widgets))
return true;var pdoc=ui.content(window.parent);for(w in window.parent.ui.widgets){var ww=ui.elementByID(pdoc,w).contentWindow;if(ww==window){document.widget=ui.elementByID(pdoc,window.parent.ui.widgets[w]);document.widget.innerHTML=document.body.innerHTML;return true;}}
return true;};ui.loadiframe=function(el,doc){var f=el+'Frame';$(el).innerHTML='<iframe id="'+f+'" class="loadedframe" scrolling="no" frameborder="0" src="'+doc+'"></iframe>';return f;};ui.numpos=function(p){if(p=='')
return 0;return Number(p.substr(0,p.length-2));};ui.pixpos=function(p){return p+'px';};ui.datatable=function(l){function indent(i){if(i==0)
return'';return'&nbsp;&nbsp;'+indent(i-1);}
function rows(l,i){if(isNil(l))
return'';var e=car(l);if(!isList(e))
return rows(expandElementValues("'value",l),i);if(isList(car(e)))
return rows(expandElementValues("'value",l),i);if(elementHasValue(e)){var v=elementValue(e);if(!isList(v)){return'<tr><td class="datatdl">'+indent(i)+elementName(e).slice(1)+'</td>'+
'<td class="datatdr tdw">'+(v!=null?v:'')+'</td></tr>'+
rows(cdr(l),i);}
return rows(expandElementValues(elementName(e),v),i)+rows(cdr(l),i);}
return'<tr><td class="datatdl">'+indent(i)+elementName(e).slice(1)+'</td>'+
'<td class="datatdr tdw">'+'</td></tr>'+
rows(elementChildren(e),i+1)+
rows(cdr(l),i);}
return'<table class="datatable '+(window.name=='dataFrame'?' databg':'')+'" style="width: 100%;">'+rows(l,0)+'</table>';}
ui.datalist=function(l){function rows(l,i){if(isNil(l))
return'';var e=car(l);if(!isList(e))
return rows(expandElementValues("'value",l),i);if(isList(car(e)))
return rows(expandElementValues("'value",l),i);if(elementHasValue(e)){var v=elementValue(e);if(!isList(v)){return'<tr><td class="datatd tdw">'+(v!=null?v:'')+'</td></tr>'+
rows(cdr(l),i);}
return rows(expandElementValues(elementName(e),v),i)+rows(cdr(l),i);}
return rows(elementChildren(e),i+1)+rows(cdr(l),i);}
return'<table class="datatable '+(window.name=='dataFrame'?' databg':'')+'" style="width: 100%;">'+rows(l,0)+'</table>';}
var JSONClient={};JSONClient.escapeJSONChar=function(c){if(c=="\""||c=="\\")return"\\"+c;if(c=="\b")return"\\b";if(c=="\f")return"\\f";if(c=="\n")return"\\n";if(c=="\r")return"\\r";if(c=="\t")return"\\t";var hex=c.charCodeAt(0).toString(16);if(hex.length==1)return"\\u000"+hex;if(hex.length==2)return"\\u00"+hex;if(hex.length==3)return"\\u0"+hex;return"\\u"+hex;};JSONClient.escapeJSONString=function(s){var parts=s.split("");for(var i=0;i<parts.length;i++){var c=parts[i];if(c=='"'||c=='\\'||c.charCodeAt(0)<32||c.charCodeAt(0)>=128)
parts[i]=JSONClient.escapeJSONChar(parts[i]);}
return"\""+parts.join("")+"\"";};JSONClient.toJSON=function(o){if(o==null)
return"null";if(o.constructor==String)
return JSONClient.escapeJSONString(o);if(o.constructor==Number)
return o.toString();if(o.constructor==Boolean)
return o.toString();if(o.constructor==Date)
return'{javaClass: "java.util.Date", time: '+o.valueOf()+'}';if(o.constructor==Array){var v=[];for(var i=0;i<o.length;i++)
v.push(JSONClient.toJSON(o[i]));return"["+v.join(", ")+"]";}
var v=[];for(attr in o){if(o[attr]==null)
v.push("\""+attr+"\": null");else if(typeof o[attr]=="function")
;else
v.push(JSONClient.escapeJSONString(attr)+": "+JSONClient.toJSON(o[attr]));}
return"{"+v.join(", ")+"}";};function HTTPBindingClient(name,uri){this.name=name;this.uri=uri;this.apply=this.createApplyMethod();}
HTTPBindingClient.jsonrpcID=1;HTTPBindingClient.prototype.createApplyMethod=function(){var fn=function(){var methodName=arguments[0];var args=[];for(var i=1;i<arguments.length;i++)
args.push(arguments[i]);var cb=null;if(typeof args[args.length-1]=="function")
cb=args.pop();var req=HTTPBindingClient.makeJSONRequest(methodName,args,cb);return fn.client.jsonApply(req);};fn.client=this;return fn;};HTTPBindingClient.makeJSONRequest=function(methodName,args,cb){var req={};req.id=HTTPBindingClient.jsonrpcID++;if(cb)
req.cb=cb;var obj={};obj.id=req.id;obj.method=methodName;obj.params=args;req.data=JSONClient.toJSON(obj);return req;};HTTPBindingClient.jsonResult=function(http){function httpCharset(http){try{var contentType=http.getResponseHeader("Content-type");var parts=contentType.split(/\s*;\s*/);for(var i=0;i<parts.length;i++){if(parts[i].substring(0,8)=="charset=")
return parts[i].substring(8,parts[i].length);}}catch(e){}
return"UTF-8";}
if(!HTTPBindingClient.charset)
HTTPBindingClient.charset=httpCharset(http);var obj;eval("obj = "+http.responseText);if(obj.error)
throw new HTTPBindingClient.Exception(obj.error.code,obj.error.msg);var res=obj.result;return res;};HTTPBindingClient.prototype.jsonApply=function(req){var http=HTTPBindingClient.getHTTPRequest();var hascb=req.cb?true:false;http.open("POST",this.uri,hascb);http.setRequestHeader("Content-type","application/json-rpc");if(hascb){http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200){var res=null;try{res=HTTPBindingClient.jsonResult(http);}catch(e){req.cb(null,e);}
req.cb(res);}else
req.cb(null,HTTPBindingClient.Exception(http.status,http.statusText));}};http.send(req.data);return req.id;}
http.send(req.data);if(http.status==200)
return HTTPBindingClient.jsonResult(http);throw new HTTPBindingClient.Exception(http.status,http.statusText);};HTTPBindingClient.prototype.get=function(id,cb){var http=HTTPBindingClient.getHTTPRequest();var hascb=cb?true:false;http.open("GET",this.uri+'/'+id,hascb);if(hascb){http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200)
cb(http.responseText);else
cb(null,new HTTPBindingClient.Exception(http.status,http.statusText));}};http.send(null);return true;}
http.send(null);if(http.status==200)
return http.responseText;throw new HTTPBindingClient.Exception(http.status,http.statusText);};HTTPBindingClient.prototype.post=function(entry,cb){var http=HTTPBindingClient.getHTTPRequest();var hascb=cb?true:false;http.open("POST",this.uri,hascb);http.setRequestHeader("Content-Type","application/atom+xml");if(hascb){http.onreadystatechange=function(){if(http.readyState==4){if(http.status==201)
cb(http.responseText);else
cb(null,new HTTPBindingClient.Exception(http.status,http.statusText));}};http.send(entry);return true;}
http.send(entry);if(http.status==201)
return http.responseText;throw new HTTPBindingClient.Exception(http.status,http.statusText);};HTTPBindingClient.prototype.put=function(id,entry,cb){var http=HTTPBindingClient.getHTTPRequest();var hascb=cb?true:false;http.open("PUT",this.uri+'/'+id,hascb);http.setRequestHeader("Content-Type","application/atom+xml");if(hascb){http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200)
cb();else
cb(new HTTPBindingClient.Exception(http.status,http.statusText));}};http.send(entry);return true;}
http.send(entry);if(http.status==200)
return true;throw new HTTPBindingClient.Exception(http.status,http.statusText);};HTTPBindingClient.prototype.del=function(id,cb){var http=HTTPBindingClient.getHTTPRequest();var hascb=cb?true:false;http.open("DELETE",this.uri+'/'+id,hascb);if(cb){http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200)
cb();else
cb(new HTTPBindingClient.Exception(http.status,http.statusText));}};http.send(null);return true;}
http.send(null);if(http.status==200)
return true;throw new HTTPBindingClient.Exception(http.status,http.statusText);};HTTPBindingClient.Exception=function(code,message){this.name="HTTPBindingClientException";this.code=code;this.message=message;};HTTPBindingClient.Exception.prototype=new Error();HTTPBindingClient.Exception.prototype.toString=function(){return this.name+": "+this.message;};HTTPBindingClient.msxmlNames=["MSXML2.XMLHTTP.5.0","MSXML2.XMLHTTP.4.0","MSXML2.XMLHTTP.3.0","MSXML2.XMLHTTP","Microsoft.XMLHTTP"];HTTPBindingClient.getHTTPRequest=function(){if(HTTPBindingClient.httpFactory)
return HTTPBindingClient.httpFactory();try{HTTPBindingClient.httpFactory=function(){return new XMLHttpRequest();};return HTTPBindingClient.httpFactory();}catch(e){}
for(var i=0;i<HTTPBindingClient.msxmlNames.length;i++){try{HTTPBindingClient.httpFactory=function(){return new ActiveXObject(HTTPBindingClient.msxmlNames[i]);};return HTTPBindingClient.httpFactory();}catch(e){}}
HTTPBindingClient.httpFactory=null;throw new HTTPBindingClient.Exception(0,"Can't create XMLHttpRequest object");};var sca={};sca.httpclient=function(name,uri){return new HTTPBindingClient(name,uri);};sca.component=function(name){return new HTTPBindingClient(name,'/components/'+name);};sca.reference=function(comp,rname){return new HTTPBindingClient(comp.name+'/'+rname,"/references/"+comp.name+"/"+rname);};sca.defun=function(ref){function defapply(name){return function(){var args=new Array();args[0]=name;for(i=0,n=arguments.length;i<n;i++)
args[i+1]=arguments[i];return this.apply.apply(this,args);};}
for(f=1;f<arguments.length;f++){var fn=arguments[f];ref[fn]=defapply(fn);}
return ref;};
