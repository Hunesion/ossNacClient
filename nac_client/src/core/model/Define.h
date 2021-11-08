#ifndef _HUNE_CORE_MODEL_DEFINE_H_
#define _HUNE_CORE_MODEL_DEFINE_H_
#include "../macro.h"
#include <string>

#define HUNE_APPLICATION_ETC_PATH   "/etc/ionenac" 


BEGIN_HUNE_CORE


namespace Define
{
class UserInfo {
    public:
    std::string UserId;
    std::string Password;

    UserInfo() {
        this->Clear();
    }

    void Clear() {
        UserId.clear();
        Password.clear();
    }
};


} 

END_HUNE_CORE

#endif /* _HUNE_CORE_MODEL_DEFINE_H_ */