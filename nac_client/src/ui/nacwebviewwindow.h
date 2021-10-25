#ifndef _NAC_UI_WEB_VIEW_WINDOW_H_
#define _NAC_UI_WEB_VIEW_WINDOW_H_   
#include "huneuicommon.h"

#define NAC_TYPE_WEB_VIEW_WINDOW (nac_web_view_window_get_type ())
G_DECLARE_FINAL_TYPE (NacWebViewWindow, nac_web_view_window, NAC, WEB_VIEW_WINDOW, GtkWindow)


NacWebViewWindow*           nac_web_view_window_new(GtkWindow *parent);

GtkResponseType             nac_web_view_window_show(NacWebViewWindow *window, const char *uri);

#endif /* _NAC_UI_WEB_VIEW_WINDOW_H_ */
