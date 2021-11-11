#include "IonexShell.h"
#include "StringUtils.h"
#include "sys_utils.h"
#include "model/Define.h"

#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <chrono>

USING_HUNE_CORE;


namespace Ionex { namespace Shell
{
    static std::string s_ionexDir;

    void SetDirectory(const std::string &dir) {
        s_ionexDir = dir;
    }

    static std::string ExecuteCommand(const std::string &parameter) {
        std::string output = "";
        char buf[1024] = { 0, };
        int fd[2] = { 0, };
        int pid = -1;

        if (pipe(fd) < 0) {
            goto END;
        }

        pid = hune_command(parameter, fd[1]);
        if (fd[1] > 0) {
            close(fd[1]);
        }

        if (pid < 0) {
            goto END;
        }

        while (read(fd[0], buf, 1024) > 0) {
            output += buf;
        }

        StringUtils::trim(output);

    END:
        if (fd[0] > 0) {
            close(fd[0]);
        }
        
        return output;
    }

    std::string Start(const std::string &interface, const std::string &userid, const std::string &password) {
        std::string parameter;

#ifdef HUNE_DEBUG
        StringUtils::format(parameter, "%s/ionexcli -i%s interface_add %s %s/ionexd.conf wired '%s' '%s'",
                            s_ionexDir.c_str(), interface.c_str(), interface.c_str(), s_ionexDir.c_str(), userid.c_str(), password.c_str());
#else
        StringUtils::format(parameter, "%s/ionexcli -i%s interface_add %s %s/ionexd.conf wired '%s' '%s'",
                            s_ionexDir.c_str(), interface.c_str(), interface.c_str(), HUNE_APPLICATION_ETC_PATH, userid.c_str(), password.c_str());
#endif

        return ExecuteCommand(parameter);
    }

    std::string Stop(const std::string &interface) {
        std::string parameter;

        StringUtils::format(parameter, "%s/ionexcli interface_remove %s",
                            s_ionexDir.c_str(), interface.c_str());

        return ExecuteCommand(parameter);
    }

    
    std::string Status(const std::string &interface) {
        std::string parameter, output, result;
        std::list<std::string> splitList;

        StringUtils::format(parameter, "%s/ionexcli -i%s status",
                            s_ionexDir.c_str(), interface.c_str());

        output = ExecuteCommand(parameter);
        auto keyValue = StringUtils::getKeyValue(output);

        auto finder = keyValue.find("wpa_state");
        if (finder == keyValue.end()) {
            return "FAIL";
        }

        return finder->second;
    }

    void HttpDownload(const std::string &url, const std::string &path) {
        std::string parameter;

        StringUtils::format(parameter, "curl -k -o %s %s", path.c_str(), url.c_str());
        ExecuteCommand(parameter);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        StringUtils::format(parameter, "dos2unix %s", path.c_str());
        ExecuteCommand(parameter);
    }

    std::string ExecuteScript(const std::string &execute_command) {
        std::string parameter;

        StringUtils::format(parameter, "%s/ionexcli execute_script %s", s_ionexDir.c_str(), execute_command.c_str());
        printf("ExecuteScript: %s\n", parameter.c_str());

        return ExecuteCommand(parameter);
    }

    std::string getInterfaceName() {
        std::string parameter;

        parameter = "ip link show |grep UP |grep -v lo |head -1 |awk '{print $2}' |awk -F: '{print $1}'";
        return ExecuteCommand(parameter);
    }
}}
