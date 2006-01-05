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
  -  wrapper body for a given SCA service
  --> 
<xsl:output method="text"/>
 
<!-- Delete cppScopes -->
<xsl:template match="cppScope"></xsl:template>

<!-- Delete private methods cppScopes -->
<xsl:template match="scaOperation[@cppScope='private']">
</xsl:template>

<!-- Don't act on return types -->
<xsl:template match="scaOperationReturnType">
</xsl:template>


<!-- -->
<!-- CPP Header/Root -->
<!-- -->
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
using namespace tuscany::sca;
<!-- thats the prologue over and done with
     now continue with the scaServiceElement -->
<xsl:apply-templates/> <!-- will be applied to scaService -->
<!-- Always make sure we have a final newline -->
<xsl:text>

</xsl:text>
</xsl:template>
  
<!-- -->
<!-- Service -->
<!-- -->
<xsl:template match="scaService">
<xsl:variable name="class">
   <xsl:value-of select="../@implClass"/>
   <xsl:text>_</xsl:text>
   <xsl:value-of select="../@serviceName"/>
   <xsl:text>_Wrapper</xsl:text>
</xsl:variable>
<xsl:text>extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    </xsl:text><xsl:value-of select="$class"/>
<xsl:text>* </xsl:text>
<xsl:value-of select="$class"/>
<xsl:text>_Factory(tuscany::sca::model::Service* target)
    {
        return new </xsl:text><xsl:value-of select="$class"/><xsl:text>(target);
    }
}

</xsl:text>
<xsl:value-of select="$class"/><xsl:text>::</xsl:text><xsl:value-of select="$class"/><xsl:text>(Service* target) : ComponentServiceWrapper(target)
{
    impl = (</xsl:text><xsl:value-of select="../@implClass"/><xsl:text>*)getImplementation();
}

</xsl:text>
<xsl:value-of select="$class"/><xsl:text>::~</xsl:text><xsl:value-of select="$class"/><xsl:text>()
{
    releaseImplementation();
}

void* </xsl:text>
<xsl:value-of select="$class"/><xsl:text>::newImplementation()
{
    return new </xsl:text><xsl:value-of select="../@implClass"/><xsl:text>;
}

void </xsl:text>
<xsl:value-of select="$class"/><xsl:text>::deleteImplementation()
{
    delete impl;
}

void </xsl:text><xsl:value-of select="$class"/><xsl:text>::invokeService(Operation&amp; operation)
{
    const string&amp; operationName = operation.getName();

</xsl:text>
<xsl:apply-templates/><!-- an if statment for each operation -->
<xsl:text>        

    throw ServiceRuntimeException("Invalid operation");
    
}</xsl:text>
</xsl:template>  

<xsl:template match="scaService" mode="include_header">
<xsl:variable name="class">
   <xsl:value-of select="../@implClass"/>
   <xsl:text>_</xsl:text>
   <xsl:value-of select="../@serviceName"/>
   <xsl:text>_Wrapper</xsl:text>
</xsl:variable>
<xsl:text>#include "</xsl:text>
<xsl:value-of select="$class"/>
<xsl:text>.h"</xsl:text>
</xsl:template>

<!-- -->
<!-- Operation -->
<!-- -->
<xsl:template match="scaOperation">
<!--For example
    if (operationName == "getCustomerInformation")
    {
        operation.setReturnValue(impl->getCustomerInformation((const char*)parms[0]));
        return;
    }
-->
<xsl:if test="@operationNameAttr != ../../@intfClass
             and @operationNameAttr != concat('~', ../../@intfClass)" >
<xsl:text>    if (operationName == "</xsl:text>
<xsl:value-of select="@operationNameAttr"/>
<xsl:text>")
    {
</xsl:text>
            <xsl:call-template name="this_operation_body"/>
<xsl:text>
    }
</xsl:text>

</xsl:if>
</xsl:template>

<xsl:template name="this_operation_body">
<xsl:if test="./scaOperationParameter/text() != 'void' ">
  <xsl:if test="normalize-space(./scaOperationParameter/text()) != ' ' ">
    <xsl:if test="./scaOperationParameter/text() != '' ">
    <xsl:for-each select="scaOperationParameter">
            <xsl:call-template name="prepare_parameter_n"/>
    </xsl:for-each> 
  </xsl:if>
  </xsl:if>
</xsl:if>
<xsl:call-template name="invoke_for_return_type"/>
<xsl:text>
        return;</xsl:text>
</xsl:template>

<xsl:template name="prepare_parameter_n">
     <xsl:variable name="isConst">
        <xsl:value-of select="@scaOperationParameterConstAttr"/>
     </xsl:variable>
    <xsl:variable name="type">
        <xsl:value-of select="."/>
    </xsl:variable>
<xsl:text>        </xsl:text>
    <xsl:choose>
        <xsl:when test="contains($type, '&amp;')"><!-- reference -->
            <xsl:variable name="type_no_amp">
                <xsl:value-of select="substring-before($type, '&amp;')"/>
            </xsl:variable>
            <xsl:value-of select="$type"/><xsl:text> p</xsl:text><xsl:value-of select="position()-1"/><xsl:text> = *(</xsl:text><xsl:value-of select="$type_no_amp"/>
             <xsl:text>*)operation.getParameter(</xsl:text><xsl:value-of select="position()-1"/><xsl:text>);
</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, '*')"><!-- pointer -->
            <xsl:if test="contains($isConst,'true')">const </xsl:if>
            <xsl:value-of select="."/><xsl:text> p</xsl:text><xsl:value-of select="position()-1"/><xsl:text> = (</xsl:text>
             <xsl:if test="contains($isConst,'true')">const</xsl:if><xsl:text> </xsl:text>
              <xsl:value-of select="$type"/>
             <xsl:text>)operation.getParameter(</xsl:text><xsl:value-of select="position()-1"/><xsl:text>);
