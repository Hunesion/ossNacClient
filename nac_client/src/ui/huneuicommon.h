#ifndef _HUNE_UI_COMMON_H_
#define _HUNE_UI_COMMON_H_
#include <gtk/gtk.h>
#include "../core/core.h"

gpointer    hune_ui_get_main_window();
gpointer    hune_ui_get_application();

GtkResponseType hune_ui_message_box(GtkMessageType type, GtkButtonsType buttons, const char *message);
GtkResponseType hune_ui_message_box(GtkMessageType type, GtkButtonsType buttons, const std::string &message);
void        hune_ui_change_cursor(const char *cursor_name);

#endif