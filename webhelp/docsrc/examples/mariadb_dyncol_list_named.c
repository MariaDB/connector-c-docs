#include <mysql.h>
#include <ma_dyncol.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  DYNAMIC_COLUMN dyncol;
  MYSQL_LEX_STRING *list_keys, my_keys[]= {{"key1", 4}, {"key2",4}, {"key3", 4}};
  char *my_vals[]= {"value1", "value2", "value3"};
  DYNAMIC_COLUMN_VALUE *vals;
  unsigned int i, column_count;
  CHARSET_INFO *cs= mysql_get_charset_by_name("utf8");

  mariadb_dyncol_init(&dyncol);

  vals= (DYNAMIC_COLUMN_VALUE *)malloc(sizeof(DYNAMIC_COLUMN_VALUE) * 3);
  for (i=0; i < 3; i++) 
  {
    vals[i].type= DYN_COL_STRING;
    vals[i].x.string.value.length= strlen(my_vals[i]);
    vals[i].x.string.value.str= my_vals[i];
    vals[i].x.string.charset= cs;
  }
  /* Add columns */
  if (mariadb_dyncol_create_many_named(&dyncol, 3, my_keys, vals, 0) != ER_DYNCOL_OK)
  {
    printf("Can't add columns\n");
    exit(-1);
  }

  if (mariadb_dyncol_list_named(&dyncol, &column_count, &list_keys) == ER_DYNCOL_OK)
  {
    for (i=0; i < column_count; i++)
      printf("Column: %u  Key: %s\n", i, list_keys[i].str);
    /* Don't forget to free the keys! */
    free(list_keys);
  }
  mariadb_dyncol_free(&dyncol);

  return 0;
}
