#include "GlobalVar.h"
#include <string>

BEGIN_HUNE_CORE

static ClientConfig s_ClientConfig ; 
static StringResource s_StringResource ;
static Define::UserInfo s_UserInfo;


namespace GlobalVar
{
    void init(){
        s_ClientConfig.init(); 
        s_StringResource.init();
    }
    
    void dispose() {
        s_ClientConfig.dispose();
        s_StringResource.dispose();
    }

    const ClientConfig &getClientConfig(){
        return s_ClientConfig; 
    }

    const StringResource &getStringResource(){
        return s_StringResource;
    }

    const Define::UserInfo& getUserInfo() {
        return s_UserInfo;
    }

    void setUserInfo(const Define::UserInfo &userInfo) {
        s_UserInfo = userInfo;
    }

} // namespace GlobalVar
END_HUNE_CORE
