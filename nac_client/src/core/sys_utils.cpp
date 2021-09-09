#include "core.h"
#include "sys_utils.h"

#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <gio/gio.h>
#include <sys/wait.h>


static pthread_t s_main_thread = 0;

BEGIN_HUNE_CORE

int getPID()
{
    return getpid();
}

unsigned long int getThreadId()
{
    return pthread_self();
}

void hune_system_init()
{
    s_main_thread = pthread_self();
}
bool hune_is_main_thread()
{
    return s_main_thread == pthread_self();
}

int hune_command(const std::string &param, int wfd)
{
    int pid = -1;

    pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid == 0) {
        char *argv[4] = { 0, };
        char arg0[] = "sh";
        char arg1[] = "-c";
        char *arg2 = NULL;
        
        arg2 = new(std::nothrow) char[param.length() + 1];
        if (arg2 == nullptr) {
            return -1;
        }
        memset(arg2, 0, param.length() + 1);
        memcpy(arg2, param.c_str(), param.length());

        argv[0] = arg0;
        argv[1] = arg1;
        argv[2] = arg2;
        argv[3] = NULL;

        if (wfd > 0) {
            dup2(wfd, STDOUT_FILENO);
        }
        
        execv("/bin/sh", argv);
        
        if (arg2) {
            delete[] arg2;
            arg2 = NULL;
        }
        exit(127);
    }
    
    return pid;
}

int hune_xdg_open(const std::string &uri)
{
    std::string param = "xdg-open " + uri;
    return hune_command(param);
}

int hune_open_default_uri(const std::string &param)
{
    GError *error  = NULL;

    if (g_app_info_launch_default_for_uri(param.c_str(), NULL, &error)) {
        return 0;
    }

    g_error_free(error);

    return 1;
}

int hune_open_url(const std::string &url)
{
    std::string param = url;
    if ((param.find("http://") == std::string::npos) && (param.find("https://") == std::string::npos)) {
        param = param.insert(0, "http://");
    }

    return hune_xdg_open(param);
}

int hune_open_file(const std::string &file)
{
    std::string param = file;
    if (param.find("file://") == std::string::npos) {
        param = param.insert(0, "file://");
    }

    return hune_xdg_open(param);
}


END_HUNE_CORE