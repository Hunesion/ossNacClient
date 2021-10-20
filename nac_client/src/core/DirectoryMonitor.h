#ifndef _HUNE_CORE_DIRECTORY_MONITOR_H_
#define _HUNE_CORE_DIRECTORY_MONITOR_H_
#include <string>
#include <thread>
#include <atomic>
#include <sys/inotify.h>
#include "macro.h"



BEGIN_HUNE_CORE
typedef struct {
    int mask;
    std::string dir;
    std::string filename;
    void *userData;
} DmParam;

class DirectoryMonitor
{
public:
    DirectoryMonitor(const std::string &dir, const std::string &eventName, void *userData); 
    virtual ~DirectoryMonitor(); 

    bool Start(u_int32_t mask);
    void Stop();
private:
    static void MonitoringThreadFunc(DirectoryMonitor *self);
    void InotifyHandler(const struct inotify_event *event);

private:
    std::thread _monitorThread;
    std::string _dir;
    std::string _eventName;
    void *_userData;
    int _fdMonitor;
    int _wdMonitor;
    bool _isStarted;
    bool _isThreadStop;
};

END_HUNE_CORE
#endif /* _HUNE_CORE_DIRECTORY_MONITOR_H_ */


