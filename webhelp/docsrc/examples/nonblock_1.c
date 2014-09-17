static void run_query(const char *host, const char *user, const char *password) {
  int err, status;
  MYSQL mysql, *ret;
  MYSQL_RES *res;
  MYSQL_ROW row;

  mysql_init(&mysql);
  mysql_options(&mysql, MYSQL_OPT_NONBLOCK, 0);

  status = mysql_real_connect_start(&ret, &mysql, host, user, password, NULL, 0, NULL, 0);
  while (status) {
    status = wait_for_mysql(&mysql, status);
    status = mysql_real_connect_cont(&ret, &mysql, status);
  }

  if (!ret)
    fatal(&mysql, "Failed to mysql_real_connect()");

  status = mysql_real_query_start(&err, &mysql, SL("SHOW STATUS"));
  while (status) {
    status = wait_for_mysql(&mysql, status);
    status = mysql_real_query_cont(&err, &mysql, status);
  }
  if (err)
    fatal(&mysql, "mysql_real_query() returns error");

  /* This method cannot block. */
  res= mysql_use_result(&mysql);
  if (!res)
    fatal(&mysql, "mysql_use_result() returns error");

  for (;;) {
    status= mysql_fetch_row_start(&row, res);
    while (status) {
      status= wait_for_mysql(&mysql, status);
      status= mysql_fetch_row_cont(&row, res, status);
    }
    if (!row)
      break;
    printf("%s: %s\n", row[0], row[1]);
  }
  if (mysql_errno(&mysql))
    fatal(&mysql, "Got error while retrieving rows");
  mysql_free_result(res);
  mysql_close(&mysql);
}

/* Helper function to do the waiting for events on the socket. */
static int wait_for_mysql(MYSQL *mysql, int status) {
  struct pollfd pfd;
  int timeout, res;

  pfd.fd = mysql_get_socket(mysql);
  pfd.events =
    (status & MYSQL_WAIT_READ ? POLLIN : 0) |
    (status & MYSQL_WAIT_WRITE ? POLLOUT : 0) |
    (status & MYSQL_WAIT_EXCEPT ? POLLPRI : 0);
  if (status & MYSQL_WAIT_TIMEOUT)
    timeout = 1000*mysql_get_timeout_value(mysql);
  else
    timeout = -1;
  res = poll(&pfd, 1, timeout);
  if (res == 0)
    return MYSQL_WAIT_TIMEOUT;
  else if (res < 0)
    return MYSQL_WAIT_TIMEOUT;
  else {
    int status = 0;
    if (pfd.revents & POLLIN) status |= MYSQL_WAIT_READ;
    if (pfd.revents & POLLOUT) status |= MYSQL_WAIT_WRITE;
    if (pfd.revents & POLLPRI) status |= MYSQL_WAIT_EXCEPT;
    return status;
  }
}
