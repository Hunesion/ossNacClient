#ifndef _HUNE_CORE_STRING_UTILS_H_
#define _HUNE_CORE_STRING_UTILS_H_
#include <list>
#include <vector>
#include <string>
#include <locale>
#include <map>
#include "macro.h"

BEGIN_HUNE_CORE
namespace StringUtils {
    void ltrim(std::string &s);
    void ltrim(std::string &s, char remove);
    void rtrim(std::string &s);
    void rtrim(std::string &s, char remove);
    void trim(std::string &s);
    void trim(std::string &s, char remove);
    int getInt(const std::string &s); 
    int getInt(const char* s); 
    long getlong(const std::string &s); 
    long getlong(const char* s); 
    bool getBool(const std::string &s); 
    bool getBool(const char* s); 
    void replace(std::string &s, const std::string &p, const std::string &r);
    std::list<std::string> split(const std::string &s, const char p);
    std::vector<std::string> splitToVector(const std::string &s, const char p);
    void format(std::string &s, const std::string &fmt, ...);
    std::string format(const std::string &fmt, ...); 
    bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen); 
    bool endsWith(const std::string& str, const char* suffix); 
    bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen); 
    bool startsWith(const std::string& str, const char* prefix); 
    std::map<std::string, std::string> getKeyValue(const std::string& str, const char s = '=', const char p = '\n');
}
END_HUNE_CORE
#endif /* _HUNE_CORE_STRING_UTILS_H_ */