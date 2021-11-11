#include "core.h"
#include "IonexLogic.h"
#include "IonexShell.h"

#include <thread>
#include <sys/stat.h>

USING_HUNE_CORE;


namespace Ionex
{
    static std::string s_interface;

    static void LoginThread(std::string userid, std::string password);
    static void LogoutThread();
    static void DownloadScriptThread(std::string userid);

    static void CallResultEvent(const std::string &eventName, const std::string &result) {
        EventManager *eventMgr = EventManager::getInstance();
        std::string* presult = NULL;
        if (eventMgr == NULL) {
            return;
        }

        presult = presult = new std::string(result);
        
        eventMgr->dispatchEventAsync(eventName.c_str(), presult, [](void* release){
            std::string *param = (std::string*)release;
            if (param != NULL) {
                delete param;
                param = NULL;
            }
        });
    }

    void Init() {
        const ClientConfig &clientConfig = GlobalVar::getClientConfig();
        Shell::SetDirectory(clientConfig.getIonexDir());
        s_interface = Shell::getInterfaceName();
    }

    void Login(const std::string &userid, const std::string &password) {
        std::thread t1(LoginThread, userid, password);
        t1.detach();
    }

    static void LoginThread(std::string userid, std::string password) {
        std::string result;

        result = Shell::Start(s_interface, userid, password);

        if (result == IONEX_LOGIN_RESULT_OK) {
            result = IONEX_LOGIN_RESULT_TIMEOUT;

            for (int i = 0; i < 30; i++) {
                if (Shell::Status(s_interface) == IONEX_LOGIN_RESULT_COMPLETED) {
                    result = IONEX_LOGIN_RESULT_COMPLETED;
                    break;
                }

                //  로그인 시도 중
                CallResultEvent(HUNE_CORE_IONEX_LOGIN, IONEX_LOGIN_RESULT_ASSOCIATED);

                sleep(1);
            }
        }

        CallResultEvent(HUNE_CORE_IONEX_LOGIN, result);
    }

    void Logout() {
        std::thread t1(LogoutThread);
        t1.detach();
    }

    static void LogoutThread() {
        std::string result;

        result = Shell::Stop(s_interface);

        CallResultEvent(HUNE_CORE_IONEX_LOGOUT, result);
    }

    void DownloadAndExecuteScript(const std::string &userid) {
        std::thread t1(DownloadScriptThread, userid);
        t1.detach();
    }

    static void DownloadScriptThread(std::string userid) {
        const ClientConfig &clientConfig = GlobalVar::getClientConfig();
        std::string url, policyPath;
        StringUtils::format(url, "https://%s:%d/IM/IM_ACC_MGNT/iam_download_linux_policy.jsp?strId=%s", clientConfig.getServer().c_str(),
                        clientConfig.getPort(), userid.c_str());
        StringUtils::format(policyPath, "%s/policy.sh", clientConfig.getScriptDir().c_str());
        Shell::HttpDownload(url, policyPath);
        chmod(policyPath.c_str(), 0755);
        Shell::ExecuteScript(policyPath);
    }
}




