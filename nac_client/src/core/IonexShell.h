#ifndef _HUNE_CORE_IONEX_SHELL_H_
#define _HUNE_CORE_IONEX_SHELL_H_
#include "macro.h"
#include <string>

namespace Ionex { namespace Shell
{
    void SetDirectory(const std::string &dir);

    std::string Start(const std::string &interface, const std::string &userid, const std::string &password);

    std::string Stop(const std::string &interface);

    std::string Status(const std::string &interface);

    void HttpDownload(const std::string &url, const std::string &path);

    std::string ExecuteScript(const std::string &execute_command);

    std::string getInterfaceName();
}}


#endif