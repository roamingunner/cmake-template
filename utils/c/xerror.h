/**
 * @file xerror.h
 * @author leo
 * @brief 
 * include this file in your source code. And define DEF_XERROR_GLOBALS marco in one of source file.
 * @version 0.1
 * @date 2023-03-07
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef DEF_XERROR_GLOBALS
#define EXTERN 
#else
#define EXTERN extern
#endif

#define MAX_ERRMSG_SZ		256
typedef struct __xerror_state{
    int __errno;
    char __errmsg[MAX_ERRMSG_SZ];
}XERROR;

/*
#define CLOLOR_NONE			"\e[0m"
#define CLOLOR_RED			"\e[0;31m"
*/

EXTERN __thread XERROR _xerr;

void XERR_clean_error(void){
    memset(&_xerr,0,sizeof(XERROR));
}

int XERR_get_error(void){
    return _xerr.__errno;
}

const char * XERR_get_error_string(void){
    return _xerr.__errmsg;
}

#define XERR_set_error(xerrno, xerrmsg, ...) \
    do {\
        XERR_clean_error();\
        _xerr.__errno = xerrno ;\
        snprintf(_xerr.__errmsg ,MAX_ERRMSG_SZ,"[%s:%d]:" xerrmsg "\n",__func__,__LINE__, ##__VA_ARGS__);\
    }while(0)


/**
 * example..
 * 
#define DEF_XERROR_GLOBALS
#include "xerror.h"

int main(int argc, char const *argv[])
{

    XERR_set_error(0,"hello world");
    printf("XERROR:%d,%s",XERR_get_error(),XERR_get_error_string());
    XERR_set_error(-ENOMEM,"No Memory...");
    printf("XERROR:%d,%s",XERR_get_error(),XERR_get_error_string());
    return 0;
}

 */