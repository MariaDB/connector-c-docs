#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  MYSQL *mysql;
  MYSQL_RES *result;

  mysql= mysql_init(NULL);

  mysql_real_connect(mysql, "localhost", "example", "example_pw", 
                            "example_db", 0, NULL, 0);

  printf("Current character set: %s\n",
          mysql_character_set_name(mysql));


  mysql_set_character_set(mysql, "latin2");
  printf("New character set: %s\n",
          mysql_character_set_name(mysql));

  mysql_close(mysql);
  return 0;
}
