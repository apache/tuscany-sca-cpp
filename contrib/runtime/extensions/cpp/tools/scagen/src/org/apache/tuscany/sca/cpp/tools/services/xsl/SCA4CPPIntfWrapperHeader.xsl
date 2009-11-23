<!--
   Licensed to the Apache Software Foundation (ASF) under one
   or more contributor license agreements.  See the NOTICE file
   distributed with this work for additional information
   regarding copyright ownership.  The ASF licenses this file
   to you under the Apache License, Version 2.0 (the
   "License"); you may not use this file except in compliance
   with the License.  You may obtain a copy of the License at
   
     http://www.apache.org/licenses/LICENSE-2.0
     
   Unless required by applicable law or agreed to in writing,
   software distributed under the License is distributed on an
   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
   KIND, either express or implied.  See the License for the
   specific language governing permissions and limitations
   under the License.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<!-- Part of the org.apache.tuscany.sca.cpp.tools.services packages
  - 
  -  This stylesheet creates the CPP implementation of the 
  -  wrapper header for a given SCA service
  -->
    <xsl:output method="text" />

    <!-- Delete cppScopes -->
    <xsl:template match="cppScope"></xsl:template>

    <!-- Delete private methods cppScopes -->
    <xsl:template match="scaOperation[@cppScope='private']">
    </xsl:template>

    <!-- Delete private methods cppScopes -->
    <xsl:template match="scaOperationReturnType"></xsl:template>


    <!-- -->
    <!-- CPP Header/Root -->
    <!-- -->
    <xsl:template match="cppHeader">
        <xsl:variable name="class">
               <xsl:value-of select="concat(@implClass, '_', @serviceName, '_Wrapper')"/>    
        </xsl:variable>
<xsl:text>/*
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

</xsl:text>
        <xsl:call-template name="ifndef_start">
            <xsl:with-param name="class" 
                            select="$class"/>
        </xsl:call-template>


        <xsl:call-template name="include_headers">
            <xsl:with-param name="header" 
                            select="@compositeXmlFileHeader"/>
        </xsl:call-template>

        <xsl:apply-templates select="scaService" mode="class_body"/>

        <xsl:call-template name="ifndef_end">
            <xsl:with-param name="class" 
                            select="$class"/>
        </xsl:call-template>
<!-- Always make sure we have a (visible) final return -->
<xsl:text>

</xsl:text>
</xsl:template>


<!-- Here are the subroutine templates in order -->

    <xsl:template name="ifndef_start">
        <!--For example,  
            #ifndef CustomerInfoImpl_CustomerInfoService_Wrapper_h
            #define CustomerInfoImpl_CustomerInfoService_Wrapper_h
        -->
        <xsl:param name="class"/>
        <xsl:text>#ifndef </xsl:text><xsl:value-of select="$class"/><xsl:text>_h</xsl:text>
<!-- newline -->
<xsl:text>
</xsl:text>
        <xsl:text>#define </xsl:text><xsl:value-of select="$class"/><xsl:text>_h</xsl:text>
<!-- newline -->
<xsl:text>

</xsl:text>
		<xsl:text>#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif </xsl:text>
<!-- newline -->
<xsl:text>

</xsl:text>
    </xsl:template>

    <xsl:template name="include_headers">
        <!--For example,  
            #include "CustomerInfoImpl.h"
            #include "tuscany/sca/cpp/CPPServiceWrapper.h"
        -->
        <xsl:param name="header"/>
        <xsl:text>#include "</xsl:text><xsl:value-of select="translate($header,'\','/')"/><xsl:text>"</xsl:text>
<!-- newline -->
<xsl:text>
</xsl:text>
        <xsl:text>#include "tuscany/sca/cpp/CPPServiceWrapper.h"</xsl:text>
    </xsl:template>

    <xsl:template match="scaService" mode="class_body">
        <!--For example
            class wrapperclassname : public tuscany::sca::cpp::CPPServiceWrapper
            {
             constructor
             destructor
                 then apply operation templates for operations
            };

            E.g. Specifically

        class CustomerInfoImpl_CustomerInfoService_Wrapper : public tuscany::sca::cpp::CPPServiceWrapper
        {
        public:
            CustomerInfoImpl_CustomerInfoService_Wrapper(tuscany::sca::model::Component*);
            virtual ~CustomerInfoImpl_CustomerInfoService_Wrapper();
            virtual void invoke(tuscany::sca::Operation& operation);
    
        private:
            CustomerInfoImpl* impl;
        };
        -->
        <!-- TODO second definition, remove? -->
        <xsl:variable name="class">
               <xsl:value-of select="concat(../@implClass, '_', ../@serviceName, '_Wrapper')"/>    
        </xsl:variable>
<xsl:text>

class </xsl:text>
<xsl:value-of select="$class"/>
<xsl:text> : public tuscany::sca::cpp::CPPServiceWrapper
{
public:
    </xsl:text><xsl:value-of select="$class"/><xsl:text>(tuscany::sca::model::Service* target);</xsl:text>
    <xsl:text>
    virtual ~</xsl:text><xsl:value-of select="$class"/><xsl:text>();
    virtual void invokeService(tuscany::sca::Operation&amp; operation);
    virtual void* newImplementation();
    virtual void deleteImplementation();
</xsl:text>
<xsl:text>private:
    </xsl:text>
    <xsl:value-of select="../@implNamespace"/><xsl:value-of select="../@implClass"/><xsl:text>* impl;
};
</xsl:text>
    </xsl:template>

    <xsl:template name="ifndef_end">
        <!--For example,  
            #endif // CustomerInfoImpl_CustomerInfoService_Wrapper_h          
        -->
        <xsl:param name="class"/>
        <xsl:text>
#endif // </xsl:text><xsl:value-of select="$class"/><xsl:text>_h</xsl:text>
    </xsl:template>

<xsl:template match="scaOperation">
</xsl:template>

</xsl:stylesheet>



