#include <string.h>

#include "FileInfo.h"
#include "StringUtils.h"
#include "Logger.h"


#define PATH_SEPERATOR '/'


BEGIN_HUNE_CORE

FileInfo::FileInfo():_exist(false){
    
}

FileInfo::FileInfo(const char *path) :
_exist(false){
    int rv = 0 ; 
    size_t site = 0 ;
    const char *tmp = nullptr ; 
    memset(&(this->_stat), 0 , sizeof(struct stat)); 
    if (path == nullptr) return ; 
    this->_fullPath = path ; 
    if (this->_fullPath.empty()) return ; 

    site = this->_fullPath.size() - 1;
    if (this->_fullPath.size() > 1 && this->_fullPath.at(site) == PATH_SEPERATOR){
        StringUtils::rtrim(this->_fullPath, PATH_SEPERATOR);

        // if _fullPath = "//////" => _fullPath = "/"
        if (this->_fullPath.empty()) {
            this->_fullPath = PATH_SEPERATOR;
        }
    }

    tmp = this->_fullPath.c_str(); 

    rv = stat(tmp , &(this->_stat)); 
    if (rv == 0) {
        this->_exist = true ; 
    }
    this->seperatePath(); 
}

FileInfo::~FileInfo(){

}

FileInfo FileInfo::getFileInfo(const char *path){
    FileInfo rv(path); 
    return rv ; 
}

void FileInfo::seperatePath(){
    size_t pos = std::string::npos ; 
    if (this->_fullPath.empty()) return ; 

    pos = this->_fullPath.find_last_of(PATH_SEPERATOR)    ; 
    if (pos == std::string::npos){
        // ex) _fullPath='file.ext' => dir: '.' , file: 'file.ext'
        this->_path = ".";
        this->_fileName = this->_fullPath;
        return ; 
    }

    if (this->isDirectory()){
        this->_path = this->_fullPath ;
    } else {
        this->_path = this->_fullPath.substr(0, pos); 
    }
    if (pos + 1 < this->_fullPath.size()){
        this->_fileName = this->_fullPath.substr(pos +1 ); 
    }
}

bool FileInfo::isExist() const {
    return this->_exist ; 
}

bool FileInfo::isFile() const {
    bool rv = false ; 
    if (!this->_exist) return rv ; 
    rv = S_ISREG(this->_stat.st_mode) ; 
    return rv ; 
}

bool FileInfo::isDirectory() const{
    bool rv = false ; 
    if (!this->_exist) return rv ; 
    rv = S_ISDIR(this->_stat.st_mode) ; 
    return rv ; 
}

bool FileInfo::isReadable() const {
    bool rv = false ; 
    if (!this->_exist) return rv ; 
    rv = (this->_stat.st_mode & S_IRUSR) == S_IRUSR ; 
    return rv ; 
}

bool FileInfo::isWritable() const {
    bool rv = false ; 
    if (!this->_exist) return rv ; 
    rv = (this->_stat.st_mode & S_IWUSR) == S_IWUSR ; 
    return rv ; 
}

long long  FileInfo::length() const{
    long long rv = 0L ;
    if (!this->_exist) return rv ; 
    if (!this->isFile()) return rv ; 
    return this->_stat.st_size; 
}

const std::string & FileInfo::getFullPath() const{
    return this->_fullPath ; 
}
const std::string &FileInfo::getPath() const {
    return this->_path ; 
}
const std::string &FileInfo::getFileName() const {
    return this->_fileName ; 
}

const std::string &FileInfo::getRelativeDir() const {
    return this->_relativeDir ;
}

void FileInfo::setRelativeDir(const char *relativeDir) {
    this->_relativeDir = relativeDir;
}

std::tm FileInfo::getLastModifyTime()
{
    std::tm tm;
    if (! localtime_r(&this->_stat.st_mtim.tv_sec, &tm)) {
        //FTC_LOG_DEBUG("--getLastModifyTime localtime_r fail");
    }
    return tm;
}

std::tm FileInfo::getLastAccessTime()
{
    std::tm tm;
    if (! localtime_r(&this->_stat.st_atim.tv_sec, &tm)) {
        //FTC_LOG_DEBUG("--getLastAccessTime localtime_r fail");
    }
    return tm;
}
    
std::tm FileInfo::getLastStatusChangeTime()
{
    std::tm tm;
    if (! localtime_r(&this->_stat.st_ctim.tv_sec, &tm)) {
        //FTC_LOG_DEBUG("--getLastStatusChangeTime localtime_r fail");
    }
    return tm;
}


END_HUNE_CORE