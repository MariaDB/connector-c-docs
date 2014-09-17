#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  MYSQL *mysql;
  MYSQL_RES *result;
  MYSQL_ROW row;

  /*Open two connetions with the same user */

  mysql= mysql_init(NULL);

  mysql_real_connect(mysql, "localhost", "example", "example_pw", 
                            "example_db", 0, NULL, 0);

  mysql_query(mysql, "DROP TABLE IF EXISTS data_seek");
  mysql_query(mysql, "CREATE TABLE data_seek(id int not null primary key, "
                     "surname char(50))");

  mysql_query(mysql, "INSERT INTO data_seek VALUES (1, 'Rupert'),"
                     "(2, 'Hans'), (3, 'Willi'), (4, 'Archibald')");

  mysql_query(mysql, "SELECT id, surname FROM data_seek ORDER BY id");

  if ((result= mysql_store_result(mysql)))
  {
    /* seek to the 3rd row */
    mysql_data_seek(result, 2);
    if ((row= mysql_fetch_row(result)))
      printf("Id: %s Surname: %s\n", row[0], row[1]);
    mysql_free_result(result);
  }
  mysql_close(mysql);
}
