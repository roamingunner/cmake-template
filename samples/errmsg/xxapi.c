#include <stdbool.h>
#include "errmsg.h"
static bool gt_7(int x){
    if (x <= 7){
        update_errmsg("not greate then 7");
        return false;
    }
    return true;
}


static bool gt_8(int x){
    if (!gt_7(x))
        return false;
    
    if (x <= 8){
        update_errmsg("not greate then 8");
        return false;
    }
    return true;
}


static bool gt_9(int x){
    if (!gt_8(x))
        return false;
    
    if (x <= 9){
        update_errmsg("not greate then 9");
        return false;
    }
    return true;
}

bool gt_10(int x){
    if (!gt_9(x))
        return false;
    
    if (x <= 10){
        update_errmsg("not greate then 10");
        return false;
    }
    return true;
}

const char *xxapi_errmsg(void){
        return __emsg;
}
