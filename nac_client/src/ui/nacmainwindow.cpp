#include "nacmainwindow.h"
#include "huneuicommon.h"
#include "nacwebviewwindow.h"
#include "../core/DirectoryMonitor.h"
#include "../core/ClientUpdater.h"
#include <string>

//////////  구현부  ////////

struct _NacMainWindow
{
    GtkApplicationWindow parent;
};

typedef struct _NacMainWindowPrivate NacMainWindowPrivate;
struct _NacMainWindowPrivate
{
    /* 소스에서 정의 */
    int download_policy_timer_id;
    Hune::Core::DirectoryMonitor *dir_monitor;
    bool is_login_complete;
    NacWebViewWindow *webview_window;

    /* template에서 정의 */
    GtkImage      *img_main_logo;
    GtkBox        *box_content;
    GtkLabel      *lbl_login;
    GtkLabel      *lbl_state;
    GtkLabel      *lbl_state_txt;
    GtkEntry      *etr_id;
    GtkEntry      *etr_password;
    GtkCheckButton *ckbtn_save_id;
    GtkButton     *btn_login;
};

G_DEFINE_TYPE_WITH_PRIVATE(NacMainWindow, nac_main_window, GTK_TYPE_APPLICATION_WINDOW);



////////////////////////////////
static NacMainWindow *s_win = NULL ; 

gpointer hune_ui_get_main_window()
{
    return s_win;
}

////////////////////////////////

//////////  FTC 함수  //////////
static void nac_main_window_login_button_logic(NacMainWindow *main_window);

static void nac_main_window_on_click_btn_login(GtkWidget *widget, gpointer user_data);

static void nac_main_window_on_key_release_event(GtkWidget *widget, GdkEventKey *keydata);

static void nac_main_window_do_login(NacMainWindow *main_window);

static void nac_main_window_login_result_event(void *target, Hune::Core::Event *event);

static void nac_main_window_do_logout(NacMainWindow *main_window);

static void nac_main_window_logout_result_event(void *target, Hune::Core::Event *event);

static void nac_main_window_logout_dispose(NacMainWindow *main_window);

static void nac_main_window_login_widget_sensitive(NacMainWindow *main_window, bool is_sensitive);

static void nac_main_window_login_complete(NacMainWindow *main_window);

static void nac_main_window_login_associated(NacMainWindow *main_window);

static void nac_main_window_download_policy_script(NacMainWindow *main_window);

static void nac_main_window_error_html_dir_monitoring_event(void *target, Hune::Core::Event *event);

static gboolean nac_main_window_download_and_execute_policy_timer_func(gpointer user_data);



static void 
nac_main_window_init (NacMainWindow *win)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::ClientConfig &client_config = Hune::Core::GlobalVar::getClientConfig();
    Hune::Core::EventManager *event_mgr = Hune::Core::EventManager::getInstance();
    Hune::Core::ClientUpdater client_updater;
    GdkPixbuf *icon = NULL;
    gtk_widget_init_template (GTK_WIDGET (win));
    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(win);

    s_win = win ; 
    priv->download_policy_timer_id = 0;
    priv->dir_monitor = NULL;
    priv->is_login_complete = false;

    //  Signal (Event) 함수 연결
    //  key release
    g_signal_connect(win, "key_release_event", G_CALLBACK(nac_main_window_on_key_release_event), win);

    //  btn_login click
    g_signal_connect(priv->btn_login, "clicked", G_CALLBACK(nac_main_window_on_click_btn_login), win);

    gtk_widget_set_size_request(GTK_WIDGET(win), 700, 460);    

    gtk_widget_set_size_request(GTK_WIDGET(priv->etr_id), -1 , 40);
    gtk_widget_set_size_request(GTK_WIDGET(priv->etr_password), -1 , 40);
    gtk_widget_set_size_request(GTK_WIDGET(priv->btn_login), -1 , 40);

    gtk_image_set_from_resource(priv->img_main_logo, "/com/hunesion/nacclient/main_logo.png");

    gtk_widget_set_name(GTK_WIDGET(win), "default_background_window");
    gtk_widget_set_name(GTK_WIDGET(priv->lbl_login), "label_login");
    gtk_widget_set_name(GTK_WIDGET(priv->lbl_state), "label_state");
    gtk_widget_set_name(GTK_WIDGET(priv->lbl_state_txt), "label_state_txt");
    gtk_widget_set_name(GTK_WIDGET(priv->etr_id), "entry_default");
    gtk_widget_set_name(GTK_WIDGET(priv->etr_password), "entry_default");
    gtk_widget_set_name(GTK_WIDGET(priv->btn_login), "button_login");
    gtk_widget_set_name(GTK_WIDGET(priv->ckbtn_save_id), "checkbox_id_save");

    if (event_mgr != NULL) {
        event_mgr->addEventListener(win, HUNE_CORE_IONEX_LOGIN, HUNE_CALLBACK_CLS_STATIC_2(nac_main_window_login_result_event));
        event_mgr->addEventListener(win, HUNE_CORE_IONEX_LOGOUT, HUNE_CALLBACK_CLS_STATIC_2(nac_main_window_logout_result_event));
        event_mgr->addEventListener(win, NAC_UI_MONITORING_ERROR_HTML, HUNE_CALLBACK_CLS_STATIC_2(nac_main_window_error_html_dir_monitoring_event));
    }

    //  아이콘
    icon = gdk_pixbuf_new_from_resource("/com/hunesion/nacclient/nacclient.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(win), icon);
    g_object_unref(icon);
    icon = NULL;

    Ionex::Init();

    if (client_config.getAutoUpdate()) {
        if (client_updater.isUpdate()) {
            client_updater.update();
            event_mgr->dispatchEvent(NAC_UI_APPLICATION_QUIT, hune_ui_get_application());
        }
    }
    
}

