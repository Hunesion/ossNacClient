#include <algorithm>
#include <functional>
#include <cctype>

#include <sstream>
#include <cstdarg>
#include <iconv.h>
#include <string.h>
#include <codecvt>

#include "StringUtils.h"

BEGIN_HUNE_CORE

namespace StringUtils
{

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(static_cast<unsigned char>(ch));
    }));
}
void ltrim(std::string &s, char remove)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [remove](char ch) {
        return ch != remove;
    }));
}

void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return !std::isspace(static_cast<unsigned char>(ch));
    }).base(), s.end());
}
void rtrim(std::string &s, char remove)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [remove](char ch) {
        return ch != remove;
    }).base(), s.end());
}

void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}
void trim(std::string &s, char remove)
{
    ltrim(s, remove);
    rtrim(s, remove);
}
int getInt(const std::string &s)
{
    int rv = 0;
    if (s.empty())
        return rv;

    try
    {
        rv = std::stoi(s);
    }
    catch (std::invalid_argument &ex)
    {
        //HUNE_LOG_DEBUG("Failed StringUtils::getInt : %s", ex.what());
    }
    return rv;
}

int getInt(const char* s){
    int rv = 0 ; 
    if (s == nullptr) return rv ; 
    
    std::string str = s ; 

    rv = getInt(str);
    return rv ; 
}

long getlong(const std::string &s)
{
    long rv = 0;
    if (s.empty())
        return rv;

    try
    {
        rv = std::stol(s);
    }
    catch (std::invalid_argument &ex)
    {
        //HUNE_LOG_DEBUG("Failed StringUtils::getlong : %s", ex.what());
    }
    return rv;
}
    
long getlong(const char* s)
{
    long rv = 0 ; 
    if (s == nullptr) return rv ; 
    
    std::string str = s ; 

    rv = getlong(str) ;
    return rv ; 
}

bool getBool(const std::string &s)
{
    bool rv = false;
    try
    {
        if (s.empty())
            return rv;
        std::string tmp = s;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        trim(tmp);
        rv = tmp.compare("true") == 0;
    }
    catch (const std::bad_alloc &ex)
    {
        //HUNE_LOG_DEBUG("Failed StringUtils::getBool %s", ex.what());
    }

    return rv;
}

bool getBool(const char* s){
    bool rv = false ; 
    if (s == nullptr) return rv ; 

    std::string str = s ; 

    rv = getBool(str); 
    return rv;
}

void replace(std::string &s, const std::string &p, const std::string &r)
{
    std::string::size_type pos = 0;
    std::string::size_type offset = 0;

    while ((pos = s.find(p, offset)) != std::string::npos)
    {
        s.replace(s.begin() + pos, s.begin() + pos + p.size(), r);
        offset = pos + r.size();
    }
}

std::list<std::string> split(const std::string &s, const char p)
{
    std::list<std::string> rv;

    if (s.length() == 0)
    {
        return rv;
    }

    std::istringstream istream(s);
    std::string token = "";
    while (std::getline(istream, token, p))
    {
        rv.push_back(token);
    }

    return rv;
}

std::vector<std::string> splitToVector(const std::string &s, const char p)
{
    std::vector<std::string> rv;

    if (s.length() == 0)
    {
        return rv;
    }

    std::istringstream istream(s);
    std::string token = "";
    while (std::getline(istream, token, p))
    {
        rv.push_back(token);
    }

    return rv;
}

void format(std::string &s, const std::string &fmt, ...)
{
    int size = ((int)fmt.size()) * 2;
    std::string buffer;
    va_list ap;
    while (1)
    {
        buffer.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)buffer.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)
        {
            buffer.resize(n);
            s = buffer;
            return;
        }

        if (n > -1) {
            size = n + 1;
        } else {
            size *= 2;
        }
    }

    s = buffer;
}

std::string format(const std::string &fmt, ...)
{
    int size = ((int)fmt.size()) * 2;
    std::string rv;
    va_list ap;
    while (1)
    {
        rv.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)rv.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)
        {
            rv.resize(n);
            return rv;
        }

        if (n > -1) {
            size = n + 1;
        } else {
            size *= 2;
        }
    }
    return rv;
}


bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen)
{
    return str.size() >= suffixLen && 0 == str.compare(str.size()-suffixLen, suffixLen, suffix, suffixLen);
}

bool endsWith(const std::string& str, const char* suffix)
{
    return endsWith(str, suffix, std::string::traits_type::length(suffix));
}

bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen)
{
    return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
}

bool startsWith(const std::string& str, const char* prefix)
{
    return startsWith(str, prefix, std::string::traits_type::length(prefix));
}

std::map<std::string, std::string> getKeyValue(const std::string& str, const char s, const char p)
{
    std::map<std::string, std::string> rv;

    if (str.length() == 0)
    {
        return rv;
    }

    std::istringstream istreamLine(str);
    std::string tokenLine = "";
    while (std::getline(istreamLine, tokenLine, p))
    {
        std::istringstream istreamSep(tokenLine);
        std::string tokenSep;
        std::string keyValue[2];
        int index = 0;

        while(std::getline(istreamSep, tokenSep, s))
        {
            if (index == 2) {
                break;
            }

            keyValue[index] = tokenSep;
            index++;
        }

        rv.insert(std::pair<std::string, std::string>(keyValue[0], keyValue[1]));
    }
}

} // namespace StringUtils

END_HUNE_CORE