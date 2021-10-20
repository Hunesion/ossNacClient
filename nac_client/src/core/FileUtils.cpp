#include "FileUtils.h"
#include "Logger.h"
#include "StringUtils.h"
#include "crypto.h"
#include "FileInfo.h"
#include <sstream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


BEGIN_HUNE_CORE
namespace FileUtils
{
        
    bool fileWrite(const char *filepath, const char *buffer, unsigned int len, int mode) {
        GFile *file = NULL;
        GOutputStream *stream = NULL;
        GError *error = NULL;
        gsize outLen = 0, writeBytes = 0;
        bool rv = false, exist = false;

        if (filepath == NULL || buffer == NULL) {
            return rv;
        }
        if (len == 0) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (file == NULL) {
            return rv;
        }

        if (g_file_query_exists(file, NULL) == true) {
            if (mode == 0) {
                return rv;
            } else if (mode == 1) {
                if (g_file_delete(file, NULL, NULL) == false) {
                    return rv;
                }
            } else {
                exist = true;
            }
        }

        try {
            if (exist) {
                stream = G_OUTPUT_STREAM(g_file_append_to(file, G_FILE_CREATE_NONE, NULL, &error));
            } else {
                stream = G_OUTPUT_STREAM(g_file_create(file, G_FILE_CREATE_NONE, NULL, &error));
            }
            if (stream == NULL) {
                throw 1;
            }

            if (g_output_stream_write_all(stream, buffer, len, &outLen, NULL, &error) == false) {
                throw 2;
            }

            rv = true;
        } catch (int exception) {
            HUNE_LOG("FileUtis File Write Exception %d", exception);
            if (error != NULL) {
                HUNE_LOG(" -- FileUtils File Write Exception ErrorMessage : %s", error->message);
            }
        }

        if (stream != NULL) {
            g_output_stream_close(stream, NULL, NULL);
            g_object_unref(stream);
            stream = NULL;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    bool fileDelete(const char *filepath)
    {
        GFile *file = NULL;
        bool rv = false;

        if (filepath == NULL) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (file == NULL) {
            return rv;
        }

        if (g_file_query_exists(file, NULL) == true) {
            rv = g_file_delete(file, NULL, NULL);
        }

        return rv;
    }

    bool fileRead(const char *filepath, char **buffer, unsigned int *outlen)
    {
        GFile *file = NULL;
        GInputStream *istream = NULL;
        GError *error = NULL;
        gchar *read = NULL;
        gsize readBytes = 0;
        bool rv = false;

        if (! filepath || ! outlen) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (! file) {
            return rv;
        }

        rv = g_file_load_contents(file, NULL, &read, &readBytes, NULL, NULL);
        if (rv) {
            *buffer = new char[readBytes];
            memcpy(*buffer, read, readBytes);
            *outlen = readBytes;
        }

        if (read) {
            g_free(read);
            read = NULL;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            HUNE_LOG(error->message);
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    std::string fileReadString(const char *filepath)
    {
        GFile *file = NULL;
        GInputStream *istream = NULL;
        GError *error = NULL;
        gsize readBytes = 0;
        std::string rv = "";  

        if (! filepath) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (! file) {
            return rv;
        }

        try
        {
            char buffer[1024] = { 0, };

            istream = G_INPUT_STREAM(g_file_read(file, NULL, &error));
            if (! istream) {
                
                throw std::runtime_error(error->message);
            }

            do
            {
                readBytes = g_input_stream_read(istream, buffer, sizeof(buffer), NULL, &error);
                if (readBytes == 0) {
                    break;
                } else if (readBytes < 0) {
                    throw std::runtime_error(error->message);
                }
                rv.append(buffer, readBytes);
            } while (true);
        }
        catch(const std::runtime_error& e)
        {
            HUNE_LOG("FileUtils File Read String exception %s \n", e.what());
            rv = "";
        }

        if (istream != NULL) {
            g_input_stream_close(istream, NULL, NULL);
            g_object_unref(istream);
            istream = NULL;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    bool encFileWrite(const char *filepath, const std::string &writeStr, int mode)
    {
        GFile *file = NULL;
        GOutputStream *stream = NULL;
        GError *error = NULL;
        gsize writeBytes = 0;
        bool rv = false, exist = false;
        std::string data, encData;
        std::list<std::string> splitData;

        if (writeStr.length() == 0) {
            return rv;
        }

        data = writeStr;
        splitData = StringUtils::split(data, '\n');

        if (splitData.size() == 0) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (file == NULL) {
            return rv;
        }

        if (g_file_query_exists(file, NULL) == true) {
            if (mode == 0) {
                return rv;
            } else if (mode == 1) {
                if (g_file_delete(file, NULL, &error) == false) {
                    return rv;
                }
            } else {
                exist = true;
            }
        }

        try {
            if (exist) {
                stream = G_OUTPUT_STREAM(g_file_append_to(file, G_FILE_CREATE_NONE, NULL, &error));
            } else {
                stream = G_OUTPUT_STREAM(g_file_create(file, G_FILE_CREATE_NONE, NULL, &error));
            }
            
            if (stream == NULL) {
                throw 1;
            }

            for (auto it : splitData) {
                encData = aes_256_encrypt_and_base64(AES_DEFAULT_KEY, it.c_str(), strlen(it.c_str()));
                encData += "\n";

                if (g_output_stream_write_all(stream, encData.c_str(), strlen(encData.c_str()), &writeBytes, NULL, &error) == false) {
                    throw 2;
                }
            }

            rv = true;
        } catch (int exception) {
            HUNE_LOG("FileUtis ENC File Write Exception %d", exception);
            if (error != NULL) {
                HUNE_LOG(" -- FileUtis ENC File Write Exception ErrorMessage %s", error->message);
            }
        }

        if (stream != NULL) {
            g_output_stream_close(stream, NULL, NULL);
            g_object_unref(stream);
            stream = NULL;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    std::string encFileRead(const char *filepath)
    {
        GFile *file = NULL;
        GInputStream *istream = NULL;
        GError *error = NULL;
        gsize readBytes = 0;
        std::string rv, data, plain;
        std::list<std::string> splitData;
        std::stringstream ss;

        if (! filepath) {
            return rv;
        }

        file = g_file_new_for_path(filepath);
        if (! file) {
            return rv;
        }

        try
        {
            char buffer[1024] = { 0, };

            istream = G_INPUT_STREAM(g_file_read(file, NULL, &error));
            if (! istream) {
                throw std::exception();
            }

            do
            {
                readBytes = g_input_stream_read(istream, buffer, sizeof(buffer), NULL, &error);
                if (readBytes == 0) {
                    break;
                } else if (readBytes < 0) {
                    throw std::exception();
                }
                data.append(buffer, readBytes);
            } while (true);

            splitData = StringUtils::split(data, '\n');

            for (auto it : splitData) {
                plain = aes_256_decrypt_and_base64(AES_DEFAULT_KEY, it.c_str(), strlen(it.c_str()));
                StringUtils::trim(plain);

                ss << plain;
                ss << "\n";
            }

            rv = ss.str();
        }
        catch(const std::exception& e)
        {
            HUNE_LOG("FileUtils ENC File Read Exception");
            if (error != NULL) {
                HUNE_LOG(" -- FileUtils ENC File Read ErrorMessage %s", error->message);
            }
            rv = "";
        }

        if (istream != NULL) {
            g_input_stream_close(istream, NULL, NULL);
            g_object_unref(istream);
            istream = NULL;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    bool createDirIfnotExist(const char *dirpath)
    {
        GFile *file = NULL;
        GError *error = NULL;
        bool rv = false;
        
        file = g_file_new_for_path(dirpath);
        if (file == NULL) {
            return rv;
        }    

        if (g_file_query_exists(file, NULL) == true) {
            rv = true;
        } else {
            if (g_file_make_directory_with_parents(file, NULL, &error) == true) {
                rv = true;
            } else {
                HUNE_LOG("createDirIfnotExist FAIL %s", error->message);
            }
        }
        
        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        if (error != NULL) {
            g_error_free(error);
            error = NULL;
        }

        return rv;
    }

    bool isExist(const char *filepath) 
    {
        GFile *file = NULL;
        bool rv = false;
        
        file = g_file_new_for_path(filepath);
        if (file == NULL) {
            return rv;
        }    

        if (g_file_query_exists(file, NULL) == true) {
            rv = true;
        }

        if (file != NULL) {
            g_object_unref(file);
            file = NULL;
        }

        return rv;
    }

    std::string getFileExtension(const char *filepath)
    {
        std::string rv = "", file_name;
        std::string::size_type idx;

        if (!filepath) {
            return rv;
        }

        file_name = filepath;

        idx = std::string(file_name).rfind('.');
        if (idx != std::string::npos) {
            rv = file_name.substr(idx + 1);
        }
        
        return rv;
    }

    std::string getFileNameUniqueIndex(const char *filepath)
    {
        FileInfo fi = FileInfo::getFileInfo(filepath);
        std::string rv, indexFilename, filename, extension, dir, indexFullpath;
        int index = 1;

        if (fi.isExist() == false) {
            rv = fi.getFileName();
            return rv;
        }

        dir = fi.getPath();
        filename = fi.getFileName();
        extension = "." + getFileExtension(filename.c_str());
        if (StringUtils::endsWith(filename, extension.c_str())) {
            filename = filename.substr(0, filename.length() - extension.length());
        }
        else
        {
            extension = "";
        }
        
        
        do {
            indexFilename = filename;
            indexFilename.append("(");
            indexFilename.append(std::to_string(index));
            indexFilename.append(")");
            if (extension.empty() == false)
            {
                indexFilename.append(extension);
            }

            indexFullpath = dir + "/" + indexFilename;
            fi = FileInfo::getFileInfo(indexFullpath.c_str());
            index++;
        } while (fi.isExist() == true);

        rv = fi.getFileName();
        return rv;
    }

    bool removeDirectoryInAllFiles(const char *dirpath, bool force)
    {
        DIR * dir_ptr = NULL;
        struct dirent *file = NULL;
        struct stat buf;
        char filename[1024]; /* 목록을 읽을 디렉토리명으로 DIR *를 return 받습니다. */ 
        bool rv = false;
        
        if((dir_ptr = opendir(dirpath)) == NULL) { 
            /* path가 디렉토리가 아니라면 삭제하고 종료합니다. */ 
            if (unlink(dirpath) == -1) {
                return rv;
            }
        } 
        
        /* 디렉토리의 처음부터 파일 또는 디렉토리명을 순서대로 한개씩 읽습니다. */
        while((file = readdir(dir_ptr)) != NULL) { 
            // readdir 읽혀진 파일명 중에 현재 디렉토리를 나타네는 . 도 포함되어 있으므로 // 무한 반복에 빠지지 않으려면 파일명이 . 이면 skip 해야 함 
            if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) { 
                 continue;
            } 
            
            sprintf(filename, "%s/%s", dirpath, file->d_name);
            /* 파일의 속성(파일의 유형, 크기, 생성/변경 시간 등을 얻기 위하여 */ 
            if(lstat(filename, &buf) == -1) { 
                continue;
            } 
            
            if(S_ISDIR(buf.st_mode)) { 
                // 검색된 이름의 속성이 디렉토리이면 /* 검색된 파일이 directory이면 재귀호출로 하위 디렉토리를 다시 검색 */
                if(removeDirectoryInAllFiles(filename, force) == false && !force) { 
                    return rv;
                } 
            } else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { 
                // 일반파일 또는 symbolic link 이면 
                if(unlink(filename) == -1 && !force) { 
                    return rv;
                } 
            } 
        } 
        
        /* open된 directory 정보를 close 합니다. */ 
        closedir(dir_ptr);

        if (rmdir(dirpath) == -1) {
            return rv;
        }

        rv = true;
        return rv;
    }

    std::string getFileSha256(const char *filepath)
    {
        std::string rv;
        char sha256[SHA256_HEX_LENGTH] = { 0, };
        char *buf = NULL;
        int fd = -1, file_len = 0, read_size = 0, read_total = 0;

        if (! filepath) {
            return rv;
        }

        fd = open(filepath, O_RDONLY);
        if (fd < 0) {
            goto END;
        }

        file_len = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);

        buf = new char[file_len];
        if (! buf) {
            goto END;
        }
        memset(buf, 0, file_len);

        do {
            read_size = read(fd, (buf + read_total), (file_len - read_total)) ;
            if (read_size <= 0) break ; 
            read_total += read_size ; 
        }while(file_len != read_total); 

        Hune::Core::get_sha256(buf, file_len, sha256, SHA256_HEX_LENGTH);
        rv = sha256;
        for (auto & c: rv) {
            c = std::toupper(c);
        }

END:
        if (fd >= 0) {
            close(fd);
        }

        if (buf) {
            delete[] buf;
            buf = NULL;
        }
        
        return rv;
    }

    
    static int copyFile(const char *source, const char *dest)
    {
        int input, output;
        struct stat stInput;

        if ((input = open(source, O_RDONLY)) == -1)
        {
            return -1;
        }    
        if ((output = creat(dest, 0660)) == -1)
        {
            close(input);
            return -1;
        }

        //sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
        off_t bytesCopied = 0;
        struct stat fileinfo = {0};
        fstat(input, &fileinfo);
        int result = sendfile(output, input, &bytesCopied, fileinfo.st_size);
        if (result >= 0) {
            stat(source, &stInput);
            chmod(dest, stInput.st_mode);
        }

        close(input);
        close(output);

        return result;
    }

    static bool copyDirectory(const char *source, const char *dest)
    {
        DIR *dir = NULL;
        struct dirent *de = NULL;
        bool rv = false;

        if (! createDirIfnotExist(dest)) {
            return rv;
        }

        dir = opendir(source);
        if (dir) {
            while (de = readdir(dir)) {
                Hune::Core::FileInfo subFi;
                std::string sourceTarget, destTarget;
                
                sourceTarget = source;
                sourceTarget.append("/");
                sourceTarget.append(de->d_name);

                destTarget = dest;
                destTarget.append("/");
                destTarget.append(de->d_name);

                subFi = Hune::Core::FileInfo::getFileInfo(sourceTarget.c_str());
                if (subFi.isExist()) {
                    if (subFi.isDirectory()) {
                        if (strcmp(de->d_name, ".") == 0) {
                            continue;
                        } else if (strcmp(de->d_name, "..") == 0) {
                            continue;
                        } else {
                            copyDirectory(sourceTarget.c_str(), destTarget.c_str());
                        }
                    } else if (subFi.isFile()) {
                        copyFile(sourceTarget.c_str(), destTarget.c_str());
                    }
                }
            }

            closedir(dir);
        }

        rv = true;
        return rv;
    }

    bool copy(const char *source, const char *dest)
    {
        FileInfo sourceFi; 
        FileInfo destFi;
        std::string destTarget; 
        bool rv = false;

        if (! source || ! dest) {
            return rv;
        }

        sourceFi = FileInfo::getFileInfo(source);
        destFi = FileInfo::getFileInfo(dest);
        destTarget = dest;

        if (! sourceFi.isExist()) {
            return rv;
        }

        if (destFi.isExist()) {
            if (destFi.isDirectory()) {
                destTarget.append("/");
                destTarget.append(sourceFi.getFileName());
            } else {
                fileDelete(destTarget.c_str());
            }
        }

        if (sourceFi.isDirectory()) {
            rv = copyDirectory(source, destTarget.c_str());
        } else {
            if (copyFile(source, destTarget.c_str()) > 0) {
                rv = true;
            } else {
                rv = false;
            }
        }

        return rv;
    }
}

END_HUNE_CORE

