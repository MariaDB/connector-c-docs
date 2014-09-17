<?php

$funcref_path= ".";

$iter = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($funcref_path, RecursiveDirectoryIterator::KEY_AS_PATHNAME),
    RecursiveIteratorIterator::SELF_FIRST,
    RecursiveIteratorIterator::CATCH_GET_CHILD // Ignore "Permission denied"
    );


$entities= "";
foreach ($iter as $path => $dir) {
  if (!$dir->isDir() && strpos($path, ".xml") > 0 && strpos($path, ".in") == FALSE) {
    $buffer= file($path);
    $version=NULL;
    $retval= 0;
    $itable= 0;
    $entry= 0;
    if ($fp= fopen($path . ".tmp", "w"))
    for ($i=0; $i < count($buffer); $i++)
    {
      $buffer[$i]= str_replace("\n", "", $buffer[$i]);
      $line= trim($buffer[$i]);

      if ($line == '<informaltable frame="none">')
      {
        $itable= 1;
        $i++;
        fprintf($fp, "<para>\n<variablelist>\n");
        continue;
      }
      if ($line == '</informaltable>')
      {
        printf("itable off\n");
        $itable= 0;
        fprintf($fp, "</variablelist>\n</para>\n");
        continue;
      }

      if ($line == '<entry>&param_in;</entry>' ||
          $line == '<entry>&param_out;</entry>' ||
          $line == '<entry>&param_inout;</entry>')
          continue;

      if ($itable == 1)
      {
        if (stristr($line, "<entry>"))
        {
          if ($entry == 0)
          {
            $line= str_replace("<entry>", "<term><parameter>", $line);
            $line= str_replace("</entry>", "</parameter></term>", $line);
            $entry= 1;
          }
          else
          {
            $line= str_replace("<entry>", "<listitem><para>", $line);
            $line= str_replace("</entry>", "</para></listitem>", $line);
            $entry= 0;
          }
          fprintf($fp, $line);
          fprintf($fp, "\n");
          $line= "";
          continue;
        }

        if (strstr($line, "<tgroup>") ||
          strstr($line, "?dbhtml") ||
          $line == "</tgroup>" ||
          $line == "<tbody>" ||
          $line == "</tbody>")
        {
          $line= "";
          continue;
        }

        if ($line == "<row>")
        {
          fprintf($fp, "<varlistentry>\n");
          continue;
        }
        if ($line == "</row>")
        {
          fprintf($fp, "</varlistentry>\n");
          continue;
        }
      }

      if ($line == "&version_1_0;")
      {
        $version="1.0";
        continue;
      }
      if ($line == "&version_1_1;")
      {
        $version="1.1";
        continue;
      }
      if ($line == "&version_2_0;")
      {
        $version="2.0";
        continue;
      }
      if ($line == '<refsect1 role="description">')
      {
        $buffer[$i].= "\n&reftitle.description;";
      }
      if ($line == '<refsect1 role="returnvalue">')
      {
        $retval++;
        if ($retval == 2 && $version)
        {
          fprintf($fp, "<refsect1 role='version'>\n&reftitle.version;\n<para>\n");
          fprintf($fp, "&product_name; %s\n</para>\n</refsect1>\n", $version); 
        }
      }


      if ($line == "<title></title>")
        continue;
      if ($line == "<note>")
        $buffer[$i]= "&reftitle.notes;\n<itemizedlist>";
      if ($line == "</note>")
        $buffer[$i]= "</itemizedlist>";

      if ($line == "<title>")
      {
        $title= trim($buffer[$i+1]);
        if ($title == "See also")
        {
          $line= "&reftitle.seealso;";
          $i+= 2;
        }
        if ($title == "Parameters")
        {
          $line= "&reftitle.parameters;";
          $i+= 2;
        }
        if ($title == "Notes")
        {
          $line= "&reftitle.notes;";
          $i+= 2;
        }
        if ($title == "Return value")
        {
          $line= "&reftitle.retval;";
          $i+= 2;
        }
        fprintf($fp, "%s\n", $line);
        continue;
      }

      fprintf($fp, "%s\n", $buffer[$i]);
    }
    fclose($fp);
    copy($path . ".tmp", $path);
    unlink($path . ".tmp");
  }
}



          ?>
