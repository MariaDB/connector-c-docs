#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  MYSQL *mysql;
  MYSQL_RES *result;
  char *query;

  mysql= mysql_init(NULL);

  mysql_real_connect(mysql, "localhost", "example", "example_pw", 
                            "example_db", 0, NULL, 0);
  
  mysql_query(mysql, "DROP TABLE IF EXISTS auto_commit_exanple");
 
  /* This example will only work with transactional storage engines */
  mysql_query(mysql, "CREATE TABLE auto_commit_example(a int) ENGINE=InnoDB");

  /**************************************************
   Check if autocommit mode is already on. There are
   three ways to check the autocommit mode:
   1. SHOW VARIABLES LIKE 'autocommit';
      returns ON/OFF
   2. SELECT @@autocommit;
      returns 1/0
   3. SELECT VARIABLE_VALUE FROM INFORMATION_SCHEMA.SESSION_VARIABLES
             WHERE VARIABLE_NAME='AUTOCOMMIT';
      returns ON/OFF
   **************************************************/
  mysql_query(mysql, "SELECT @@autocommit");
 
  if ((result= mysql_store_result(mysql)))
  {
    MYSQL_ROW row= mysql_fetch_row(result);
    if (row)
      printf("Autocommit is %s\n", atoi(row[0]) ? "ON" : "OFF");
    mysql_free_result(result);
  }

  /**************************************************
    1. enable autocommit
   **************************************************/
  mysql_autocommit(mysql, 1);

  mysql_query(mysql, "INSERT INTO auto_commit_example VALUES (1),(2)");

  /* now try to rollback, this should have no effect */
  mysql_rollback(mysql);

  mysql_query(mysql, "SELECT * FROM auto_commit_example");

  if ((result= mysql_store_result(mysql)))
  {
    printf("Expected 2 rows, got %u rows\n", mysql_num_rows(result));
    mysql_free_result(result);
  }
  mysql_query(mysql, "DELETE FROM auto_commit_example");

  /**************************************************
    2. disable autocommit
   **************************************************/
  mysql_autocommit(mysql, 0);
  mysql_query(mysql, "INSERT INTO auto_commit_example VALUES (1),(2)");

  /* now try to rollback, so table should contain now rows */
  mysql_rollback(mysql);

  mysql_query(mysql, "SELECT * FROM auto_commit_example");

  if ((result= mysql_store_result(mysql)))
  {
    printf("Expected 0 rows, got %u rows\n", mysql_num_rows(result));
    mysql_free_result(result);
  }

  mysql_query(mysql, "DROP TABLE auto_commit_example");

  mysql_close(mysql);
  return 0;
}
