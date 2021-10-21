#ifndef _HUNE_CORE_LOGGER_H_
#define _HUNE_CORE_LOGGER_H_

#include <stdarg.h>
#include "macro.h"

#define HUNE_LOG(format, ... ) Hune::Core::Logger::log(format , ##__VA_ARGS__)

#ifdef HUNE_DEBUG
#define HUNE_LOG_DEBUG(format , ... )  Hune::Core::Logger::log(format , ##__VA_ARGS__)
#else 
#define HUNE_LOG_DEBUG( ... ) do{}while(0)
#endif

BEGIN_HUNE_CORE
namespace Logger{
    void init(const char *path, const char *prefix);
    
    void log(const char *format , ... ); 
}
END_HUNE_CORE


#endif