static void
nac_main_window_dispose (GObject *object)
{
    NacMainWindowPrivate *priv = NULL;
    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(NAC_MAIN_WINDOW(object));
    if (! priv) {
        return;
    }

    if (s_win) {
        if (priv->is_login_complete) {
            nac_main_window_do_logout(s_win);
        }

        s_win = NULL ; 
    }

    Hune::Core::EventManager::getInstance()->removeEventListener(object);

    G_OBJECT_CLASS(nac_main_window_parent_class)->dispose(object);
}

static void
nac_main_window_finalize(GObject *object)
{
    G_OBJECT_CLASS(nac_main_window_parent_class)->finalize(object);
}

static void 
nac_main_window_class_init (NacMainWindowClass *clz)
{
    G_OBJECT_CLASS (clz)->finalize = nac_main_window_finalize;
    G_OBJECT_CLASS (clz)->dispose = nac_main_window_dispose;

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS(clz), "/com/hunesion/nacclient/nac-main-window.ui");
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, box_content);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, img_main_logo);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, lbl_login);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, lbl_state);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, lbl_state_txt);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, etr_id);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, etr_password);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, ckbtn_save_id);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(clz), NacMainWindow, btn_login);
}

NacMainWindow*
nac_main_window_new (NacApplication *app)
{
    return (NacMainWindow *)g_object_new (NAC_TYPE_MAIN_WINDOW, "application", app, NULL);
}

void 
nac_main_window_open (NacMainWindow *main_window, GFile *file)
{
    NacMainWindowPrivate *priv = NULL;
    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }
    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

}

static void
nac_main_window_login_button_logic(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::StringResource& string_resource = Hune::Core::GlobalVar::getStringResource();
    std::string userid, password, button_label;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    button_label = gtk_button_get_label(priv->btn_login);
    if (button_label == string_resource.get_message_00010()) {
        //  로그인 로직
        nac_main_window_do_login(main_window);
    } else {
        //  로그아웃
        nac_main_window_do_logout(main_window);
    }
}

static void 
nac_main_window_on_click_btn_login(GtkWidget *widget, gpointer user_data)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;

    if (! NAC_IS_MAIN_WINDOW(user_data)) {
        return;
    }

    main_window = NAC_MAIN_WINDOW(user_data);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    printf("nac_main_window_on_click_btn_login \n");

    nac_main_window_login_button_logic(main_window);
}

static void 
nac_main_window_on_key_release_event(GtkWidget *widget, GdkEventKey *keydata)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;

    if (! NAC_IS_MAIN_WINDOW(widget)) {
        return;
    }
    main_window = NAC_MAIN_WINDOW(widget);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (keydata->keyval == GDK_KEY_Return) {
        nac_main_window_login_button_logic(main_window);
    }
}

