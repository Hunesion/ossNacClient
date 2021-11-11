#ifndef _HUNE_CORE_CLIENT_CONFIG_H_
#define _HUNE_CORE_CLIENT_CONFIG_H_
#include <string>
#include <map>
#include <gtk/gtk.h>
#include "../macro.h"
#include "../INIManager.h"
#include "../StringUtils.h"



BEGIN_HUNE_CORE

class ClientConfig
{
public:
    ClientConfig(); 
    virtual ~ClientConfig(); 

    void init();
    void init(const char *iniPath);
    void init(const char *data, size_t length);
    void dispose();

    void readConfigini();
    bool writeConfigini();
  
    std::string getUserConfigDirPath() const;
    std::string getUserHomeDirPath() const;   
    std::string getIniPath(bool isHomeDir) const;
    bool existUserConfigDirIfNotCreate() const;
    std::string getUserDownloadDirPath() const;
private:
    

private:
    INIManager _iniMgr;

    /*Global Schema Value*/
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string , _server, Server); 
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(int , _port, Port); 
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string , _scriptDir, ScriptDir);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string , _errorHtmlDir, ErrorHtmlDir);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string , _ionexDir, IonexDir);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(bool , _autoUpdate, AutoUpdate); 

}; 
END_HUNE_CORE
#endif /* _HUNE_CORE_CLIENT_CONFIG_H_ */


