#include "nacapplication.h"
#include "nacmainwindow.h"
#include "huneuicommon.h"



struct _NacApplication
{
  GtkApplication parent;
};

G_DEFINE_TYPE(NacApplication, nac_application, GTK_TYPE_APPLICATION);

static NacApplication *s_app = NULL ;


gpointer 
nac_get_application()
{
  return s_app;
}



static void nac_application_init_config(NacApplication *app);

static void nac_application_quit(NacApplication *app);

static void nac_application_quit_event(void *target, Hune::Core::Event *evt);



static void
nac_application_init (NacApplication *app)
{
  Hune::Core::EventManager::getInstance()->addEventListener(app, NAC_UI_APPLICATION_QUIT, HUNE_CALLBACK_CLS_STATIC_2(nac_application_quit_event));
}

static void
nac_application_activate (GApplication *app)
{
  GList *windows = NULL;
  NacMainWindow *win = NULL;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows) {
    win = NAC_MAIN_WINDOW (windows->data);
  } else {
    nac_application_init_config(NAC_APPLICATION(app));
    win = nac_main_window_new (NAC_APPLICATION (app));
  }
  
  gtk_window_present (GTK_WINDOW (win));
}

static void
nac_application_open (GApplication  *app,
                  GFile        **files,
                  gint           n_files,
                  const gchar   *hint)
{
  GList *windows = NULL;
  NacMainWindow *win = NULL;
  int i = 0;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows){
    win = NAC_MAIN_WINDOW (windows->data);
  } else {
    nac_application_init_config(NAC_APPLICATION(app));
    win = nac_main_window_new (NAC_APPLICATION (app));
  }

  for (i = 0; i < n_files; i++){
    nac_main_window_open (win, files[i]);
  }

  gtk_window_present (GTK_WINDOW (win));
}

static void nac_applcation_shutdown(GApplication *app){
  Hune::Core::EventManager::getInstance()->removeEventListener(app);
  Hune::Core::hune_core_dispose();
  hune_event_callback_dispose();

  G_APPLICATION_CLASS(nac_application_parent_class)->shutdown(app);
}

static void
nac_application_class_init (NacApplicationClass *clz)
{
  G_APPLICATION_CLASS (clz)->activate = nac_application_activate;
  G_APPLICATION_CLASS (clz)->open = nac_application_open;
  G_APPLICATION_CLASS (clz)->shutdown = nac_applcation_shutdown; 
}

NacApplication *
nac_application_new (void)
{
  s_app = (NacApplication *)g_object_new (FTC_TYPE_APPLICATION,
                       "application-id", "com.hunesion.nacclient",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
  return s_app;
}

static void 
nac_application_init_config(NacApplication *app)
{
  GtkCssProvider *provider = NULL;
  GtkBuilder *builder = NULL;
  GMenuModel *app_tray_menu = NULL;
  const gchar *quit_accesls[2] = { "<Ctrl>Q", NULL };
  
  Hune::Core::hune_core_init();
  hune_event_callback_init();
  // ftc_dbus_server_init(); 
  
  //g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);

  gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.quit", quit_accesls);

  //  CSS 파일 전역으로 로딩
  //
  provider = gtk_css_provider_new();
  gtk_css_provider_load_from_resource(provider, "/com/hunesion/nacclient/style.css"); 
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);  

  if (builder) {
    g_object_unref(builder);
    builder = NULL;
  }
  
  if (provider) {
    g_object_unref(provider);
    provider = NULL;
  }
}

static void
nac_application_quit(NacApplication *app)
{
  if (! NAC_IS_APPLICATION(app)) {
    return;
  }

  g_application_quit(G_APPLICATION(app));
}

static void
nac_application_quit_event(void *target, Hune::Core::Event *evt)
{
  NacApplication *app = NULL;

  if (!evt) {
    return;
  }

  if (!NAC_IS_APPLICATION(evt->getUserData())) {
    return;
  }

  app = NAC_APPLICATION(evt->getUserData());

  nac_application_quit(app);
}

