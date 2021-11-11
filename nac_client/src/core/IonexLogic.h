#ifndef _HUNE_CORE_IONEX_LOGIC_H_
#define _HUNE_CORE_IONEX_LOGIC_H_
#include "macro.h"
#include <string>

#define IONEX_LOGIN_RESULT_OK                       "OK"
#define IONEX_LOGIN_RESULT_FAIL                     "FAIL"
#define IONEX_LOGIN_RESULT_COMPLETED                "COMPLETED"
#define IONEX_LOGIN_RESULT_TIMEOUT                  "TIMEOUT"
#define IONEX_LOGIN_RESULT_ASSOCIATED               "ASSOCIATED"
#define IONEX_LOGIN_RESULT_CANNOT_FIND_INTERFACE    "Cannot find interface"



namespace Ionex 
{
    void Init();

    void Login(const std::string &userid, const std::string &password);

    void Logout();

    void DownloadAndExecuteScript(const std::string &userid);
}


#endif