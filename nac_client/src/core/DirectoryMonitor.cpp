#include "DirectoryMonitor.h"
#include "core.h"
#include <new>

#define     INOTIFY_EVENT_SIZE  (sizeof(struct inotify_event))
#define     INOTIFY_BUFFER      (1024 * (INOTIFY_EVENT_SIZE + 16))

BEGIN_HUNE_CORE

DirectoryMonitor::DirectoryMonitor(const std::string &dir, const std::string &eventName, void *userData) {
    this->_dir = dir;
    this->_eventName = eventName;
    this->_userData = userData;
    this->_fdMonitor = 0;
    this->_wdMonitor = 0;
    this->_isStarted = false;
    this->_isThreadStop = false;
}

DirectoryMonitor::~DirectoryMonitor() {

}

bool DirectoryMonitor::Start(u_int32_t mask) {
    this->_fdMonitor = inotify_init();
    if (this->_fdMonitor <= 0) {
        return false;
    }

    this->_wdMonitor = inotify_add_watch(this->_fdMonitor, this->_dir.c_str(), mask);
    if (this->_wdMonitor <= 0) {
        close(this->_fdMonitor);
        return false;
    } 

    this->_monitorThread = std::thread(MonitoringThreadFunc, this);

    this->_isStarted = true;
    return this->_isStarted;
}

void DirectoryMonitor::Stop() {
    if (this->_isStarted == false) {
        return;
    }

    this->_isThreadStop = true;
    if (this->_wdMonitor > 0) {
        inotify_rm_watch(this->_fdMonitor, this->_wdMonitor);
        this->_wdMonitor = 0;
    }
    this->_monitorThread.join();
    this->_isThreadStop = false;

    if (this->_fdMonitor > 0) {
        close(this->_fdMonitor);
        this->_fdMonitor = 0;
    }

    this->_isStarted = false;
}

void DirectoryMonitor::MonitoringThreadFunc(DirectoryMonitor *self) {
    do {
        char inotifyBuffer[INOTIFY_BUFFER] = { 0, };
        int length = 0, index = 0;

        memset(inotifyBuffer, 0x00, INOTIFY_BUFFER);

        length = read(self->_fdMonitor, inotifyBuffer, INOTIFY_BUFFER);
        while (index < length) {
            struct inotify_event *event = reinterpret_cast<inotify_event *>(&inotifyBuffer[index]);
            if (event->mask & IN_IGNORED) {
                //  inotify_rm_watch가 호출됨
                break;
            }

            //  0.1초 대기
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            self->InotifyHandler(event);

            index += INOTIFY_EVENT_SIZE + event->len;
        }

    } while (self->_isThreadStop == false);
}

void DirectoryMonitor::InotifyHandler(const struct inotify_event *event) {
    EventManager *eventMgr = EventManager::getInstance();
    DmParam *dmParam = nullptr;
    if (! eventMgr) {
        return;
    }   

    dmParam = new (std::nothrow) DmParam();
    if (!dmParam) {
        return;
    }

    dmParam->dir = this->_dir;
    dmParam->filename = event->name;
    dmParam->mask = event->mask;
    dmParam->userData = this->_userData;

    eventMgr->dispatchEventAsync(this->_eventName.c_str(), dmParam, [](void *data) {
        if (data) {
            DmParam *param = (DmParam*)data;
            delete param;
            param = nullptr;
        }
    });
}

END_HUNE_CORE
