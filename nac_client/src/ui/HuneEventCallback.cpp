#include <gtk/gtk.h>
#include <string>
#include <list>
#include <mutex>
#include "HuneEventCallback.h"
#include "../core/core.h"


class EventCustom ; 

static bool event_g_idle_stop = false ; 
static std::list<EventCustom *> _events ; 
static std::mutex _lock ; 

class EventCustom{
public: 
    EventCustom(const char *event_name , void *user_data, void (*release)(void*)):
    eventName(event_name), 
    userData(user_data), 
    release(release)
    {

    }
    virtual ~EventCustom(){
        if (userData && release){
            release(userData); 
        }
    }

    std::string eventName ; 
    void *userData ; 
    void (*release)(void*) ; 
}; 

static bool push_event(EventCustom *evt){
    bool rv = false ; 
    std::lock_guard<std::mutex> lk(_lock); 
    if (!evt) return rv ; 
    if (event_g_idle_stop) return rv ; 
    _events.push_back(evt); 
    rv = true ; 
    return rv ;  
}

static EventCustom *pop_event(){
    EventCustom *rv = nullptr ; 
    std::lock_guard<std::mutex> lk(_lock); 
    if (_events.empty() == true){
        return rv ; 
    }
    rv = _events.front(); 
    _events.pop_front(); 
    return rv ; 
}

static void clear(){
    std::lock_guard<std::mutex> lk(_lock); 
    _events.clear(); 
}


static gboolean event_source_func(gpointer data){
    gboolean rv = false ; 
    EventCustom *evt = nullptr ; 
    int loopCount = 0 ; 
    Hune::Core::EventManager *manager = Hune::Core::EventManager::getInstance(); 
    if (! manager) {
        return rv;
    }

    do {
        if (loopCount > 3) break ; 

        evt = pop_event(); 
        if (!evt) break ; 
        
        manager->dispatchEvent(evt->eventName.c_str(), evt->userData); 

        delete evt ; 
        evt = nullptr ; 
        
        ++loopCount ; 

    } while(true); 
    
    return rv ; 
}


static bool evnet_manager_callback(const char *event_name , void *user_data, void (*release)(void*)){
    bool rv = false ; 
    EventCustom *evt = nullptr ; 
    if (event_g_idle_stop) return rv; 

    if (!event_name) return rv; 
    evt = new (std::nothrow) EventCustom(event_name , user_data , release); 
    if (evt == nullptr) return rv ; 

    if (push_event(evt)){
        g_main_context_invoke(NULL, event_source_func, NULL); 
        rv = true ; 
    }
    
    return rv ; 
}

bool hune_event_callback_init(){
    bool rv = false ; 
    Hune::Core::EventManager *manager = Hune::Core::EventManager::getInstance();
    if (!manager) return rv ; 
    manager->setAsyncEventCallback(evnet_manager_callback); 
    rv = true ; 
    return rv ; 
}

bool hune_event_callback_dispose(){
    bool rv = false; 
    if (event_g_idle_stop) return rv ; 
    event_g_idle_stop = true ; 
    rv = true ; 
    return rv ; 
}