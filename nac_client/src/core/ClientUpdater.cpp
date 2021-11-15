#include "ClientUpdater.h"
#include "core.h"
#include "IonexShell.h"
#include "TimeUtils.h"
#include "sys_utils.h"
#include "model/Define.h"


BEGIN_HUNE_CORE

ClientUpdater::ClientUpdater() {
    
}

ClientUpdater::~ClientUpdater() {

}

bool ClientUpdater::isUpdate() {
    const ClientConfig &clientConfig = GlobalVar::getClientConfig();
    std::string url, newVerPath, curVerPath, newVer, curVer;
    std::tm tm = TimeUtils::now();

    StringUtils::format(url, "https://%s:%d/linuxUpdate/version.txt", clientConfig.getServer().c_str(), clientConfig.getPort());
    StringUtils::format(newVerPath, "/tmp/nac_ver_%s", TimeUtils::getDateFormat(tm, "%Y%m%d%H%M%S").c_str());

    Ionex::Shell::HttpDownload(url, newVerPath);

    newVer = FileUtils::fileReadString(newVerPath.c_str());

    curVerPath = "/etc/ionenac/version.txt";
    curVer = FileUtils::fileReadString(curVerPath.c_str());
    
    if (curVer.compare(newVer) == 0) {
        return false;
    }
    
    return true;
}

bool ClientUpdater::update() {
    const ClientConfig &clientConfig = GlobalVar::getClientConfig();
    std::string url, path;

    StringUtils::format(url, "https://%s:%d/linuxUpdate/ionenac-client.deb", clientConfig.getServer().c_str(), clientConfig.getPort());
    StringUtils::format(path, "%s/ionenac_client_new.deb", clientConfig.getUserDownloadDirPath().c_str());

    Ionex::Shell::HttpDownload(url, path);

    hune_open_file(path);

    return true;
}

END_HUNE_CORE
