#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DEF_XERROR_GLOBALS
#include "xerror.h"



static bool is_gt_7(int x){
    if (x <= 7){
        XERR_set_error(0,"not greate then 7");
        return false;
    }
    return true;
}


static bool is_gt_8(int x){
    if (!is_gt_7(x))
        return false;
    
    if (x <= 8){
        XERR_set_error(0,"not greate then 8");
        return false;
    }
    return true;
}


static bool is_gt_9(int x){
    if (!is_gt_8(x))
        return false;
    
    if (x <= 9){
        XERR_set_error(0,"not greate then 9");
        return false;
    }
    return true;
}

bool is_gt_10(int x){
    if (!is_gt_9(x))
        return false;
    
    if (x <= 10){
        XERR_set_error(0,"not greate then 10");
        return false;
    }
    return true;
}

int main(int argc, char **argv){
    printf("hello from %s\n",argv[0]);
    for(int i = 1; i < argc; i++){
        if (is_gt_10(atoi(argv[i])) == false){
            printf("ERROR:%s\n",XERR_get_error_string());
        }
    }
    return 0;
}