#include "nacwebviewwindow.h"
#include <webkit2/webkit2.h>
#include <string>

//////////  구현부  ////////

struct _NacWebViewWindow
{
    GtkWindow parent;
};

typedef struct _NacWebViewWindowPrivate NacWebViewWindowPrivate;
struct _NacWebViewWindowPrivate
{
    /* 소스에서 정의 */
    GtkWindow *parent;
    GtkResponseType response;

    /* template에서 정의 */
    GtkBox *box_contents_area;
};

G_DEFINE_TYPE_WITH_PRIVATE(NacWebViewWindow, nac_web_view_window, GTK_TYPE_WINDOW);



////////////////////////////////


//////////  멤버 함수  //////////
static void nac_web_view_window_on_key_release_event(GtkWidget *widget, GdkEventKey *event);

static gboolean nac_web_view_window_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static void nac_web_view_window_create_webview(NacWebViewWindow *window, const char *uri);


static void 
nac_web_view_window_init (NacWebViewWindow *win)
{
    NacWebViewWindowPrivate *priv = NULL;
    gtk_widget_init_template (GTK_WIDGET (win));
    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(win);

    //  Signal (Event) 함수 연결
    //  창이 닫힐 때 이벤트
    //  window key release
    g_signal_connect(G_OBJECT(win), "key_release_event", G_CALLBACK(nac_web_view_window_on_key_release_event), NULL);

    //  window close event
    g_signal_connect(G_OBJECT(win), "delete-event", G_CALLBACK(nac_web_view_window_on_delete_event), NULL);

    //  모달설정
    gtk_window_set_transient_for(GTK_WINDOW(win), priv->parent);
    gtk_window_set_keep_above(GTK_WINDOW(win), true);
    gtk_window_set_modal(GTK_WINDOW(win), true);
    //  리사이즈 불가
    gtk_window_set_resizable(GTK_WINDOW(win), false);
    
    priv->response = GTK_RESPONSE_CANCEL;
}

static void
nac_web_view_window_dispose (GObject *object)
{
    NacWebViewWindowPrivate *priv = NULL;
    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(NAC_WEB_VIEW_WINDOW(object));
    if (! priv) {
        return;
    }

    Hune::Core::EventManager::getInstance()->removeEventListener(object);

    G_OBJECT_CLASS(nac_web_view_window_parent_class)->dispose(object);
}

static void
nac_web_view_window_finalize(GObject *object)
{
    G_OBJECT_CLASS(nac_web_view_window_parent_class)->finalize(object);
}

static void 
nac_web_view_window_class_init (NacWebViewWindowClass *clz)
{
    G_OBJECT_CLASS (clz)->finalize = nac_web_view_window_finalize;
    G_OBJECT_CLASS (clz)->dispose = nac_web_view_window_dispose;

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS(clz), "/com/hunesion/nacclient/nac-web-view-window.ui");
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacWebViewWindow, box_contents_area);
    
}

NacWebViewWindow*
nac_web_view_window_new (GtkWindow *parent)
{
    NacWebViewWindow *window = NULL;
    NacWebViewWindowPrivate *priv = NULL;

    if (! parent) {
        return NULL;
    }

    window = (NacWebViewWindow *)g_object_new (NAC_TYPE_WEB_VIEW_WINDOW, NULL);
    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(window);;
    if (! priv) {
        gtk_widget_destroy(GTK_WIDGET(window));
        return NULL;
    }
    priv->parent = parent;

    return window;
}

GtkResponseType
nac_web_view_window_show(NacWebViewWindow* window, const char *uri)
{
    NacWebViewWindowPrivate *priv = NULL;
    GtkResponseType rv = GtkResponseType::GTK_RESPONSE_CANCEL;
    if (! NAC_IS_WEB_VIEW_WINDOW(window)) {
        return rv;
    }
    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(window);
    if (! priv) {
        return rv;
    }

    nac_web_view_window_create_webview(window, uri);

    gtk_window_set_position(GTK_WINDOW(window), GtkWindowPosition::GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_widget_show(GTK_WIDGET(window));
    gtk_main();

    rv = priv->response;

    return rv;
}

static void 
nac_web_view_window_on_key_release_event(GtkWidget *widget, GdkEventKey *event)
{
    NacWebViewWindow *window = NULL;
    NacWebViewWindowPrivate *priv = NULL;

    if (! NAC_IS_WEB_VIEW_WINDOW(widget)) {
        return;
    }
    window = NAC_WEB_VIEW_WINDOW(widget);

    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(window);
    if (! priv) {
        return;
    }

    if (event->keyval == GDK_KEY_Return) {

    }
}

static gboolean
nac_web_view_window_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    //  로그인윈도우를 호출한 윈도우에서 삭제처리하기 위함
    gtk_widget_hide(widget);
    gtk_main_quit();
    return true;
}

static void 
nac_web_view_window_create_webview(NacWebViewWindow *window, const char *uri)
{
    NacWebViewWindowPrivate *priv = NULL;
    WebKitWebView *webview = NULL;

    if (! NAC_IS_WEB_VIEW_WINDOW(window)) {
        return;
    }
    priv = (NacWebViewWindowPrivate*)nac_web_view_window_get_instance_private(window);
    if (! priv) {
        return;
    }

    webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    if (! webview) {
        return;
    }

    gtk_widget_set_vexpand(GTK_WIDGET(webview), true);
    gtk_widget_set_hexpand(GTK_WIDGET(webview), true);

    gtk_box_pack_start(priv->box_contents_area, GTK_WIDGET(webview), true, true, 0);
    gtk_widget_show(GTK_WIDGET(webview));

    webkit_web_view_load_uri(webview, uri);
}
