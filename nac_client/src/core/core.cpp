#include <pthread.h>
#include "core.h"


BEGIN_HUNE_CORE
static StringResource s_StringResource ;
static bool s_core_init = false ;  

bool hune_core_init(){
    bool rv = false ;

    GlobalVar::init();

    rv = true ; 
    s_core_init = true ; 
    return rv ; 
}


void hune_core_dispose(){
    if (!s_core_init) return ; 

    GlobalVar::dispose();

    s_core_init = false ; 
}
END_HUNE_CORE