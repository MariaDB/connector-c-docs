<?php


  /* add_funcref.php

     Generates a function reference
   */

  $funcref_path= "api";

  $iter = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($funcref_path, RecursiveDirectoryIterator::KEY_AS_PATHNAME),
    RecursiveIteratorIterator::SELF_FIRST,
    RecursiveIteratorIterator::CATCH_GET_CHILD // Ignore "Permission denied"
);


$entities= "";
foreach ($iter as $path => $dir) {
    if (!$dir->isDir() && strpos($path, ".xml") > 0) {
/*
        $entities .= sprintf("<!ENTITY function.%s SYSTEM \"%s/%s/%s\">\n",
                              str_replace(".xml", "", basename($path)), 
                              getcwd(),
                              pathinfo($path, PATHINFO_DIRNAME),
                              pathinfo($path, PATHINFO_BASENAME));
*/
        $entities .= sprintf("<!ENTITY function.%s \"<link linkend='functions.%s'><function>%s</function></link>\">\n",
                              str_replace(".xml", "", basename($path)), 
                              str_replace(".xml", "", basename($path)), 
                              str_replace(".xml", "", basename($path)));
    }
}


print($entities);
?>
