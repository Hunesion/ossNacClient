#ifndef _HUNE_CORE_MODEL_STRING_RESOURCE_H_
#define _HUNE_CORE_MODEL_STRING_RESOURCE_H_
#include <string>
#include <gtk/gtk.h>
#include "../macro.h"

//  INI_GROUP
#define STRING_GROUP                "STRING"

BEGIN_HUNE_CORE
class StringResource
{
public:
    StringResource(); 
    virtual ~StringResource(); 

    void init(); 
    void dispose();

private:
    bool readStringResource();
    GKeyFile* loadStringResourceFile();

private:
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00001, _message_00001);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00002, _message_00002);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00003, _message_00003);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00004, _message_00004);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00005, _message_00005);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00006, _message_00006);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00007, _message_00007);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00008, _message_00008);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00009, _message_00009);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00010, _message_00010);
    HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, message_00011, _message_00011);
}; 
END_HUNE_CORE
#endif 