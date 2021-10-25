#include <gtk/gtk.h>
#include "ui/nacapplication.h"

// USING_FTC_CORE ; 

int main(int argc , char **argv){
    int rv = 0 ; 

    NacApplication *app = NULL ; 
    //HUNE_LOG("Main Application Started"); 
    app = nac_application_new(); 
    if (app == NULL) return rv ; 
    rv =  g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app); 
    //HUNE_LOG("Main Application End"); 
    return rv ; 
}