static void 
nac_main_window_do_login(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::StringResource& string_resource = Hune::Core::GlobalVar::getStringResource();
    std::string userid, password, button_label;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    userid = gtk_entry_get_text(priv->etr_id);
    password = gtk_entry_get_text(priv->etr_password);

    Hune::Core::StringUtils::trim(userid);
    Hune::Core::StringUtils::trim(password);

    if (userid.empty() == true) {
        //  아이디를 입력하세요.
        hune_ui_message_box(GtkMessageType::GTK_MESSAGE_INFO, GtkButtonsType::GTK_BUTTONS_OK, string_resource.get_message_00001());
        return;
    } else if (password.empty() == true) {
        //  비밀번호를 입력하세요.
        hune_ui_message_box(GtkMessageType::GTK_MESSAGE_INFO, GtkButtonsType::GTK_BUTTONS_OK, string_resource.get_message_00002());
        return;
    }

    nac_main_window_login_widget_sensitive(main_window, false);
    hune_ui_change_cursor("wait");

    Ionex::Login(userid, password);
}

static void
nac_main_window_login_result_event(void *target, Hune::Core::Event *event)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;
    std::string *presult = NULL;

    if (! NAC_IS_MAIN_WINDOW(target)) {
        return;
    }

    main_window = NAC_MAIN_WINDOW(target);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (! event->getUserData()) {
        return;
    }

    presult = (std::string*)event->getUserData();

    if (*presult == IONEX_LOGIN_RESULT_ASSOCIATED) {
        nac_main_window_login_associated(main_window);
        return;
    }

    hune_ui_change_cursor("default");

    if (*presult == IONEX_LOGIN_RESULT_COMPLETED) {
        //  로그인 성공
        nac_main_window_login_complete(main_window);
        return;
    } else if (*presult == IONEX_LOGIN_RESULT_CANNOT_FIND_INTERFACE) {

    } else if (*presult == IONEX_LOGIN_RESULT_FAIL) {
        
    } else {
        //  이 외의 메세지
    }

    hune_ui_message_box(GtkMessageType::GTK_MESSAGE_INFO, GtkButtonsType::GTK_BUTTONS_OK, *presult);
    nac_main_window_login_widget_sensitive(main_window, true);
}

static void
nac_main_window_do_logout(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    
    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    nac_main_window_login_widget_sensitive(main_window, false);
    hune_ui_change_cursor("wait");

    Ionex::Logout();
}

static void
nac_main_window_logout_result_event(void *target, Hune::Core::Event *event)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::StringResource& string_resource = Hune::Core::GlobalVar::getStringResource();
    std::string *presult = NULL;
    

    if (! NAC_IS_MAIN_WINDOW(target)) {
        return;
    }

    main_window = NAC_MAIN_WINDOW(target);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (! event->getUserData()) {
        return;
    }

    presult = (std::string*)event->getUserData();

    nac_main_window_login_widget_sensitive(main_window, true);
    hune_ui_change_cursor("default");
    gtk_button_set_label(priv->btn_login, string_resource.get_message_00010().c_str());
    gtk_label_set_text(priv->lbl_login, string_resource.get_message_00009().c_str());
    gtk_label_set_text(priv->lbl_state_txt, string_resource.get_message_00004().c_str());

    nac_main_window_logout_dispose(main_window);

    priv->is_login_complete = false;
}

static void
nac_main_window_logout_dispose(NacMainWindow *main_window) 
{
    NacMainWindowPrivate *priv = NULL;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (priv->dir_monitor) {
        priv->dir_monitor->Stop();
        delete priv->dir_monitor;
        priv->dir_monitor = NULL;
    }

    if (priv->download_policy_timer_id > 0) {
        g_source_remove(priv->download_policy_timer_id);
        priv->download_policy_timer_id = 0;
    }

    if (priv->webview_window) {
        gtk_widget_destroy(GTK_WIDGET(priv->webview_window));
        priv->webview_window = NULL;
    }
}

static void
nac_main_window_login_widget_sensitive(NacMainWindow *main_window, bool is_sensitive)
{
    NacMainWindowPrivate *priv = NULL;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (is_sensitive == true) {
        gtk_widget_set_sensitive(GTK_WIDGET(priv->btn_login), true);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->etr_id), true);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->etr_password), true);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->ckbtn_save_id), true);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(priv->btn_login), false);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->etr_id), false);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->etr_password), false);
        gtk_widget_set_sensitive(GTK_WIDGET(priv->ckbtn_save_id), false);
    }

}

