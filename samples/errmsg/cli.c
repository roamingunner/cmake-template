#include <stdio.h>
#include <stdlib.h>
#include "xxapi.h"

int main(int argc, char **argv){
    printf("hello from %s\n",argv[0]);
    for(int i = 1; i < argc; i++){
        if (gt_10(atoi(argv[i])) == false){
            printf("gt_10 error:%s\n",xxapi_errmsg());
        }
    }
    return 0;
}