<?xml version="1.0" encoding="WINDOWS-1251"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:template match="/profiling">
	<html>
	<body>
		<table border="1">
		  <tr bgcolor="#CCCCCC">
			<!--<td align="center"><strong>N</strong></td>-->
			<td align="center"><strong>Summary</strong></td>
			<td align="center"><strong>Calls</strong></td>
		    <td align="center"><strong>Average</strong></td>
			<td align="center"><strong>Address</strong></td>
			<td align="center"><strong>Function</strong></td>
		  </tr>
		  <!-- выбираем функции-->
		  <xsl:apply-templates select="prof">
			<!-- сортируем пустые функции-аналоги в конец
			<xsl:sort select="@analog = false()"/>-->
			<!-- сортируем функции-аналоги-->
			<xsl:sort select="@summary" data-type ="number" order="descending"/>
			<!-- сортируем функции-ipp-->
			<xsl:sort select="@name" order="ascending"/>
		  </xsl:apply-templates>
		</table>
		</body>
</html>
	</xsl:template>

	<xsl:template match="prof">
	  <tr bgcolor="#F5F5F5">
		<!--<td><xsl:value-of select="position()"/></td>-->
		<td><xsl:value-of select="@summary"/></td>
		<td><xsl:value-of select="@calls"/></td>
		<td><xsl:value-of select="@average"/></td>
		<td><xsl:value-of select="@addr"/></td>
		<td><xsl:value-of select="@name"/></td>
	  </tr>
	</xsl:template>
</xsl:stylesheet>