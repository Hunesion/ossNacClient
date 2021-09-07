#include "huneuicommon.h"


static GtkWidget *s_msg_dlg = NULL;


GtkResponseType        
hune_ui_message_box(GtkMessageType type, GtkButtonsType buttons, const char *message)
{
    std::string param;
    GtkResponseType rv = GtkResponseType::GTK_RESPONSE_NONE;
    if (! message) {
        return rv;
    }
    param = message;
    rv = hune_ui_message_box(type, buttons, param);
    return rv;
}

GtkResponseType
hune_ui_message_box(GtkMessageType type, GtkButtonsType buttons, const std::string &message)
{
    std::string param;
    GtkResponseType rv;

    if (s_msg_dlg) {
        gtk_widget_destroy(s_msg_dlg);
        s_msg_dlg = NULL;
    }
    param = message;
    Hune::Core::StringUtils::replace(param, "\\n", "\n");

    s_msg_dlg = gtk_message_dialog_new(GTK_WINDOW(hune_ui_get_main_window()), GTK_DIALOG_MODAL, type, buttons, param.c_str(), NULL);
    rv = (GtkResponseType)gtk_dialog_run(GTK_DIALOG(s_msg_dlg));
    if (s_msg_dlg) {
        gtk_widget_destroy(s_msg_dlg);
        s_msg_dlg = NULL;
    }

    return rv;
}

void        
hune_ui_change_cursor(const char *cursor_name)
{
    if (! cursor_name) {
        return;
    }
    GdkCursor *c = gdk_cursor_new_from_name(gdk_display_get_default(), cursor_name);
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(hune_ui_get_main_window())), c);
    g_object_unref(c);
}