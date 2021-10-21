#ifndef _HUNE_CORE_TIME_UTILS_H_
#define _HUNE_CORE_TIME_UTILS_H_
#include "StringUtils.h"
#include "macro.h"
#include <ctime>

BEGIN_HUNE_CORE
namespace TimeUtils {
    std::tm now();
    std::tm addDay(std::tm &tm, int day);
    std::string getDateFormat(std::tm &tm, const char *format);
    std::string getHM(int sec);
}
END_HUNE_CORE
#endif /* _HUNE_CORE_TIME_UTILS_H_ */
