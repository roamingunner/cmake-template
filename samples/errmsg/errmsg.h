#pragma once
#include <string.h>
#define MAX_ERRMSG_SIZE         (128)

extern char __emsg[MAX_ERRMSG_SIZE];
#define clean_errmsg()  do {memset(__emsg,0,MAX_ERRMSG_SIZE);} while(0)
#define update_errmsg(msg, ...) do { clean_errmsg(); snprintf(__emsg,MAX_ERRMSG_SIZE, "[%s:%d]ERROR: " msg, __func__ , __LINE__ ,##__VA_ARGS__);} while(0)