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

#include "commonj/sdo/ChangeSummaryBuilder.h"

#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/ChangeSummary.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XMLQName.h"
#include "commonj/sdo/DASProperty.h"
#include "commonj/sdo/Logging.h"

#include <stdio.h>
#include <stdlib.h>
namespace commonj
{
    namespace sdo
    {
        
// ========================================================================
// Constructor/Destructor
// ========================================================================

        ChangeSummaryBuilder::ChangeSummaryBuilder(
            DataFactoryPtr df,
            DataObjectPtr& rootDO)
            
            : dataFactory(df),
            rootDataObject(rootDO)
            
    
        {
            LOGINFO(INFO,"CSBuilder constructor");
            currentState = baseState;
        }
        
        ChangeSummaryBuilder::~ChangeSummaryBuilder()
        {
            LOGINFO(INFO,"CSBuilder destructor");
        }
        
        
// ========================================================================
// Deletion handling
// ========================================================================

        void ChangeSummaryBuilder::populateDeletion(ChangeSummaryImpl* csi,DataObjectPtr dob,
                                            int currentIndex)
        {
            LOGENTRY(INFO,"CSBuilder populateDeletion");
            try {
                
                std::list<deletionAttribute>::iterator  attributesiter;
                std::list<deletionElement>::iterator    elementsiter;

                for (attributesiter=  deletions[currentIndex].attributes.begin();
                     attributesiter!= deletions[currentIndex].attributes.end();
                     ++attributesiter)
                {

                    LOGINFO_1(INFO,"CSBuilder Populate deletion attribute:%s",
                    (const char*)((*attributesiter).name));

                    dob->setCString((const char*)((*attributesiter).name),
                                    (const char*)((*attributesiter).value));
                }
                for (elementsiter=  deletions[currentIndex].elements.begin();
                     elementsiter!= deletions[currentIndex].elements.end();
                     ++elementsiter)
                {

                    LOGINFO_1(INFO,"CSBuilder:Populate deletion element:%s",
                    (const char*)((*elementsiter).name));
                    
                    SDOXMLString value = (*elementsiter).value;
                    SDOXMLString prop = (*elementsiter).name;

                    if ((*elementsiter).isDeletion)
                    {
                        for (int i = 0; i < deletions.size(); i++)
                        {
                            if (value.equals (deletions[i].reference))
                            {
                                if (deletions[i].dob == 0)
                                {
                                    handleDeletion(csi,i,dob,(*elementsiter).value,
                                        (*elementsiter).name);
                                }
                                // dob may have been set by the line above.
                                if (deletions[i].dob != 0)
                                {
                                    DataObjectList& dli = dob->getList((const char*)prop);
                                    dli.insert(0,deletions[i].dob);
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        DataObjectList& dli = dob->getList((const char*)prop);
                        dli.append((const char*)value);
                    }
                }
            }
            catch (SDORuntimeException e)
            {
                LOGSDOEXCEPTION(ERROR,"Error populating a deletion record",e);
            }
            LOGEXIT(INFO,"CSBuilder populateDeletion");
            return;
        }


// ========================================================================
// Deletion handling
// ========================================================================

        void ChangeSummaryBuilder::handleDeletion(
                              ChangeSummaryImpl* csi, 
                              int currentIndex, 
                              DataObjectPtr cont,
                              SDOXMLString path,
                              SDOXMLString prop)
        {
            LOGENTRY(INFO,"CSBuilder handleDeletion(2)");

            if (deletions[currentIndex].completedprocessing)
            {
                LOGEXIT(INFO,"CSBuilder handleDeletion(2) - Exit1");
                return;
            }

            const Property& p = cont->getProperty((const char*)prop);
            DataObjectPtr dob = dataFactory->create(
                                p.getType());
            populateDeletion(csi,dob,currentIndex);
            deletions[currentIndex].dob = dob;
            deletions[currentIndex].completedprocessing=true;
            csi->appendToDeletions(p,dob,cont,path);

            LOGEXIT(INFO,"CSBuilder handleDeletion(2) - Exit2");

            return;
        }

// ========================================================================
// Deletion handling
// ========================================================================

        void ChangeSummaryBuilder::handleDeletion(
                              ChangeSummaryImpl* csi, 
                              int currentIndex, 
                              SDOXMLString path)
        {

            // find the original container - and see if it
            // still exists.
            // supposing the container doesnt exist, we need
            // to process the deletion of that one first, or
            // locate it in the already deleted elements.

            LOGENTRY(INFO,"CSBuilder:handleDeletion");

            if (deletions[currentIndex].completedprocessing)
            {
                LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit1");
                return;
            }

            int index = path.lastIndexOf('/');
            if (index < 0)
            {
                LOGERROR(WARNING,"CSBuilder logs deletion of the root object - not possible");
                LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit2");
                return;
            }
            
            SDOXMLString prop = 
                    path.substring(index+1);
            SDOXMLString contpath =
                    path.substring(0,index);

            index = prop.lastIndexOf('.');
            if (index >= 0) prop = prop.substring(0,index);

            index = prop.lastIndexOf('[');
            if (index >= 0) prop = prop.substring(0,index);

            DataObjectPtr cont;

            if (contpath.isNull())
            {
                cont = rootDataObject;

                LOGINFO_1(INFO,"Handling deletion from root of %s",(const char*)prop);
            }
            else 
            {
                try {
                    cont = rootDataObject->getDataObject((const char*)contpath);
                    LOGINFO_2(INFO,"Handling deletion from %s of %s",
                        (const char*)contpath,(const char*)prop);
                }
                catch (SDORuntimeException e)
                {
                    cont  = 0;
                    LOGINFO(INFO,"Container was a deleted object");
                }
            }

            if (cont != 0)
            {
                handleDeletion(csi,currentIndex,cont,path,prop);
                LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit3");
                return;
            }
            else
            {
                // this data object was also deleted
                cont = csi->matchDeletedObject(contpath);
                if (cont != 0)
                {
                    // we found a previously deleted object for 
                    // the container.
                    handleDeletion(csi,currentIndex,cont,path,prop);
                    LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit4");
                    return;
                }

            }

            // it could be a deletion which has not been processed yet
            for (int i = 0; i < deletions.size(); i++)
            {
                if (contpath.equals (deletions[i].reference))
                {
                    // matching deletion found
                    if (i == currentIndex)
                    {
                        LOGERROR(WARNING,"Loop detected in handling deletes");
                        LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit5");
                        return;
                    }
                    // Handle the deletion first
                    handleDeletion(csi,i,contpath);
                    LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit6");
                    return;
                }
            }
            LOGERROR(WARNING,"Failed to find a matching delete ");
            LOGEXIT(INFO,"CSBuilder:handleDeletion - Exit7");
            return;
        }

// ========================================================================
// Deletion handling
// ========================================================================

        SDOXMLString ChangeSummaryBuilder::shiftedIndex(int index)
        {
            int pos = createDeletes[index].value.lastIndexOf('.');
            if (pos < 0) return createDeletes[index].value;

            int val = atoi((const char*)(createDeletes[index].
                value.substring(pos+1)));

            char buf[10];

            sprintf(buf, ".%d", val + createDeletes[index].indexshift);

            return createDeletes[index].value.substring(0,pos) + buf;
        }

// ========================================================================
// Deletion handling
// ========================================================================

        void ChangeSummaryBuilder::shiftIndices(int index, int delta)
        {
            int pos = createDeletes[index].value.lastIndexOf('.');
            if (pos < 0) return;

            SDOXMLString refstring = createDeletes[index].value.substring(0,pos);
            pos = atoi((const char*)(createDeletes[index].value.substring(pos+1)));
                                     
            for (int k=index+1;k<createDeletes.size();k++)
            {

                if (createDeletes[k].type.equals("create")) continue;

                int pos2 = createDeletes[k].value.lastIndexOf('.');
                if (pos2 < 0) continue;

                SDOXMLString refto = createDeletes[k].value.substring(0,pos2);
                pos2 = atoi((const char*)(createDeletes[k].value.substring(pos2+1)));

                if (refstring.equals(refto) && pos2 >= pos)
                {
                    createDeletes[k].indexshift += delta;
                }
            }
        }

// ========================================================================
// Rebuilding of a change summary from the lists
// ========================================================================

        void ChangeSummaryBuilder::buildChangeSummary(
             DataObjectPtr changeSummaryDO)
        {
            LOGENTRY(INFO,"CSBuilder:buildChangeSummary");

            int i;

            try {
                if (changeSummaryDO == 0)
                {
                    // there is no summary data object to rebuild
                    LOGEXIT(INFO,"CSBuilder:buildChangeSummary Exit1");
                    return;
                }
                ChangeSummary* cs = changeSummaryDO->getChangeSummary();
                if (cs == 0)
                {
                    // there is no change summary - this shouldnt happen
                    LOGEXIT(INFO,"CSBuilder:buildChangeSummary Exit2");
                    return;
                }

                ChangeSummaryImpl* csi = (ChangeSummaryImpl*)cs;

                // add the creations and deletions in order

                for (i = 0; i < createDeletes.size(); i++)
                {

                    LOGINFO_1(INFO,"CSBuilder:create/delete:%s",
                        (const char*)(createDeletes[i].value));

                    SDOXMLString thispath;

                    if (createDeletes[i].indexshift != 0)
                    {
                         thispath = shiftedIndex(i);
                    }
                    else
                    {
                        thispath = createDeletes[i].value;
                    }

                    if (!createDeletes[i].type.equals("delete"))
                    {
                        DataObjectPtr dob;
                        if (thispath.isNull())
                        {
                            dob = rootDataObject;
                        }
                        else
                        {
                            dob = rootDataObject->getDataObject((const char*)thispath);
                        }
                        if (dob != 0) {
                            
                            LOGINFO(INFO,"CSBuilder:add create to change summary");

                            csi->appendToCreations(dob->getContainmentProperty(),
                                            dob, dob->getContainer());
                            shiftIndices(i,-1);
                        }
                        else 
                        {
                            LOGERROR_1(WARNING,"Failed to find object at %s",
                                (const char*)createDeletes[i].value);
                        }
                    }
                    else 
                    {
                        // its a deletion - match it.
                        for (int j = 0; j < deletions.size(); j++)
                        {
                            if (deletions[j].completedprocessing)continue;

                            if (thispath.equals (deletions[j].reference))
                            {
                                // matching deletion found
                                LOGINFO_1(INFO,"CSBuilder:matched delete:%s",
                                (const char*)deletions[j].reference);
                                
                                handleDeletion(csi, j,createDeletes[i].value);
                                // update the path on any higher items in the current 
                                // many-valued property.
                                shiftIndices(i,1);

                                break;
                            }
                        }
                    }
                }
        
                // add the changes

                std::list<changeAttribute>::iterator a;
                std::list<changeElement>::iterator e;

                for (i = 0; i< changes.size(); i++)
                {
                    DataObjectPtr dob;
                    if (changes[i].reference.isNull())
                    {
                        dob = rootDataObject;
                    }
                    else 
                    {
                        dob = rootDataObject->getDataObject((const char*)changes[i].reference);
                    }
                    if (dob != 0)
                    {
                        LOGINFO_1(INFO,"Located a change at %s",
                             (const char*)changes[i].reference);

                        // Need to get the changelogitem, and append settings to
                        // it.Append attributes

                        for (a=changes[i].attributes.begin();
                             a != changes[i].attributes.end();++a)
                        {
                            const Property& p = dob->getProperty((const char*)((*a).name));
                            if (p.getType().isDataType()) 
                            {
                                csi->appendToChanges(
                                    p,dob,(*a).value,0);
                            }
                            else
                            {
                                for (int j = 0; j < deletions.size(); j++)
                                {
                                    if ((*a).value.equals (deletions[j].reference))
                                    {
                                        csi->appendToChanges(
                                                p,dob,deletions[j].dob,0);
                                        break;
                                    }
                                }
                            }
                        }
                        for (e=changes[i].elements.begin();
                             e != changes[i].elements.end();++e)
                        {
                            const Property& p = dob->getProperty
                                               ((const char*)((*e).name));
                            if ((*e).isDeletion)
                            {

                                LOGINFO_1(INFO,"CSBuilder Change was a deletion of:%s",
                                    (const char*)((*e).path));

                                for (int j = 0; j < deletions.size(); j++)
                                {
                                    if ((*e).path.equals (deletions[j].reference))
                                    {
                                        // matching deletion found
                                        if (deletions[j].dob != 0)
                                        {
                                            csi->appendToChanges(
                                                p,dob,deletions[j].dob,(*e).index);
                                        }
                                        break;
                                    }
                                }
                            }
                            else if ((*e).isReference)
                            {
                                LOGINFO_2(INFO,"CSBuilder:Change was a reference from:%s to %s" ,
                                    (const char*)((*e).path),
                                    (const char*)((*e).value));

                                DataObjectPtr pdob = rootDataObject->getDataObject((const char*)(*e).value);
                                if (pdob != 0)
                                {
                                    csi->appendToChanges(p,dob,pdob,(*e).index);
                                }
                            }
                            else
                            {
                                LOGINFO_1(INFO,"CSBuilder:Change was a many valued item:%s" ,
                                (const char*)((*e).name));

                                csi->appendToChanges(p,dob,(*e).value,(*e).index);
                            }

                        }
                    }
                    else
                    {
                        LOGERROR_1(WARNING,"CSBuilder:Failed to find object at %s",
                            (const char*)(changes[i].reference));
                    }
                }
            }
            catch (SDORuntimeException e)
            {
                LOGSDOEXCEPTION(ERROR,"CSBuilder:Error rebuilding change summary",e);
            }
            LOGEXIT(INFO,"CSBuilder:buildChangeSummary Exit3");
        }
        
// ========================================================================
// Parsing code
// ========================================================================

        void ChangeSummaryBuilder::processStart(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            char cbuf[32];
            int i;

            LOGENTRY(INFO,"CSBuilder:processStart");

            if (currentState == baseState)
            {
                if (localname.equalsIgnoreCase("create") ||
                    localname.equalsIgnoreCase("delete"))
                {
                    currentState = dealingWithCreateDelete;
                    // will be expecting a chars message to give the 
                    // contents
            
                    createDeletes.insert(
                        createDeletes.end(),createDelete(localname));

                    LOGINFO_1(INFO,"CSBuilder: Processing a %s",(const char*)localname);
                    LOGEXIT(INFO,"CSBuilder:processStart Exit1");
                    return;
                }
                else
                {
                    // we are starting a new change record here
                    currentState = dealingWithChange;
                    
                    // we expect the entry to have an sdo:ref, and possibly
                    // some attributes for the changed single-values
                    SDOXMLString ref = attributes.getValue("ref");

                    if (ref.isNull())
                    {
                        LOGERROR(INFO,"CSBuilder:Problem with change reference");
                        ref = SDOXMLString("Unknown");
                    }

                    currentChange = change(localname, ref);

                    LOGINFO_2(INFO,"CSBuilder: Processing a change:%s:%s",
                        (const char*)localname,
                        (const char*)ref);
                
                    currentLocation.clear();

                    if (ref.equals("#/")) 
                    {
                        ref = SDOXMLString("#");
                    }

                    currentLocation.push_back(ref);
                    
                    // clear out the change state - if there was any.

                    changeIndex = 0;
                    previousChange = SDOXMLString("");

                    // now insert any remaining attributes of this change
                    
                    for (i=0; i < attributes.size(); i++)
                    {
                        // push all the attributes into the change record.
                        SDOXMLString name = attributes[i].getName();
                        if (!name.equalsIgnoreCase("ref"))
                        {
                            SDOXMLString value = attributes[i].getValue();
                            currentChange.addAttribute(
                                changeAttribute(name,value));
                        }
                    }
                }
                LOGEXIT(INFO,"CSBuilder:processStart Exit2");
                return;
            }

            // We are not in the base state - we could be in a change or a deletion
            // We are now dropping into an element, which could be either a many-valued
            // primitive type, or a placeholder in list of deletions, or a deletion
            // record.

            if (currentState == dealingWithChange)
            {

                if (!previousChange.equals(localname))
                {
                    changeIndex = 0;
                    previousChange = localname;
                }
                else
                {
                    changeIndex++;
                }


                sprintf(cbuf,"%d",changeIndex);
                currentLocation.push_back(localname + "." + cbuf);

                // build the current location...
                SDOXMLString curr_loc = "";

                if (currentLocation.size() > 0)
                {
                    for (int pi = 0; pi < currentLocation.size()-1; pi++)
                    {
                        curr_loc  = curr_loc + currentLocation[pi];
                        curr_loc = curr_loc + "/";
                    }
                    curr_loc = curr_loc + currentLocation[currentLocation.size()-1];
                }

                LOGINFO_1(INFO,"CSBuilder: Dropping into an element of a change:%s", 
                        (const char*)localname);
                LOGINFO_1(INFO,"CSBuilder:currentLocation:%s",
                        (const char*)curr_loc);

                // First case - its a placeholder for an element in a list
                // all we have to do is keep track of the list index.

                if (attributes.size() == 1)
                {
                    SDOXMLString name = attributes[0].getName();
                    if (name.equalsIgnoreCase("ref"))
                    {
                        currentState = dealingWithChangeElement;
                        currentChange.addElement(
                                    changeElement(localname, curr_loc,
                                    attributes[0].getValue(), true, false));

                        LOGEXIT(INFO,"CSBuilder:processStart Exit3");
                        return; 
                    }
                }

                if (attributes.size() == 0)
                {
                    // It could be a many-valued primitive, or 
                    // a deletion with no attributes
                    // can we match the current path to a known deletion?

                    bool isDelete = false;

                    for (int i = 0; i < createDeletes.size(); i++)
                    {
                        if (createDeletes[i].type.equals("delete"))
                        {
                            if (curr_loc.equals(createDeletes[i].value))
                            {
                                LOGINFO(INFO,"CSBuilder: An existing delete was found");

                                isDelete = true;
                                break; // out of the for loop
                            }
                        }
                    }
                    if (!isDelete) 
                    {
                        LOGINFO(INFO,"CSBuilder: Change with no atts and not deletion- an element");
                        
                        // and will get picked up by the 'characters' method.
                        currentState = dealingWithChangeElement;
                        currentLocalName = SDOXMLString(localname);

                        LOGEXIT(INFO,"CSBuilder:processStart Exit4");
                        return;
                    }
                }

                currentChange.addElement(changeElement(localname, curr_loc, false, true));
                currentDeletion = deletion(localname, curr_loc);
                
                LOGINFO_2(INFO,"CSBuilder: Found a deletion:%s:%s",
                    (const char*)localname,
                    (const char*)curr_loc);
                
                    
                // clear out the change state - if there was any.
                deletionLevel = 0;
                deletionIndex = 0;
                previousDeletion = SDOXMLString("");

                currentState = dealingWithDeletion;

                // Now add the attributes if there are any.

                for (int i=0; i < attributes.size(); i++)
                {
                    // push all the attributes into the deletion record.
                    SDOXMLString name = attributes[i].getName();
                    if (!name.equalsIgnoreCase("ref"))
                    {
                        SDOXMLString value = attributes[i].getValue();
                        currentDeletion.addAttribute(
                            deletionAttribute(name,value));
                    }
                }

                LOGEXIT(INFO,"CSBuilder:processStart Exit5");
                return;
            }
            if (currentState == dealingWithDeletion)
            {
                // we are already in a deletion, and entering an element 
                if (!previousDeletion.equals(localname))
                {
                    deletionIndex = 0;
                    previousDeletion = localname;
                }
                else
                {
                    deletionIndex++;
                }

                sprintf(cbuf,"%d",deletionIndex);
                currentLocation.push_back(localname + "." + cbuf);

                 // First case - its a placeholder for an element in a list
                // all we have to do is keep track of the list index.

                if (attributes.size() == 1)
                {
                    SDOXMLString name = attributes[0].getName();
                    if (name.equalsIgnoreCase("ref"))
                    {
                        LOGINFO(INFO,"CSBuilder: The element is a reference to an existing entry in a list");
                        
                        currentState = dealingWithDeletionElement;
                        
                        LOGEXIT(INFO,"CSBuilder:processStart Exit6");
                        return; 
                    }
                }

                SDOXMLString curr_loc = "";
                if (currentLocation.size() > 0)
                {
                    for (int pi = 0; pi < currentLocation.size()-1; pi++)
                    {
                        curr_loc  = curr_loc + currentLocation[pi];
                        curr_loc = curr_loc + "/";
                    }
                    curr_loc = curr_loc + currentLocation[currentLocation.size()-1];
                }

                if (attributes.size() == 0)
                {
                    // It could be a many-valued primitive, or 
                    // a deletion with no attributes
                    // can we match the current path to a known deletion?
                    bool isDelete = false;

                    for (int i = 0; i < createDeletes.size(); i++)
                    {
                        if (createDeletes[i].type.equals("delete"))
                        {
                            if (curr_loc.equals(createDeletes[i].value))
                            {
                                LOGINFO(INFO,"CSBuilder: An existing delete was found - its a delete");

                                isDelete = true;
                                break; // out of the for loop
                            }
                        }
                    }
                    if (!isDelete) 
                    {
                        LOGINFO(INFO,"CSBuilder: Found a change with no atts, and no deletion matches - its an element");
                        
                        // and will get picked up by the 'characters' method.
                        currentState = dealingWithDeletionElement;
                        
                        LOGEXIT(INFO,"CSBuilder:processStart Exit7");
                        return;
                    }
                }


                // We are starting a new deletion record

                LOGINFO_2(INFO,"CSBuilder: Found a deletion within a deletion:%s:%s",
                    (const char*)localname, (const char*)curr_loc);
                    
                deletionList.push_back(deletionListElement(currentDeletion,deletionIndex,
                previousDeletion));
                                
                currentDeletion = deletion(localname, curr_loc);

                // we are in a nested delete - so record the level such that we can pop state
                // from the deletionList on coming back out of this.

                deletionLevel++;
                //deletionIndex = 0;
                previousDeletion = SDOXMLString("");

                currentState = dealingWithDeletion;

                // Now add the attributes if there are any.

                for (int i=0; i < attributes.size(); i++)
                {
                    // push all the attributes into the deletion record.
                    SDOXMLString name = attributes[i].getName();
                    if (!name.equalsIgnoreCase("ref"))
                    {
                        SDOXMLString value = attributes[i].getValue();
                        currentDeletion.addAttribute(
                            deletionAttribute(name,value));
                    }
                }

                LOGEXIT(INFO,"CSBuilder:processStart Exit8");
                return;

            }
        }
        
// ========================================================================
// Parser ends an element
// ========================================================================
        
        void ChangeSummaryBuilder::processEnd(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI)
        {

            LOGENTRY(INFO,"CSBuilder:processEnd");

            if (currentState == dealingWithCreateDelete)
            {
                // its already been completed by the characters message
                currentState = baseState;
                LOGEXIT(INFO,"CSBuilder:processEnd Exit1");
                return;
            }
            else if (currentState == dealingWithChangeElement)
            {
                // its already been completed by the characters message
                currentState = dealingWithChange;
                currentLocation.pop_back();
                LOGEXIT(INFO,"CSBuilder:processEnd Exit2");
                return;
            }
            else if (currentState == dealingWithDeletionElement)
            {
                // its already been completed by the characters message
                currentState = dealingWithDeletion;
                currentLocation.pop_back();
                LOGEXIT(INFO,"CSBuilder:processEnd Exit3");
                return;
            }
            else if (currentState == dealingWithChange)
            {
                // we should have finished, there are no nested changes
                currentState = baseState;
                changes.insert(changes.end(),currentChange);
                currentLocation.clear();
                LOGEXIT(INFO,"CSBuilder:processEnd Exit4");
                return;
            }
            else if (currentState == dealingWithDeletion)
            {
                currentLocation.pop_back();
                deletions.insert(deletions.end(),currentDeletion);
                if (deletionLevel > 0)
                {
                    deletionLevel--;
                    if (deletionList.size() > 0)
                    {
                        int index = deletionIndex;
                        SDOXMLString path = currentDeletion.reference;
                        SDOXMLString name = currentDeletion.name;

                        deletionListElement dl = deletionList[deletionList.size()-1]; 
                        currentDeletion = dl.del;
                        deletionIndex = dl.index;
                        previousDeletion = dl.previous;
                        deletionList.pop_back();
                        // and insert the inside deletion in the outside one.
                        currentDeletion.insertElement(deletionElement(
                            name, path,index));
                    }

                    LOGEXIT(INFO,"CSBuilder:processEnd Exit5");
                    return;
                }
                else
                {
                    currentState = dealingWithChange;

                    LOGEXIT(INFO,"CSBuilder:processEnd Exit6");
                    return;
                }
            }
        }

// ========================================================================
// Parse characters
// ========================================================================
        
        
        void ChangeSummaryBuilder::processChars(
            const SDOXMLString& chars)
        {
            LOGENTRY(INFO,"CSBuilder:processChars");

            if (currentState == dealingWithCreateDelete)
            {
                // this is text within a create/delete indicating the 
                // value of the item deleted/created.
                if (createDeletes.size() > 0) 
                {
                    createDeletes[createDeletes.size()-1].value = chars;
                    LOGINFO_1(INFO,"CSBuilder:characters create/delete:%s",
                        (const char*)chars);
                }
                LOGEXIT(INFO,"CSBuilder:processChars Exit1");
                return;
            }
            else if (currentState == dealingWithChangeElement)
            {
                changeElement ce = changeElement(currentLocalName, previousChange, false, false);
                ce.value = chars;
                ce.index = changeIndex;
                currentChange.addElement(ce);

                LOGINFO_1(INFO,"CSBuilder: Characters in a change element:%s",
                    (const char *)chars); 
                
                LOGEXIT(INFO,"CSBuilder:processChars Exit2");
                return;
            }
            else if (currentState == dealingWithDeletionElement)
            {
                deletionElement ce = deletionElement(previousDeletion);
                ce.value = chars;
                ce.index = deletionIndex;
                currentDeletion.addElement(ce);
                
                LOGINFO_1(INFO,"CSBuilder: Characters in a deletion element:%s",
                    (const char *)chars); 

                LOGEXIT(INFO,"CSBuilder:processChars Exit3");
                return;
            }

            LOGINFO_1(INFO,"CSBuilder: Characters in nothing!!:%s",
                (const char *)chars); 
            LOGEXIT(INFO,"CSBuilder:processChars Exit4");
        }

    
// ========================================================================
// Classes holding temporary parsed information
// ========================================================================
        

        /////////////////////////////////////////////////
        // An element in the list of creates/deletes
        /////////////////////////////////////////////////

        createDelete::createDelete()
        {
        }

        createDelete::createDelete(SDOXMLString intype) :
        type(intype)
        {
            indexshift = 0;
        }

        createDelete::~createDelete()
        {
        }

        /////////////////////////////////////////////////
        // An element in the list of changes
        /////////////////////////////////////////////////

        changeAttribute::changeAttribute()
        {
        }

        changeAttribute::changeAttribute(SDOXMLString inname,
            SDOXMLString invalue): name(inname), value(invalue)
        {
        }

        changeAttribute::~changeAttribute()
        {
        }

        changeElement::changeElement()
        {
        }

        changeElement::changeElement(SDOXMLString inname, 
            SDOXMLString inpath, bool isRef, bool isDel)
            : name(inname) , path(inpath)
        {
            isReference=isRef;
            isDeletion= isDel;
        }

        changeElement::changeElement(SDOXMLString inname, 
            SDOXMLString inpath,
            SDOXMLString invalue, bool isRef, bool isDel)
            : name(inname), value(invalue), path(inpath)
        {
            isReference=isRef;
            isDeletion= isDel;
        }

        changeElement::~changeElement()
        {
        }

        change::change()
        {
        }

        change::change(SDOXMLString inname, SDOXMLString ref): 
                name(inname), reference(ref)
        {
        }

        void change::addAttribute(changeAttribute ca)
        {
            attributes.insert(attributes.end(),ca);
        }

        void change::addElement(changeElement ce)
        {
            elements.insert(elements.end(),ce);
        }

        /////////////////////////////////////////////////
        // An element in the list of deletions
        /////////////////////////////////////////////////

        deletionAttribute::deletionAttribute()
        {
        }

        deletionAttribute::deletionAttribute(SDOXMLString inname,
            SDOXMLString invalue): name(inname), value(invalue)
        {
        }

        deletionAttribute::~deletionAttribute()
        {
        }

        deletionElement::deletionElement()
        {
        }

        deletionElement::deletionElement(SDOXMLString inname)
            : name(inname)
        {
            isDeletion = false;
        }

        deletionElement::deletionElement(SDOXMLString inname,
            SDOXMLString inpath, int inindex)
            : name(inname), value(inpath), index(inindex)
        {
            isDeletion = true;
        }

        deletionElement::~deletionElement()
        {
        }

        deletion::deletion()
        {
        }

        deletion::deletion(SDOXMLString inname, SDOXMLString ref): 
                name(inname), reference(ref)
        {
            completedprocessing=false;
        }

        void deletion::addAttribute(deletionAttribute ca)
        {
            attributes.insert(attributes.end(),ca);
        }

        void deletion::addElement(deletionElement ce)
        {
            elements.insert(elements.end(),ce);
        }

        void deletion::insertElement(deletionElement ce)
        {
            elements.insert(elements.begin(),ce);
        }

        deletionListElement::deletionListElement()
        {
        }

        deletionListElement::deletionListElement(
            deletion in_del, 
            int in_index, 
            SDOXMLString in_prev):
            del(in_del), index(in_index), previous(in_prev)
        {
        }
        
    } // End - namespace sdo
} // End - namespace commonj

