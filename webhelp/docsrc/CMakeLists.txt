PROJECT(mariadb_client_docs C)
# Minimum required version is Cmake 2.6.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0009 NEW)
ENDIF(COMMAND CMAKE_POLICY)

IF(WITH_BZR)
  EXEC_PROGRAM(bzr "${CMAKE_SOURCE_DIR}/revhistory"
               ARGS log --short --xml lp:mariadb-native-client > log.xml.in)
  EXEC_PROGRAM(xsltproc "${CMAKE_SOURCE_DIR}"
               ARGS ${CMAKE_SOURCE_DIR}/xsl/bzr.xsl ${CMAKE_SOURCE_DIR}/revhistory/log.xml.in > ${CMAKE_SOURCE_DIR}/revhistory/log.xml)
ENDIF()
EXEC_PROGRAM(xsltproc "${CMAKE_SOURCE_DIR}"
             ARGS ${CMAKE_SOURCE_DIR}/xsl/bzr.xsl ${CMAKE_SOURCE_DIR}/revhistory/log.xml.in > ${CMAKE_SOURCE_DIR}/revhistory/log.xml)

FILE(GLOB_RECURSE XML_FILES ${CMAKE_SOURCE_DIR} *.xml)
LIST(SORT XML_FILES)
FOREACH(xml ${XML_FILES})
  FILE(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${xml})
  STRING(REPLACE "/" "." relative_path ${relative_path})
  STRING(REPLACE ".xml" "" relative_path ${relative_path})
  SET(file_entities "${file_entities}<!ENTITY ${relative_path} SYSTEM 'file:///${xml}'>\n")
ENDFOREACH()

EXEC_PROGRAM(php "${CMAKE_SOURCE_DIR}"
               ARGS -f ${CMAKE_SOURCE_DIR}/scripts/add_funcref.php ${xml}
               OUTPUT_VARIABLE function
               RETURN_VARIABLE ret)
IF(function)
  SET(function_entities "${function_entities} ${function}\n")
ENDIF()

FILE(GLOB_RECURSE C_FILES ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/examples/*.c)
LIST(SORT C_FILES)
FOREACH(c_file ${C_FILES})
  FILE(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${c_file})
  STRING(REPLACE "/" "." relative_path ${relative_path})
  STRING(REPLACE ".c" "" relative_path ${relative_path})
  SET(file_entities "${file_entities}<!ENTITY ${relative_path} '<programlisting language=\"C\"><xi:include href=\"${c_file}\" parse=\"text\" xmlns:xi=\"http://www.w3.org/2001/XInclude\"/></programlisting>'>\n")
ENDFOREACH()

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/entities/file.ent.in
               ${CMAKE_SOURCE_DIR}/entities/file.ent
               NEWLINE_STYLE UNIX)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/entities/var.ent.in
               ${CMAKE_SOURCE_DIR}/entities/var.ent
               NEWLINE_STYLE UNIX)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/entities/funcref.ent.in
               ${CMAKE_SOURCE_DIR}/entities/funcref.ent
               NEWLINE_STYLE UNIX)
