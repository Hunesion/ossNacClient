#ifndef _NAC_UI_APPLICATION_H_
#define _NAC_UI_APPLICATION_H_
#include <gtk/gtk.h>
#include "Event.h"
#include "HuneEventCallback.h"

#define FTC_TYPE_APPLICATION (nac_application_get_type ())
G_DECLARE_FINAL_TYPE (NacApplication, nac_application, NAC, APPLICATION, GtkApplication)


NacApplication     *nac_application_new         (void);


#endif /* _NAC_UI_APPLICATION_H_ */
