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

var editWidget = sca.component("EditWidget");
var user= sca.defun(sca.reference(editWidget, "user"), "id");

/**
 * Display the current signed in user.
 */
function userMenu() {
    function UserMenu() {
        this.content = function() {
            var u = user.id()
            return '<span>' + u + '</span>';
        };
    }
    return new UserMenu();
}

/**
 * Display the menu bar.
 */
function displaymenu() {
    var mdiv = $('menu'); 
    var name = ui.queryParams()['app'];

    mdiv.innerHTML = ui.menubar(
        append(mklist(ui.menu('Home', '/'), ui.menu('Store', '/store/')),
                (isNil(name) || name == 'undefined')?
                    mklist() :
                    mklist(
                        ui.menu('Stats', '/stats/?app=' + name),
                        ui.menu('Page', '/page/?app=' + name),
                        ui.menu(isNil(config.compose)? 'Composition' : config.compose, '/graph/?app=' + name))),
        mklist(ui.menu('Account', '/account/'), ui.menu('Sign out', '/logout/')));
}