static void 
nac_main_window_login_complete(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::ClientConfig &client_config = Hune::Core::GlobalVar::getClientConfig();
    const Hune::Core::StringResource &string_resource = Hune::Core::GlobalVar::getStringResource();
    const char *userid = NULL, *passwd = NULL;
    Hune::Core::Define::UserInfo user_info;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    gtk_label_set_text(priv->lbl_state_txt, string_resource.get_message_00006().c_str());
    gtk_label_set_text(priv->lbl_login, string_resource.get_message_00008().c_str());
    gtk_button_set_label(priv->btn_login, string_resource.get_message_00011().c_str());
    gtk_widget_set_sensitive(GTK_WIDGET(priv->btn_login), true);

    userid = gtk_entry_get_text(priv->etr_id);
    passwd = gtk_entry_get_text(priv->etr_password);

    user_info.UserId = userid;
    user_info.Password = passwd;
    Hune::Core::GlobalVar::setUserInfo(user_info);

    priv->dir_monitor = new Hune::Core::DirectoryMonitor(client_config.getErrorHtmlDir(), NAC_UI_MONITORING_ERROR_HTML, main_window);
    priv->dir_monitor->Start(IN_CLOSE_WRITE);

    //  최소 한번 실행 후 타이머에서 실행한다.
    nac_main_window_download_policy_script(main_window);
    priv->download_policy_timer_id = g_timeout_add(60000, nac_main_window_download_and_execute_policy_timer_func, main_window);

    priv->is_login_complete = true;
}

static void 
nac_main_window_login_associated(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::StringResource &string_resource = Hune::Core::GlobalVar::getStringResource();
    std::string label_message, message;

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    message = string_resource.get_message_00005().c_str();
    label_message = gtk_label_get_text(priv->lbl_state_txt);
    if (label_message.find(message) != std::string::npos) {
        int dot_len = 0;

        Hune::Core::StringUtils::replace(label_message, message, "");
        dot_len = label_message.length();

        if (dot_len == 0) {
            message.append(".");
        } else if (dot_len == 1) {
            message.append("..");
        }
    }

    gtk_label_set_text(priv->lbl_state_txt, message.c_str());
}

static void
nac_main_window_download_policy_script(NacMainWindow *main_window)
{
    NacMainWindowPrivate *priv = NULL;
    const Hune::Core::Define::UserInfo &user_info = Hune::Core::GlobalVar::getUserInfo();

    if (! NAC_IS_MAIN_WINDOW(main_window)) {
        return;
    }

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    Ionex::DownloadAndExecuteScript(user_info.UserId);
}

static void 
nac_main_window_error_html_dir_monitoring_event(void *target, Hune::Core::Event *event)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;
    Hune::Core::DmParam *param = NULL;
    std::string uri;
 
    if (! NAC_IS_MAIN_WINDOW(target)) {
        return;
    }

    main_window = NAC_MAIN_WINDOW(target);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return;
    }

    if (! event->getUserData()) {
        return;
    }
    param = (Hune::Core::DmParam*)event->getUserData();

    g_print("moniter : %s \n", param->filename.c_str());

    if (priv->webview_window) {
        gtk_widget_destroy(GTK_WIDGET(priv->webview_window));

    }
    priv->webview_window = nac_web_view_window_new(GTK_WINDOW(main_window));
    if (! priv->webview_window) {
        return;
    }

    Hune::Core::StringUtils::format(uri, "file://%s/%s", param->dir.c_str(), param->filename.c_str());
    nac_web_view_window_show(priv->webview_window, uri.c_str());}

static gboolean
nac_main_window_download_and_execute_policy_timer_func(gpointer user_data)
{
    NacMainWindow *main_window = NULL;
    NacMainWindowPrivate *priv = NULL;
    bool rv = false;

    if (! NAC_IS_MAIN_WINDOW(user_data)) {
        return rv;
    }
    main_window = NAC_MAIN_WINDOW(user_data);

    priv = (NacMainWindowPrivate*)nac_main_window_get_instance_private(main_window);
    if (! priv) {
        return rv;
    }

    nac_main_window_download_policy_script(main_window);

    rv = true;
    return rv;
}