</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, 'DataObjectPtr')"><!-- DataObjectPtr -->
            <xsl:text>DataObjectPtr&amp; p</xsl:text><xsl:value-of select="position()-1"/><xsl:text> = *(DataObjectPtr*)operation.getParameter(</xsl:text><xsl:value-of select="position()-1"/><xsl:text>);</xsl:text>
        </xsl:when>
        <xsl:otherwise><!-- simple type -->
    <xsl:if test="contains($isConst,'true')">const </xsl:if><xsl:value-of select="$type"/><xsl:text>&amp; p</xsl:text><xsl:value-of select="position()-1"/><xsl:text> = *(</xsl:text>
    <xsl:if test="contains($isConst,'true')">const</xsl:if><xsl:text> </xsl:text>
          <xsl:value-of select="$type"/>
     <xsl:text>*)operation.getParameter(</xsl:text><xsl:value-of select="position()-1"/><xsl:text>);
</xsl:text>

        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="invoke_for_return_type">
    <xsl:variable name="type">
        <xsl:value-of select="./scaOperationReturnType/text()"/>
    </xsl:variable>
    <xsl:choose>
        <!-- VOID -->
        <xsl:when test="$type='void'">
<xsl:text>            </xsl:text>
            <xsl:call-template name="impl_arrow_op_brackets_and_parms"/><xsl:text>;</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, '&amp;')"><!-- reference -->
        <xsl:text>        </xsl:text><xsl:value-of select="$type"/><xsl:text> ret = </xsl:text><xsl:call-template name="impl_arrow_op_brackets_and_parms"/><xsl:text>;</xsl:text>
        <xsl:text>
        operation.setReturnValue((void*)&amp;ret);</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, '*')"><!-- pointer -->
        <xsl:text>
        operation.setReturnValue((void*) </xsl:text><xsl:call-template name="impl_arrow_op_brackets_and_parms"/><xsl:text>);</xsl:text>
        </xsl:when>
        <xsl:when test="contains($type, 'DataObjectPtr')"><!-- DataObjectPtr -->
        <xsl:text>        *(DataObjectPtr*)operation.getReturnValue() = </xsl:text><xsl:call-template name="impl_arrow_op_brackets_and_parms"/><xsl:text>;</xsl:text>
        </xsl:when>
        <xsl:otherwise><!-- simple type -->
            <xml:text>        *(</xml:text><xsl:value-of select="$type"/><xsl:text>*)operation.getReturnValue() = </xsl:text><xsl:call-template name="impl_arrow_op_brackets_and_parms"/><xsl:text>;</xsl:text>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="impl_arrow_op_brackets_and_parms"><!-- context is "scaOperation" -->
        <xml:text>impl-></xml:text><xsl:value-of select="./@operationNameAttr"/><xml:text>(</xml:text>
<xsl:if test="./scaOperationParameter/text() != 'void' ">
  <xsl:if test="./scaOperationParameter/text() != '' ">
    <xsl:if test="normalize-space(./scaOperationParameter/text()) != ' ' ">
    <xsl:for-each select="scaOperationParameter">
            <xsl:call-template name="parameter_for_parameter_list"/>
    </xsl:for-each>
</xsl:if>
</xsl:if>
</xsl:if>
<xsl:text>)</xsl:text>
</xsl:template>

<xsl:template name="parameter_for_parameter_list">
<xsl:text>p</xsl:text><xsl:value-of select="position()-1"/>
  <xsl:choose>
     <xsl:when test="position()=last()"></xsl:when>
     <xsl:otherwise>
        <xsl:text>, </xsl:text>
     </xsl:otherwise>
  </xsl:choose>
</xsl:template>



</xsl:stylesheet>
