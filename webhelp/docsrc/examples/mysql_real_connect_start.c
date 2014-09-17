#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_error(MYSQL *mysql)
{
  printf("Error(%d) [%s] \"%s\"", mysql_errno(mysql),
                                  mysql_sqlstate(mysql),
                                  mysql_error(mysql));
  mysql_close(mysql);
  exit(-1);
}

int main(int argc, char *argv[])
{
  MYSQL *mysql, *retval;
  int status;

  mysql= mysql_init(NULL);
  mysql_options(mysql, MYSQL_OPT_NONBLOCK, 0);

  status= mysql_real_connect_s(&retval, mysql, "localhost", "example", "example_pw", 
                          "example_db", 0, NULL, 0))
 
  if (status)
    printf("connect in progress ");

  while (status)
  {
    printf(".");
    status= mysql_real_connect_cont(&ret, mysql, status);
  }

  /* check the value of ret */
  if (!ret)
    show_error(mysql);

  printf("done!\n");

  mysql_close(mysql);
  return 0;
}
