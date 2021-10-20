#include <map>
#include <string>
#include "INIManager.h"
#include "core.h"


BEGIN_HUNE_CORE

INIManager::INIManager() {
    this->_keyfile = nullptr;
}

INIManager::~INIManager() {

}

bool INIManager::load(const char *iniPath) {
    GKeyFile *keyfile = nullptr;
    GError *error = nullptr;
    bool rv = false;

    keyfile = g_key_file_new();
    if (! keyfile) {
        return rv;
    }

    rv = g_key_file_load_from_file(keyfile, iniPath, GKeyFileFlags::G_KEY_FILE_KEEP_COMMENTS, &error);
    if (rv) {
        this->_keyfile = keyfile;
    } else {
        g_key_file_free(keyfile);
        keyfile = nullptr;
    }

    return rv;
}

bool INIManager::load(const char *data, size_t length) {
    GKeyFile *keyfile = nullptr;
    GError *error = nullptr;
    bool rv = false;

    keyfile = g_key_file_new();
    if (! keyfile) {
        return rv;
    }

    rv = g_key_file_load_from_data(keyfile, data, length, GKeyFileFlags::G_KEY_FILE_KEEP_COMMENTS, &error);
    if (rv) {
        this->_keyfile = keyfile;
    } else {
        g_key_file_free(keyfile);
        keyfile = nullptr;
    }

    return rv;
}
    
void INIManager::dispose() {
    if (this->_keyfile) {
        g_key_file_free(this->_keyfile);
        this->_keyfile = nullptr;
    }
}

bool INIManager::save(const char *iniPath) {
    bool rv = false;
    GError *error = nullptr;

    if (! this->_keyfile || ! iniPath) {
        return rv;
    }

    rv = g_key_file_save_to_file(this->_keyfile, iniPath, &error); 
    if (error) {
        g_print("INIManager save error : %s \n", error->message);
        g_error_free(error);
        error = nullptr;
    }

    return rv;
}

std::string INIManager::getString(const char *group, const char *key, const char *def) {
    std::string rv = "";
    char *value = nullptr;
    GError *error = nullptr;

    if (def) {
        rv = def;
    }

    if (! this->_keyfile || ! group || ! key) {
        return rv;
    }

    value = g_key_file_get_string(this->_keyfile, group, key, &error);
    if (value) {
        rv = value;

        g_free(value);
        value = nullptr;
    } else if (error) {
        g_print("getString error : %s \n", error->message);
        g_error_free(error);
        error = nullptr;
    }

    return rv;
}
    
int INIManager::getInt(const char *group, const char *key, int def) {
    int rv = def;
    GError *error = nullptr;

    if (! this->_keyfile || ! group || ! key) {
        return rv;
    }

    rv = g_key_file_get_integer(this->_keyfile, group, key, &error);

    if (error) {
        rv = def;
        g_print("getInt error : %s \n", error->message);
        g_error_free(error);
        error = nullptr;
    }

    return rv;
}
    
bool INIManager::getBoolean(const char *group, const char *key, bool def) {
    bool rv = def;
    GError *error = nullptr;

    if (! this->_keyfile || ! group || ! key) {
        return rv;
    }

    rv = g_key_file_get_boolean(this->_keyfile, group, key, &error);
    
    if (error) {
        rv = def;
        g_print("getBoolean error : %s \n", error->message);
        g_error_free(error);
        error = nullptr;
    }

    return rv;
}

std::map<std::string, std::string> INIManager::getGroupData(const char *group) {
    std::map<std::string, std::string> rv;
    gsize len = 0;
    GError *error = nullptr;
    gchar **keys = nullptr;

    if (! this->_keyfile || ! group) {
        return rv;
    }

    keys = g_key_file_get_keys(this->_keyfile, group, &len, &error);
    for (int i = 0; i < len; i++) {
        const char *key = keys[i];
        std::string value;

        value = this->getString(group, key);

        std::pair<std::string, std::string> item(key, value);
        rv.insert(item);
    }

    if (keys) {
        g_strfreev(keys);
        keys = nullptr;
    }

    return rv;
}

void INIManager::setString(const char *group, const char *key, const char *value) {
    if (! this->_keyfile || ! group || ! key || ! value) {
        return;
    }

    g_key_file_set_string(this->_keyfile, group, key, value);
}

void INIManager::setString(const char *group, const char *key, const std::string &value) {
    this->setString(group, key, value.c_str());
}

void INIManager::setInt(const char *group, const char *key, int value) {
    if (! this->_keyfile || ! group || ! key) {
        return;
    }

    g_key_file_set_integer(this->_keyfile, group, key, value);
}

void INIManager::setInt(const char *group, const char *key, bool value) {
    if (! this->_keyfile || ! group || ! key) {
        return;
    }

    if (value) {
        g_key_file_set_integer(this->_keyfile, group, key, 1);
    } else {
        g_key_file_set_integer(this->_keyfile, group, key, 0);
    }
}

void INIManager::setBoolean(const char *group, const char *key, bool value) {
    if (! this->_keyfile || ! group || ! key) {
        return;
    }

    g_key_file_set_boolean(this->_keyfile, group, key, value);
}

void INIManager::setGroupData(const char *group, const std::map<std::string, std::string> &groupData) {
    if (! this->_keyfile || ! group) {
        return;
    }

    for (auto iter = groupData.begin(); iter != groupData.end(); iter++) {
        g_key_file_set_string(this->_keyfile, group, iter->first.c_str(), iter->second.c_str());
    }
}


END_HUNE_CORE
