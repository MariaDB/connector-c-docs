<?xml version='1.0'?> 
<xsl:stylesheet  
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:import href="../../docbook/webhelp/xsl/webhelp.xsl"/>
<xsl:param name="chunker.output.method">
  <xsl:choose>
    <xsl:when test="contains(system-property('xsl:vendor'), 'SAXON 6')">saxon:xhtml</xsl:when>
    <xsl:otherwise>xml</xsl:otherwise>
  </xsl:choose>
</xsl:param>

</xsl:stylesheet> 
