#ifndef _NAC_UI_MAIN_WINDOW_H_
#define _NAC_UI_MAIN_WINDOW_H_
#include "nacapplication.h"

#define NAC_TYPE_MAIN_WINDOW (nac_main_window_get_type ())
G_DECLARE_FINAL_TYPE (NacMainWindow, nac_main_window, NAC, MAIN_WINDOW, GtkApplicationWindow)



NacMainWindow       *nac_main_window_new          (NacApplication *app);
void                 nac_main_window_open         (NacMainWindow *main_window,
                                                         GFile            *file);

#endif /* _NAC_UI_MAIN_WINDOW_H_ */
