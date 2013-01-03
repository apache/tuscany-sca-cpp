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

/* $Rev$ $Date$ */

#ifndef tuscany_webservice_hpp
#define tuscany_webservice_hpp

/**
 * Web service invocation functions using Axis2.
 */
#include "config.hpp"

// Ignore redundant declarations in Axiom headers
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif
#include <axiom.h>
#include <axis2_client.h>
#include <axis2_module.h>
#include <axis2_addr_mod.h>
#include <axis2_conf_ctx.h>
#include <axis2_disp.h>
#include <axis2_http_out_transport_info.h>
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic warning "-Wredundant-decls"
#endif

#include "string.hpp"
#include "sstream.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/xml/xml.hpp"

namespace tuscany {
namespace webservice {

/**
 * Represents an Axis2 runtime context.
 */
class Axis2Context {
public:
    Axis2Context() : env(axutil_env_create_all("axis2.log", AXIS2_LOG_LEVEL_WARNING)), owner(true) {
        debug("webservice::axis2context");
    }

    Axis2Context(const Axis2Context& ax) : env(ax.env), owner(false) {
        debug("webservice::axis2context::copy");
    }

    Axis2Context& operator=(const Axis2Context& ax) = delete;

    Axis2Context(const axutil_env_t* const env) : env(const_cast<axutil_env_t*>(env)), owner(false) {
        debug("webservice::axis2context::env");
    }

    ~Axis2Context() {
        if (!owner || env == NULL)
            return;
        axutil_env_free(env);
    }

private:
    axutil_env_t* const env;
    const bool owner;

    friend const axutil_env_t* const env(const Axis2Context& ax);
};

const axutil_env_t* const env(const Axis2Context& ax) {
    return ax.env;
}

/**
 * Return the latest Axis2 error in an Axis2 context.
 */
const string axis2Error(const Axis2Context& ax) {
    ostringstream os;
    os << env(ax)->error->error_number << " : " << AXIS2_ERROR_GET_MESSAGE(env(ax)->error);
    return str(os);
}

/**
 * Convert a string to an Axiom node.
 */
const failable<axiom_node_t*> stringToAxiomNode(const string& s, const Axis2Context& ax) {
    axiom_node_t* const node = axiom_node_create_from_buffer(env(ax), const_cast<axis2_char_t*>(c_str(s)));
    if (node == NULL)
        return mkfailure<axiom_node_t*>(string("Couldn't convert XML to Axiom node: ") + axis2Error(ax));
    return node;
}

/**
 * Convert a list of values representing XML elements to an Axiom node.
 */
const failable<axiom_node_t*> valuesToAxiomNode(const list<value>& l, const Axis2Context& ax) {
    const failable<list<string> > xml = xml::writeElements(valuesToElements(l), false);
    if (!hasContent(xml))
        return mkfailure<axiom_node_t*>(xml);
    ostringstream os;
    write(content(xml), os);
    return stringToAxiomNode(str(os), ax);
}

/**
 * Convert an axiom node to a string.
 */
const failable<const string> axiomNodeToString(axiom_node_t* const node, const Axis2Context& ax) {
    const char* const c = axiom_node_to_string(node, env(ax));
    if (c == NULL)
        return mkfailure<const string>(string("Couldn't convert Axiom node to XML: ") + axis2Error(ax));
    const string s(c);
    AXIS2_FREE(env(ax)->allocator, const_cast<char*>(c));
    return s;
}

/**
 * Convert an axiom node to a list of values representing XML elements.
 */
const failable<const list<value> > axiomNodeToValues(axiom_node_t* const node, const Axis2Context& ax) {
    const failable<const string> s = axiomNodeToString(node, ax);
    if (!hasContent(s))
        return mkfailure<const list<value> >(s);
    istringstream is(content(s));
    const failable<const list<value> > l = content(xml::readElements(streamList(is)));
    if (!hasContent(l))
        return l;
    return elementsToValues(content(l));
}

/**
 * Evaluate an expression in the form (soap-action-string, document, uri). Send the
 * SOAP action and document to the Web Service at the given URI using Axis2.
 */
const failable<value> evalExpr(const value& expr, const Axis2Context& ax) {
    debug(expr, "webservice::evalExpr::input");

    // Extract func name and single argument
    const value func(car<value>(expr));
    const list<value> param(cadr<value>(expr));
    const value uri(caddr<value>(expr));

    // Create Axis2 client
    axis2_svc_client_t* const client = axis2_svc_client_create(env(ax), getenv("AXIS2C_HOME"));
    if (client == NULL)
        return mkfailure<value>("Couldn't create Axis2 client: " + axis2Error(ax));
    axis2_endpoint_ref_t* const epr = axis2_endpoint_ref_create(env(ax), c_str(uri));
    axis2_options_t* const opt = axis2_options_create(env(ax));
    axis2_options_set_to(opt, env(ax), epr);
    axis2_options_set_action(opt, env(ax), (const axis2_char_t*)c_str(func));
    axis2_svc_client_set_options(client, env(ax), opt);
    axis2_svc_client_engage_module(client, env(ax), AXIS2_MODULE_ADDRESSING);

    // Construct request Axiom node
    const failable<axiom_node_t*> req = valuesToAxiomNode(param, ax);
    if (!hasContent(req))
        return mkfailure<value>(req);

    // Call the Web service
    axiom_node_t* const res = axis2_svc_client_send_receive(client, env(ax), content(req));
    if (res == NULL) {
        axis2_svc_client_free(client, env(ax));
        return mkfailure<value>("Couldn't invoke Axis2 service: " + axis2Error(ax));
    }

    // Parse result Axiom node
    const failable<const list<value> > lval = axiomNodeToValues(res, ax);
    if (!hasContent(lval))
        return mkfailure<value>(lval);
    const value rval = content(lval);
    debug(rval, "webservice::evalExpr::result");

    // Cleanup
    axis2_svc_client_free(client, env(ax));

    return rval;
}

}
}

#endif /* tuscany_webservice_hpp */
