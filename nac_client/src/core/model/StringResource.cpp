#include "StringResource.h"
#include "Define.h"


#define HUNE_READ_STRING_RESOURCE(varName, stringName)\
    value = NULL; \
    value = g_key_file_get_string(keyfile, STRING_GROUP, stringName, NULL); \
    if (value) { \
        this->varName = value; \
        g_free(value); \
        value = NULL; \
    } 

BEGIN_HUNE_CORE

StringResource::StringResource(){
    
}

StringResource::~StringResource(){
    
}

void StringResource::init(){
    this->readStringResource();
}

void StringResource::dispose() {
}


GKeyFile* StringResource::loadStringResourceFile() {
    GKeyFile *rv = nullptr;
    GError *error = nullptr;
    std::string path;
    bool result = true;
    
    rv = g_key_file_new();
    if (! rv) {
        return rv;
    }

#ifdef HUNE_DEBUG
    path = "../../string_resource.ini";
#else
    path = HUNE_APPLICATION_ETC_PATH;
    path += "/string_resource.ini";
#endif

    result = g_key_file_load_from_file(rv, path.c_str(), GKeyFileFlags::G_KEY_FILE_KEEP_COMMENTS, &error);
    
    if (! result) {
        g_key_file_free(rv);
        rv = nullptr;

        if (error) {
            //FTC_LOG("g_key_file_load_from_data fail : %s", error->message);
            g_error_free(error);
            error = nullptr;
        }
    }
    
    return rv;
}

bool StringResource::readStringResource() {
    GKeyFile *keyfile = nullptr;
    char *value = nullptr;
    bool rv = false;

    keyfile = this->loadStringResourceFile();

    if (! keyfile) {
        return rv;
    }

    HUNE_READ_STRING_RESOURCE(message_00001, "message_00001");
    HUNE_READ_STRING_RESOURCE(message_00002, "message_00002");
    HUNE_READ_STRING_RESOURCE(message_00003, "message_00003");
    HUNE_READ_STRING_RESOURCE(message_00004, "message_00004");
    HUNE_READ_STRING_RESOURCE(message_00005, "message_00005");
    HUNE_READ_STRING_RESOURCE(message_00006, "message_00006");
    HUNE_READ_STRING_RESOURCE(message_00007, "message_00007");
    HUNE_READ_STRING_RESOURCE(message_00008, "message_00008");
    HUNE_READ_STRING_RESOURCE(message_00009, "message_00009");
    HUNE_READ_STRING_RESOURCE(message_00010, "message_00010");
    HUNE_READ_STRING_RESOURCE(message_00011, "message_00011");

END:
    if (keyfile) {
        g_key_file_free(keyfile);
        keyfile = nullptr;
    }

    if (value) {
        g_free(value);
        value = nullptr;
    }

    return rv;
}
END_HUNE_CORE
