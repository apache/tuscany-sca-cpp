<!-- 
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
 *  WITHOUT WARRANTY
 */ 
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<!-- Part of the org.apache.tuscany.sca.cpp.tools.services packages
  - 
  -  This stylesheet creates the CPP implementation of the 
  -  proxy header for a given SCA service
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
               <xsl:value-of select="concat(@implClass, '_', @nameOfSorR, '_Proxy')"/>    
        </xsl:variable>
<xsl:text>/* 
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

</xsl:text>
        <xsl:call-template name="ifndef_start">
            <xsl:with-param name="class" 
                            select="$class"/>
        </xsl:call-template>


        <xsl:call-template name="include_headers">
            <xsl:with-param name="componentTypeHeader" 
                            select="@componentTypeHeader"/>
        </xsl:call-template>

        <xsl:apply-templates select="scaService" mode="class_body"/>

        <xsl:call-template name="ifndef_end">
            <xsl:with-param name="class" 
                            select="$class"/>
        </xsl:call-template>
<!-- Always make sure we have a (visible) final newline -->
<xsl:text>

</xsl:text>
</xsl:template>


<!-- Here are the subroutine templates in order -->

    <xsl:template name="ifndef_start">
        <!--For example,  
            #ifndef CustomerInfoImpl_CustomerInfoService_Proxy_h
            #define CustomerInfoImpl_CustomerInfoService_Proxy_h
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
            #include "CustomerInfo.h"
            #include "tuscany/sca/core/ServiceWrapper.h"
        -->
        <xsl:param name="componentTypeHeader"/>
        <xsl:text>#include "</xsl:text><xsl:value-of select="translate($componentTypeHeader,'\','/')"/><xsl:text>"</xsl:text>
<!-- newline -->
<xsl:text>
</xsl:text>
        <xsl:text>#include "tuscany/sca/core/ServiceWrapper.h"</xsl:text>
    </xsl:template>

    <xsl:template match="scaService" mode="class_body">
        <!--For example
            class proxyclassname : public interface_class
            {
             constructor
             destructor
                 then apply operation templates for operations
            };

            E.g. Specifically

            class CustomerInfoImpl_CustomerInfoService_Proxy : public CustomerInfo
            {
            public:
                CustomerInfoImpl_CustomerInfoService_Proxy(tuscany::sca::ServiceWrapper*);
                virtual ~CustomerInfoImpl_CustomerInfoService_Proxy();
                virtual const char* getCustomerInformation(const char* customerID);
            private:
                tuscany::sca::ServiceWrapper* target;
            };                 
        -->
        <!-- TODO second definition, remove? -->
        <xsl:variable name="class">
               <xsl:value-of select="concat(../@implClass, '_', ../@nameOfSorR, '_Proxy')"/>    
        </xsl:variable>
<xsl:text>

class </xsl:text>
<xsl:value-of select="$class"/>
<xsl:text> : public </xsl:text><xsl:value-of select="../@intfClass"/>
<xsl:text>
{
public:
    </xsl:text><xsl:value-of select="$class"/><xsl:text>(tuscany::sca::ServiceWrapper*);</xsl:text>
    <xsl:text>
    virtual ~</xsl:text><xsl:value-of select="$class"/><xsl:text>();
</xsl:text>
    <!-- operations -->
    <xsl:apply-templates/>
<xsl:text>private:
    tuscany::sca::ServiceWrapper* target;
};
</xsl:text>
    </xsl:template>

    <xsl:template name="ifndef_end">
        <!--For example,  
            #endif // CustomerInfoImpl_CustomerInfoService_Proxy_h          
        -->
        <xsl:param name="class"/>
        <xsl:text>
#endif // </xsl:text><xsl:value-of select="$class"/><xsl:text>_h</xsl:text>
    </xsl:template>






<!-- Operation -->
<!--For example 
    virtual const char* getCustomerInformation(const char* customerID);
-->
<xsl:template match="scaOperation">
<xsl:variable name="class">
    <xsl:value-of select="concat(../../@implClass, '_', ../../@nameOfSorR, '_Proxy')"/>    
</xsl:variable>
<xsl:if test="@operationNameAttr != $class
             and @operationNameAttr != concat('~', $class)" >
<xsl:text>    virtual </xsl:text>
<xsl:value-of select="scaOperationReturnType"/>
<xsl:text> </xsl:text>
<xsl:value-of select="@operationNameAttr"/><xsl:text>(</xsl:text>
<xsl:for-each select="scaOperationParameter">
     <xsl:variable name="isConst">
        <xsl:value-of select="@scaOperationParameterConstAttr"/>
     </xsl:variable>
     <xsl:variable name="noOfParms">
        <xsl:value-of select="last()"/>
     </xsl:variable>
     <xsl:if test="contains($isConst,'true')">const</xsl:if><xsl:text> </xsl:text>
          <xsl:value-of select="."/> <!-- get the actual type -->
     <xsl:text> </xsl:text>
     <xsl:value-of select="@scaOperationParameterNameAttr"/>
  <xsl:choose>
     <xsl:when test="position()=last()"></xsl:when>
     <xsl:otherwise>
        <xsl:text>, </xsl:text>
     </xsl:otherwise>
  </xsl:choose>
  </xsl:for-each> 
<xsl:text>);
</xsl:text>
</xsl:if>
</xsl:template>
</xsl:stylesheet>



