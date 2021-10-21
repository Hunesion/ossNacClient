#ifndef _HUNE_CORE_MODEL_GLOBAL_VAR_H_
#define _HUNE_CORE_MODEL_GLOBAL_VAR_H_
#include <string>

#include "Define.h"
#include "ClientConfig.h"
#include "StringResource.h"


BEGIN_HUNE_CORE
namespace GlobalVar
{
    void init(); 
    void dispose();

    const StringResource& getStringResource(); 
    const ClientConfig& getClientConfig();

    const Define::UserInfo& getUserInfo();
    void setUserInfo(const Define::UserInfo &userInfo);


} // namespace GlobalVar

END_HUNE_CORE

#endif /* _HUNE_CORE_MODEL_GLOBAL_VAR_H_ */