#ifndef _HUNE_CORE_INI_MANAGER_H_
#define _HUNE_CORE_INI_MANAGER_H_
#include <string>
#include <map>
#include <gtk/gtk.h>
#include "macro.h"



BEGIN_HUNE_CORE
class INIManager
{
public:
    INIManager(); 
    virtual ~INIManager(); 

    bool load(const char *iniPath);
    bool load(const char *data, size_t length);
    void dispose();

    bool save(const char *iniPath);

    std::string getString(const char *group, const char *key, const char *def = nullptr);
    int getInt(const char *group, const char *key, int def = 0);
    bool getBoolean(const char *group, const char *key, bool def = false);
    std::map<std::string, std::string> getGroupData(const char *group);

    void setString(const char *group, const char *key, const char *value);
    void setString(const char *group, const char *key, const std::string &value);
    void setInt(const char *group, const char *key, int value);
    void setInt(const char *group, const char *key, bool value);
    void setBoolean(const char *group, const char *key, bool value);
    void setGroupData(const char *group, const std::map<std::string, std::string> &groupData);

private:
    std::string getEncString(const char *plainStr);
    std::string getDecString(const char *encStr);

private:
    GKeyFile *_keyfile;
};

END_HUNE_CORE
#endif /* _HUNE_CORE_INI_MANAGER_H_ */


