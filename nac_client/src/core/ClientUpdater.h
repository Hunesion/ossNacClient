#ifndef _HUNE_CORE_CLIENT_UPDATE_H_
#define _HUNE_CORE_CLIENT_UPDATE_H_
#include <string>
#include <gtk/gtk.h>
#include "macro.h"



BEGIN_HUNE_CORE
class ClientUpdater
{
public:
    ClientUpdater(); 
    virtual ~ClientUpdater(); 

    bool isUpdate();
    bool update();
private:

private:

};

END_HUNE_CORE
#endif /* _HUNE_CORE_CLIENT_UPDATE_H_ */


