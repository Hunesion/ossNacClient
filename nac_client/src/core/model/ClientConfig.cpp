#include <map>
#include <string>
#include <pwd.h>
#include <sstream>
#include "ClientConfig.h"
#include "Define.h"
#include "../core.h"

#define NAC_CLIENT_INI_NAME    "client.ini"


//  Main Key
#define NAC_CONFIG_KEY_MAIN                     "Main"


#define NAC_CONFIG_SERVER                       "Server"
#define NAC_CONFIG_PORT                         "Port"
#define NAC_CONFIG_SCRIPT_DIR                   "ScriptDir"
#define NAC_CONFIG_ERROR_HTML_DIR               "ErrorHtmlDir"
#define NAC_CONFIG_IONEX_DIR                    "IonexDir"
#define NAC_CONFIG_AUTO_UPDATE                  "AutoUpdate"



BEGIN_HUNE_CORE

ClientConfig::ClientConfig(){

}

ClientConfig::~ClientConfig(){
    
}

void ClientConfig::init() {
    std::string iniPathStr = this->getIniPath(false);
    g_print("ini path : %s \n", iniPathStr.c_str());
    this->init(iniPathStr.c_str());
}

void ClientConfig::init(const char *iniPath) {
    this->_iniMgr.load(iniPath);

    this->readConfigini();
}

void ClientConfig::init(const char *data, size_t length) {
    this->_iniMgr.load(data, length);

    this->readConfigini();
}

void ClientConfig::dispose() {
    this->_iniMgr.dispose();
    //this->writeUserConfig();
}

void ClientConfig::readConfigini() {  
    
    this->_server = this->_iniMgr.getString(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_SERVER);
    this->_port = this->_iniMgr.getInt(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_PORT);
    this->_scriptDir = this->_iniMgr.getString(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_SCRIPT_DIR);
    if (this->_scriptDir.length() > 0) {
        FileUtils::createDirIfnotExist(this->_scriptDir.c_str());
    }
    this->_errorHtmlDir = this->_iniMgr.getString(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_ERROR_HTML_DIR);
    if (this->_errorHtmlDir.length() > 0) {
        FileUtils::createDirIfnotExist(this->_errorHtmlDir.c_str());
    }
    this->_ionexDir = this->_iniMgr.getString(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_IONEX_DIR);
    this->_autoUpdate = this->_iniMgr.getInt(NAC_CONFIG_KEY_MAIN, NAC_CONFIG_AUTO_UPDATE);
}

bool ClientConfig::writeConfigini() {
    std::string iniUserPath = this->getIniPath(true);

    
    //this->_iniMgr.setString(JTAC_CONFIG_KEY_SOCKS_CHAIN, JTAC_CONFIG_VDI_URL, this->_vdiURL);

    this->existUserConfigDirIfNotCreate();
    this->_iniMgr.save(iniUserPath.c_str());
}

std::string ClientConfig::getUserConfigDirPath() const {
    return this->getUserHomeDirPath() + "/.jtac";
}

std::string ClientConfig::getUserHomeDirPath() const {
    std::string rv;
    passwd *pwd = nullptr;
    const char *homeDirEnv = nullptr;
    GFile *homeDir = nullptr;

    homeDirEnv = getenv("HOME");
    if (! homeDirEnv) {
        pwd = getpwuid(getuid());
        if (! pwd) {
            goto END;
        }

        homeDirEnv = pwd->pw_dir;
    }

    homeDir = g_file_new_for_path(homeDirEnv);
    if (! homeDir) {
        goto END;
    }

    if (! g_file_query_exists(homeDir, nullptr)) {
        goto END;
    }
    
    rv = homeDirEnv; 
END:
    if (homeDir) {
        g_object_unref(homeDir);
        homeDir = nullptr;
    }

    return rv;
}

std::string ClientConfig::getIniPath(bool isHomeDir) const {
    std::string rv;
    std::string iniUserPath;

#ifdef HUNE_DEBUG
    rv = "../../client.ini";
#else
    rv = HUNE_APPLICATION_ETC_PATH;
    rv.append("/client.ini");
#endif

    StringUtils::format(iniUserPath, "%s/%s", this->getUserConfigDirPath().c_str(), NAC_CLIENT_INI_NAME);

    if (isHomeDir) {
        rv = iniUserPath;
    } else {
#ifndef HUNE_DEBUG    
        if (FileUtils::isExist(iniUserPath.c_str())) {
            rv = iniUserPath;
        }
#endif
    }
    
    return rv;
}

bool ClientConfig::existUserConfigDirIfNotCreate() const {
    GFile *dir = nullptr;
    bool rv = false;
    std::string path = this->getUserConfigDirPath();
    GError *error = nullptr;

    dir = g_file_new_for_path(path.c_str());
    if (dir) {
        if (g_file_query_exists(dir, nullptr) == true) {
            rv = true;
        } else {
            if (g_file_make_directory_with_parents(dir, nullptr, &error) == true) {
                rv = true;
            }
        }
    }

    if (dir) {
        g_object_unref(dir);
        dir = nullptr;
    }

    if (error) {
        //FTC_LOG("fail existConfigDirIfNotCreate : %s", error->message);
        g_error_free(error);
        error = nullptr;
    }

    return rv;
}

std::string ClientConfig::getUserDownloadDirPath() const {
    std::string rv, homeDir;
    std::stringstream ssEng, ssKor;
    passwd *pwd = nullptr;
    GFile *downDirEng = nullptr, *downDirKor = nullptr;

    homeDir = this->getUserHomeDirPath();

    ssEng << homeDir;
    ssEng << "/Downloads";
    downDirEng = g_file_new_for_path(ssEng.str().c_str());
    if (downDirEng) {
        if (g_file_query_exists(downDirEng, nullptr)) {
            rv = ssEng.str();
        }
    }

    if (rv.length() == 0) {
        ssKor << homeDir;
        ssKor << "/다운로드";
        downDirKor = g_file_new_for_path(ssKor.str().c_str());
        if (downDirKor) {
            if (g_file_query_exists(downDirKor, nullptr)) {
                rv = ssKor.str();
            }
        }
    }

END:
    if (downDirEng) {
        g_object_unref(downDirEng);
        downDirEng = nullptr;
    }

    if (downDirKor) {
        g_object_unref(downDirKor);
        downDirKor = nullptr;
    }

    return rv;
}

END_HUNE_CORE
