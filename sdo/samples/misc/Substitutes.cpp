/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date: 2005/12/22 16:54:14 $ */

#include "samples.h"
using namespace std;

void Substitutes::sample()
{
    cout << " ********** Substitutes sample *************" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("companysubs.xsd");

        /**
         * The companysubs xsd defines some types including a
         * type which allows substitutions. The PublicationType is
         * the base of BookType and MagazineType, and the substitutions
         * are enabled so that properties can be either.
         */

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& book     = mdg->getType("companyNS","BookType");
        const Type& mag      = mdg->getType("companyNS","MagazineType");
        const Type& pub      = mdg->getType("companyNS","PublicationType");

    
        /**
         * Create some data to work with
         */

        DataObjectPtr comp = mdg->create((Type&)tcomp);
        comp->setCString("name","Puflet Publishing");


        DataObjectPtr book1 = mdg->create(book);
        book1->setCString("author","Mr P B Writer");
        
        /**
         * book has a title property because it inherits from Publication
         */
        book1->setCString("title","Nowhere Man");
        
         
         DataObjectPtr mag1 = mdg->create(mag);
  
        /**
         * Magazine has an eidtor, and a title inherited from publication
         */
        
        mag1->setCString("editor","Mr B Picky");
        // inherited from publication
        mag1->setCString("title","Bionicle Weekly");
    
        
        DataObjectPtr pub1 = mdg->create(pub);
        pub1->setCString("title","Noddy In Toyland");


        /**
         * The property "Publication" is defined as substitutable, so
         * any of the book, magazine  or publication should be 
         * acceptable values. When the type is queried, the type
         * returned should correspond to the current type of the
         * property...
         */

        comp->setDataObject("Publication",pub1);
        const Type& tpub1 = comp->getDataObject("Publication")->getType();
        cout << "Publication is now of type " << tpub1.getName() << endl;

        comp->setDataObject("Publication",book1);
        const Type& tpub2 = comp->getDataObject("Publication")->getType();
        cout << "Publication now is of type " << tpub2.getName() << endl;

        comp->setDataObject("Publication",mag1);
        const Type& tpub3 = comp->getDataObject("Publication")->getType();
        cout << "Publication now is of type " << tpub3.getName() << endl;

        /**
         * As the substitutes have names, they act as a sort of
         * alias, so we can address Publication as Book or Magazine too
         */

        comp->setDataObject("Book",book1);
        const Type& tpub4 = comp->getDataObject("Book")->getType();
        cout << "Book is of type " << tpub4.getName() << endl;

        comp->setDataObject("Magazine",mag1);
        const Type& tpub5 = comp->getDataObject("Magazine")->getType();
        cout << "Magazine is of type " << tpub5.getName() << endl;
    
    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in Substitutes"<< endl;
        cout<< e;
    }
    cout << " ********** Sample ends ********************" << endl;
}
