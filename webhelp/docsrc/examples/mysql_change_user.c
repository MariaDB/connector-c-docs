#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  MYSQL *mysql;
  MYSQL_RES *result;

  mysql= mysql_init(NULL);

  mysql_real_connect(mysql, "localhost", "root", NULL, 
                            "example_db", 0, NULL, 0);

  /**************************************************
   This example will not work if example user has not
   enough privileges to create a new user/role.
   **************************************************/
  if (mysql_query(mysql, "GRANT ALL ON example_db.* TO 'testuser'@'localhost'"
                         " IDENTIFIED by 'testpassword'"))
  {
    printf("Error (%d): %s\n", mysql_errno(mysql), mysql_error(mysql));
    mysql_close(mysql);
    return 1;
  }

  mysql_autocommit(mysql, 0);

  mysql_query(mysql, "DROP TABLE IF EXISTS t1");
  mysql_query(mysql, "CREATE TABLE t1 (id int, surname varchar(25)) ENGINE=InnoDB");

  mysql_query(mysql, "INSERT INTO t1 VALUES (9, 'Henrik')");

  /* change default database */
  mysql_select_db(mysql, "test");

  /**************************************************
   now we change the user for the current connection 
   and check if the last statement was rolled back
  **************************************************/
  mysql_change_user(mysql, "testuser", "testpassword", "example_db");
  
  mysql_query(mysql, "SELECT id, surname FROM t1");
  if ((result= mysql_store_result(mysql)))
  {
    printf("Expected 0 rows, got %u rows\n", mysql_num_rows(result));
    mysql_free_result(result);
  }

  /* make sure that we are connected as another user 
     to another database */
  mysql_query(mysql, "SELECT CURRENT_USER(), DATABASE()");
  if ((result= mysql_store_result(mysql)))
  {
    MYSQL_ROW row= mysql_fetch_row(result);
    if (row)
      printf("User %s is now connected to database %s\n", row[0], row[1]);
    mysql_free_result(result);
  }

  mysql_close(mysql);
  return 0;
}
