#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  MYSQL *mysql1, *mysql2;
  MYSQL_RES *result;
  char *query;

  /*Open two connetions with the same user */

  mysql1= mysql_init(NULL);
  mysql2= mysql_init(NULL);


  mysql_real_connect(mysql1, "localhost", "example", "example_pw", 
                            "example_db", 0, NULL, 0);
  mysql_real_connect(mysql2, "localhost", "example", "example_pw", 
                            "example_db", 0, NULL, 0);
  
  mysql_query(mysql1, "DROP TABLE IF EXISTS commit_exanple");
 
  /* This example will only work with transactional storage engines */
  mysql_query(mysql1, "CREATE TABLE commit_example(a int) ENGINE=InnoDB");

  /**************************************************
   Disable autocommit mode, insert two rows and 
   check the number of rows in a second connection
   before and after commit 
   **************************************************/
  mysql_autocommit(mysql1, 0);
  mysql_query(mysql1, "INSERT INTO commit_example VALUES (1),(2)");

  mysql_query(mysql2, "SELECT a FROM commit_example");
  if ((result= mysql_store_result(mysql2)))
  {
    printf("Expected 0 rows, found %d rows\n", mysql_num_rows(result));
    mysql_free_result(result);
  }

  /* now commit and check number of rows from 2nd connection */
  mysql_commit(mysql1);
  mysql_query(mysql2, "SELECT a FROM commit_example");
  if ((result= mysql_store_result(mysql2)))
  {
    printf("Expected 2 rows, found %d rows\n", mysql_num_rows(result));
    mysql_free_result(result);
  }

  mysql_close(mysql1);
  mysql_close(mysql2);
  return 0;
}
