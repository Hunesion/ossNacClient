#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>
#include <cstdarg>

#include "TimeUtils.h"
#include "StringUtils.h"

BEGIN_HUNE_CORE

namespace TimeUtils
{
    std::tm now()
    {
        std::tm rv;
        std::time_t now_date = std::time(NULL);
        localtime_r(&now_date, &rv);
        return rv;
    }

    std::tm addDay(std::tm &tm, int day)
    {
        std::tm rv;
        std::time_t time = std::mktime(&tm);

        time += 60 * 60 * 24 * day;
        
        localtime_r(&time, &rv);
        return rv;
    }

    std::string getDateFormat(std::tm &tm, const char *format)
    {
        char buffer[256] = { 0, };

        if (! format) {
            return "";
        }

        strftime(buffer, 256, format, &tm);
        
        return buffer;
    }

    std::string getHM(int sec)
    {
        std::string rv;
        int num = sec;

        //시간공식
        int hours = num / 3600;     //시 공식
        int minute = num % 3600 / 60;//분을 구하기위해서 입력되고 남은값에서 또 60을 나눈다.

        if (hours != 0)
        {
            rv += StringUtils::format("%d시간 ", hours);
        }

        if (minute != 0)
        {
            //result += String.Format("{0:D2}", minute) + "분";
            rv += StringUtils::format("%d분", minute);
        }

        return rv;
    }
} // namespace TimeUtils

END_HUNE_CORE