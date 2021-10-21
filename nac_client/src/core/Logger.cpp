#include <memory>
#include <mutex>
#include <sstream>
#include <iomanip>
#include "Logger.h"
#include "FileInfo.h"
#include "TimeUtils.h"
#include "FileUtils.h"

#define MAX_LOG_LENGTH (16 * 1024)

static std::mutex s_mtx;
static std::string s_path;
static std::string s_prefix;

BEGIN_HUNE_CORE
namespace Logger{

    void init(const char *path, const char *prefix) {
        FileInfo fi;

        if (path) {
            fi = FileInfo::getFileInfo(path);

            if (! fi.isExist()) {
                if (mkdir(path, 0755) != 0) {
                    return;
                }
            }
            
            s_path = std::string(path);
        }

        if (prefix) {
            s_prefix = std::string(prefix);
        }
    }

    static void logWrite(const char *log) {
        std::unique_lock<std::mutex> lk(s_mtx);       
        std::stringstream ss;
        std::string filepath, writeData;
        std::tm tm = TimeUtils::now();

        //  파일이름
        ss << s_path;
        ss << "/";
        ss << s_prefix;
        filepath = ss.str();
        ss.str("");
    
        //  로그포멧
        ss << "[";
        ss << TimeUtils::getDateFormat(tm, "%Y-%02m-%02d %H:%M:%S");
        ss << "] ";
        ss << "[";
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(sizeof(unsigned long)) << std::hex << pthread_self(); /*thread id*/
        ss << "] ";
        ss << log;
        writeData = ss.str();

        //  저장
#ifdef FTC_DEBUG
        FileUtils::fileWrite(filepath.c_str(), (char*)writeData.c_str(), strlen(writeData.c_str()), 2);
#else
        FileUtils::encFileWrite(filepath.c_str(), writeData, 2);
#endif
    }

    void log(const char * format, ...)
    {
        int bufferSize = MAX_LOG_LENGTH;
        char* buf = nullptr;
        int nret = 0;
        va_list args;

        do
        {
            buf = new (std::nothrow) char[bufferSize];
            if (buf == nullptr) {
                return;
            }
            va_start(args, format);
            nret = vsnprintf(buf, bufferSize - 3, format, args);
            va_end(args);

            if (nret >= 0)
            { 
                if (nret <= bufferSize - 3)
                {
                    break;
                }
                else
                {
                    bufferSize = nret + 3;
                    delete[] buf;
                }
            }
            else 
            {	
                bufferSize *= 2;
                delete[] buf;
            }
        } while (true);
        buf[nret] = '\n';
        buf[++nret] = '\0';

#ifdef FTC_DEBUG
        fprintf(stdout, "%s", buf);
        fflush(stdout);
#endif
        logWrite(buf);

        if (buf) {
            delete[] buf;
            buf = NULL;
        }
    }
}
END_HUNE_CORE