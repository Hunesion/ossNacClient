#ifndef _HUNE_CORE_FILE_INFO_H_
#define _HUNE_CORE_FILE_INFO_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include "macro.h"

BEGIN_HUNE_CORE
class FileInfo{
private:
    FileInfo(const char *path); 
public:
    FileInfo(); 
    virtual ~FileInfo(); 
    static FileInfo getFileInfo(const char *path); 
    bool isExist() const ; 
    bool isFile() const ; 
    bool isDirectory() const ; 
    bool isReadable() const ; 
    bool isWritable() const ; 
    long long length() const ;
    const std::string &getFullPath() const;  
    const std::string &getPath() const ; 
    const std::string &getFileName() const ; 
    const std::string &getRelativeDir() const ;
    void setRelativeDir(const char *relativeDir);

    std::tm getLastModifyTime();
    std::tm getLastAccessTime();
    std::tm getLastStatusChangeTime();
private: 
    void seperatePath(); 

private: 
    std::string _fullPath ; 
    std::string _fileName ; 
    std::string _path ; 
    std::string _relativeDir ;
    struct stat _stat ; 
    bool _exist ; 
}; 
END_HUNE_CORE
#endif