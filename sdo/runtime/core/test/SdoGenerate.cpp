/*
 *
 *  Copyright 2005 International Business Machines Corporation
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

/* $Id: SdoGenerate.cpp,v 1.2 2006/03/16 12:53:57 slattery Exp $ */

/* work in progress on generation of structures from sdos*/

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;



#include "sdotest.h"



using namespace commonj::sdo;


char* sdotest::convert(const char* uri)
{
    char* newval = (char*)malloc(strlen(uri) + 1);
    strcpy(newval,uri);

    char* c;
    while ((c = strchr(newval,'.')) != 0)
    {
        char * tmp = (char*)malloc(strlen(newval) + 5);
        strncpy(tmp,newval, c - newval);
        tmp[c-newval] = 0;
        strcat(tmp,"<dot>");
        strcat(tmp,c+1);
        free(newval);
        newval = tmp;
    }
    return newval;
}


void sdotest::generate(DataFactoryPtr fac)
{
    try {

        TypeList tl = fac->getTypes();

       
        cout << "// Generated structures " << endl;

        for (int i=0;i<tl.size();i++)
        {

            if (!strcmp(tl[i].getURI(),"commonj.sdo")) continue;

            char *uri = convert(tl[i].getURI());

            cout << "typedef struct _";
            if (uri) cout << uri << "_";
            cout << tl[i].getName();

            cout << "{" << endl;


            PropertyList pl = tl[i].getProperties();
            for (int j=0;j<pl.size();j++)
            {
                if (pl[j].isMany())
                {
                    cout << "struct _";
                    char *v = convert(pl[j].getType().getURI());
                    if (v) cout << v << "_";
                    cout << pl[j].getType().getName();
                    cout << "List *" << pl[j].getName() << ";" << endl;
                    if (v) free(v);
                }
                else 
                {
                    if (pl[j].getType().isDataType())
                    {
                        switch (pl[j].getTypeEnum())
                        {
                        case Type::BooleanType:

                            cout << "bool " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::ByteType:
                            cout << "char " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::BytesType:
                            cout << "char* " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::CharacterType:
                            cout << "wchar " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::DateType:
                            cout << "time_t " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::DoubleType:
                            cout << "long double " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::FloatType:
                            cout << "float " << pl[j].getName() <<  ";" << endl;
                            break;
                        case Type::IntegerType:
                            cout << "long " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::LongType:
                            cout << "int64_t " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::ShortType:
                            cout << "short " << pl[j].getName() << ";" << endl;
                            break;
                        case Type::StringType:
                        case Type::UriType:
                            cout << "wchar_t* " << pl[j].getName() << ";" << endl;
                            break;
                        default:
                            cout << "// unknown primitive: " << pl[j].getName() << endl;
                            break;
                        }
                    }
                    else
                    {
                        cout << "struct _";
                        char *v = convert(pl[j].getType().getURI());
                        if (v) cout << v << "_";
                        cout << pl[j].getType().getName();
                        cout << " *" << pl[j].getName() << ";" << endl;
                        if (v) free(v);
                    }
                }
            }

            cout << "} ";
            if (uri) cout << uri << "_";
            cout << tl[i].getName() << ";" << endl;
            if (uri) free(uri);

        }

        cout << "// generated Accessors" << endl;
    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in Generation" << endl;
        cout << e << endl;
    }
}

