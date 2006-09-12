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

 TODO There is duplication in the "..no_parms" templates with
      the equivalent templates they are based on - that it would be 
      fairly easy to factor out. 
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<!-- Part of the org.apache.tuscany.sca.cpp.tools.services packages
   
     This stylesheet creates the CPP implementation of the 
     proxy body for a given SCA service
  --> 
<xsl:output method="text"/>
 
<!-- We do not generate output sourced from cppScopes -->
<xsl:template match="cppScope"></xsl:template>

<!-- We do not generate output sourced from private scaOperations -->
<xsl:template match="scaOperation[@cppScope='private']">
</xsl:template>

<!-- We do not generate output sourced from the operation return types -->
<xsl:template match="scaOperationReturnType">
</xsl:template>


<!--
    CPP Header/Root
-->
<xsl:template match="cppHeader">/*
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

<xsl:apply-templates select="scaService" mode="include_header"/>

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;<!-- thats the prologue over nad done with
     now continue with the scaServiceElement -->
<xsl:apply-templates/> <!-- should go on to generic scaService below -->
</xsl:template>
  
<!-- -->
<!-- Service -->
<!-- -->
<xsl:template match="scaService">
<xsl:variable name="clazz">
   <xsl:value-of select="../@implClass"/>
   <xsl:text>_</xsl:text>
   <xsl:value-of select="../@nameOfSorR"/>
   <xsl:text>_Proxy</xsl:text>
</xsl:variable>
<xsl:text>
extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    </xsl:text><xsl:value-of select="$clazz"/>
<xsl:text>* </xsl:text>
<xsl:value-of select="$clazz"/>
<xsl:text>_Factory(ServiceWrapper* target)
    {
        return new </xsl:text><xsl:value-of select="$clazz"/><xsl:text>(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void </xsl:text>
<xsl:value-of select="$clazz"/>
<xsl:text>_Destructor(void* proxy)
    {
        delete (</xsl:text><xsl:value-of select="$clazz"/><xsl:text>*)proxy;
    }
}

</xsl:text>
<xsl:value-of select="$clazz"/><xsl:text>::</xsl:text><xsl:value-of select="$clazz"/>(ServiceWrapper* targ) : target(targ)
{
}

<xsl:value-of select="$clazz"/><xsl:text>::~</xsl:text><xsl:value-of select="$clazz"/>()
{
    if (target)
        delete target;
}<!--
OK we have done the stuff that is not specific to any function in the service
now we can do another apply-templates to pick up on the 0, 1 or more scaOperation
DOM nodes that each relate to a function in the service
 -->
<xsl:apply-templates/>
<!-- Always make sure we have a (visible) final newline -->
<xsl:text>

</xsl:text>
</xsl:template>  

<!--  This template is just a small function that will generate the 
      #include that pulls in the correct C++ header
 -->
<xsl:template match="scaService" mode="include_header">
<xsl:variable name="clazz">
   <xsl:value-of select="../@implClass"/>
   <xsl:text>_</xsl:text>
   <xsl:value-of select="../@nameOfSorR"/>
   <xsl:text>_Proxy</xsl:text>
</xsl:variable>
<xsl:text>#include "</xsl:text>
<xsl:value-of select="$clazz"/>
<xsl:text>.h"</xsl:text>
</xsl:template>



<!-- -->
<!-- Operation -->
<!-- -->
<xsl:template match="scaOperation">
<xsl:variable name="clazz">
   <xsl:value-of select="../../@implClass"/>
   <xsl:text>_</xsl:text>
   <xsl:value-of select="../../@nameOfSorR"/>
   <xsl:text>_Proxy</xsl:text>
</xsl:variable>

<!-- ignore the constructor and destructor -->
<xsl:if test="@operationNameAttr != $clazz
             and @operationNameAttr != concat('~', $clazz)" >
<xsl:text>
</xsl:text>
<xsl:value-of select="scaOperationReturnType"/>
<xsl:text> </xsl:text>
<xsl:value-of select="$clazz"/><xsl:text>::</xsl:text>
<xsl:value-of select="@operationNameAttr"/><xsl:text>(</xsl:text>
<xsl:for-each select="scaOperationParameter">
<xsl:if test=".!='void'"> 
    <xsl:variable name="scaOperationParameterNameAttr">
                <xsl:value-of select="concat('arg', position()-1 )"/>
    </xsl:variable>
    <xsl:variable name="isConst">
       <xsl:value-of select="@scaOperationParameterConstAttr"/>
    </xsl:variable>
    <xsl:if test="contains($isConst,'true')">const</xsl:if><xsl:text> </xsl:text>
    <xsl:value-of select="."/> <!-- get the actual type -->
    <xsl:text> </xsl:text>
    <xsl:value-of select="$scaOperationParameterNameAttr"/>
    <xsl:choose>
       <xsl:when test="position()=last()"> 
       </xsl:when>
       <xsl:otherwise>
          <xsl:text>, </xsl:text>
       </xsl:otherwise>
    </xsl:choose>
</xsl:if> <!-- test=".!='void'" -->

</xsl:for-each>
<xsl:text>)
{</xsl:text>
<xsl:call-template name="proxy_method_body"/>
<xsl:text>
}
</xsl:text>      
</xsl:if>
</xsl:template>


<xsl:template name="proxy_method_body">
   <xsl:variable name="noOfParms">
      <xsl:choose>
        <xsl:when test=".='void'">
<xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="last()"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
<xsl:text>
    Operation operation("</xsl:text>
    <xsl:value-of select="@operationNameAttr"/>
    <xsl:text>");</xsl:text>
    <xsl:if test="$noOfParms!=0">
    <xsl:for-each select="scaOperationParameter">
        <xsl:call-template name="proxy_method_body_set_parameter"/>
    </xsl:for-each>
    </xsl:if>
    <xsl:call-template name="proxy_method_body_prepare_return_var"/>
    <xsl:text>
    target-&gt;invoke(operation);</xsl:text>
    <xsl:call-template name="proxy_method_body_return"/>
</xsl:template>


<xsl:template name="proxy_method_body_return">
    <xsl:variable name="type">
        <xsl:value-of select="scaOperationReturnType/text()"/>
    </xsl:variable>
    <xsl:choose>
        <xsl:when test="$type='void'"><!-- nothing -->
            <xsl:text>
    return;</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, '&amp;')"><!-- reference -->
            <xsl:variable name="type_no_amp">
                <xsl:value-of select="substring-before($type, '&amp;')"/>
            </xsl:variable>
            <xsl:text>
    return *(</xsl:text><xsl:value-of select="$type_no_amp"/><xsl:text>*)operation.getReturnValue();</xsl:text>
        </xsl:when>
        <xsl:otherwise><!-- simple type -->
        <xsl:text>
    return *(</xsl:text><xsl:value-of select="$type"/><xsl:text>*)operation.getReturnValue();</xsl:text>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="proxy_method_body_prepare_return_var">
    <xsl:variable name="type">
        <xsl:value-of select="scaOperationReturnType/text()"/>
    </xsl:variable>
    <xsl:choose>
        <xsl:when test="$type='void'"><!-- nothing -->
        <!--  nothing -->
        </xsl:when>
        <xsl:when test="contains($type, '&amp;')"><!-- reference -->
        </xsl:when>
        <xsl:otherwise>
<xsl:text>
    </xsl:text>    <xsl:value-of select="$type"/><xsl:text> ret;
    operation.setReturnValue(&amp;ret);</xsl:text>
        </xsl:otherwise>
    </xsl:choose>

</xsl:template>

 
<xsl:template name="proxy_method_body_set_parameter">
    <xsl:variable name="scaOperationParameterNameAttrInner">
                <xsl:value-of select="concat('arg', position()-1 )"/>
    </xsl:variable>
    <xsl:variable name="type">
        <xsl:value-of select="."/>
    </xsl:variable>
    operation.addParameter(&amp;<xsl:value-of select="$scaOperationParameterNameAttrInner"/><xsl:text>);</xsl:text>
</xsl:template>
</xsl:stylesheet>
