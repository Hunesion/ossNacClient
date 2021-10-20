#ifndef _HUNE_CORE_FILE_UTILS_H_
#define _HUNE_CORE_FILE_UTILS_H_
#include "macro.h"
#include <gtk/gtk.h>
#include <string>

BEGIN_HUNE_CORE
namespace FileUtils
{
    bool fileWrite(const char *filepath, const char *buffer, unsigned int len, int mode = 0);

    bool fileDelete(const char *filepath);

    bool fileRead(const char *filepath, char **buffer, unsigned int *outlen);

    std::string fileReadString(const char *filepath);

    /*mode 0 : 파일이 있으면 실패, mode 1 : 파일이 있으면 덮어쓰기 mode 2 : 파일이 있으면 Append*/
    bool encFileWrite(const char *filepath, const std::string &writeStr, int mode = 0); 

    std::string encFileRead(const char *filepath);

    bool createDirIfnotExist(const char *dirpath);

    bool isExist(const char *filepath);

    std::string getFileExtension(const char *filepath);

    std::string getFileNameUniqueIndex(const char *filepath);

    bool removeDirectoryInAllFiles(const char *dirpath, bool force);

    std::string getFileSha256(const char *filepath);

    bool copy(const char *source, const char *dest);
}
END_HUNE_CORE

#endif /*_HUNE_CORE_FILE_UTILS_H_*/