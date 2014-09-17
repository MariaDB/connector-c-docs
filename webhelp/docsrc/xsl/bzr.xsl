<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet
[
  <!ENTITY dbns "http://docbook.org/ns/docbook">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output method="xml" encoding="utf-8" indent="yes"/>
  
  <xsl:param name="include.paths" select="1"/>
  <xsl:param name="include.copies" select="1"/>
  
  <xsl:template match="logs">
  <informaltable frame="none">
   <tgroup cols="2" colsep="0" rowsep="0">
<?dbhtml cellspacing="0" ?>
<?dbhtml cellpadding="1" ?>
    <thead>
     <row>
      <entry role="ma_table" colsep="0">Rev.</entry>
      <entry role="ma_table" colsep="0">Date/Description</entry>
     </row>
    </thead>
    <tbody>
        <xsl:apply-templates select="log"/>
    </tbody>
   </tgroup>
  </informaltable>
  </xsl:template>
  
  <xsl:template match="log">
    <row>
     <entry><emphasis role="strong"><xsl:apply-templates select="revno"/></emphasis></entry>
     <entry><xsl:apply-templates select="timestamp"/></entry>
   </row>
   <row>
    <entry role="ma_table"></entry>
    <entry role="ma_table"><xsl:apply-templates select="message"/></entry>
   </row>
  </xsl:template>
  
  <xsl:template match="revno">
      <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="committer"/>

  <xsl:template match="timestamp">
      <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="message">
        <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="branch-nick"/>

  <xsl:template match="logentry/@revision">
    <revnumber xmlns="&dbns;">
      <xsl:apply-templates/>
    </revnumber>
  </xsl:template>
  
  <xsl:template match="tag">
    <revremark xmlns="&dbns;">
      <xsl:apply-templates/>
    </revremark>
  </xsl:template>
  
  <xsl:template match="date">
    <date xmlns="&dbns;">
      <xsl:apply-templates/>
    </date>
  </xsl:template>
  
  <xsl:template match="author">
    <xsl:variable name="firstname" select="substring-before(., ' ')"/>
    <xsl:variable name="surname" select="substring-after(., ' ')"/>
    <author xmlns="&dbns;">
      <personname>
        <firstname><xsl:value-of select="$firstname"/></firstname>
        <!--<othername></othername>-->
        <surname><xsl:value-of select="$surname"/></surname>
      </personname>
      <email><xsl:value-of select="@email"/></email>
    </author>
  </xsl:template>
  
  <xsl:template match="msg">
    <para>
      <xsl:apply-templates/>
    </para>
  </xsl:template>
  
  <xsl:template match="paths">
    <xsl:if test="$include.paths != 0">
      <itemizedlist xmlns="&dbns;">
        <title>Paths</title>
        <xsl:apply-templates/>
      </itemizedlist>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="path">
    <listitem xmlns="&dbns;">
      <para>
        <xsl:apply-templates select="@action|text()"/>
      </para>
    </listitem>    
  </xsl:template>
  
  <xsl:template match="path/@action">
    <xsl:text>[</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>] </xsl:text>
  </xsl:template>
  <xsl:template match="path/text()">
    <filename xmlns="&dbns;">
      <xsl:value-of select="."/>
    </filename>
  </xsl:template>

  <xsl:template match="copyies">
    <xsl:if test="$include.paths != 0">
      <listitem xmlns="&dbns;">
        <title>Copies</title>
        <para>
          <xsl:apply-templates select="@*|text()"/>
        </para>
      </listitem>
    </xsl:if>
  </xsl:template>
  <xsl:template match="copy">
    <listitem xmlns="&dbns;">
      <para>
        <xsl:apply-templates select="@*|text()"/>
      </para>
    </listitem>    
  </xsl:template>
  
  <xsl:template match="copy/@source">
    <filename xmlns="&dbns;">
      <xsl:value-of select="."/>
    </filename>
  </xsl:template>
  <xsl:template match="copy/text()">
    <xsl:text> -> </xsl:text>
    <filename xmlns="&dbns;">
      <xsl:value-of select="."/>
    </filename>
  </xsl:template>
  
</xsl:stylesheet>
