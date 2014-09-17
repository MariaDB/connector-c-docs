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

  /* this error comes from the client library */
  if (mysql_set_character_set(mysql, "utf19"))
    printf("Error: %s\n", mysql_error(mysql));

  /* this error comes from the server */
  if (mysql_query(mysql, "SELECT anycolumn FROM anonexisting_table"))
    printf("Error: %s\n", mysql_error(mysql));

  mysql_close(mysql);
}
