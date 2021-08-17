#ifndef _HUNE_CORE_EVENT_MANAGER_H_
#define _HUNE_CORE_EVENT_MANAGER_H_
#include <functional>
#include <map>
#include <list>
#include "macro.h"


BEGIN_HUNE_CORE

class Event ; 
class EventListener ; 
class EventManager; 

typedef const std::function<void(void*, Event*)> EVENT_CALLBACK; 

class Event {
public: 
    static Event *create(const char *eventName, void* _userData = nullptr); 
    static Event *create(const std::string &eventName , void* _userData = nullptr); 
    virtual ~Event(); 
    const std::string &getName(){ return _name; }
    void *getUserData() {return _userData; }
    inline int incRefCount(){
        ++(this->_refCount); 
        return this->_refCount; 
    }
    inline void decRefCount() {
        --(this->_refCount); 
        if (this->_refCount <= 0) {
            delete this; 
        }
    }

    

private: 
    Event(const std::string &eventName, void *userData); 
    

    std::string _name ; 
    void *_userData ; 
    int _refCount ; 

}; 

class EventListener{
public : 
    static EventListener *create(void *target , const char *eventName , const EVENT_CALLBACK &callback); 
    static EventListener *create(void *target, const std::string &eventName , const EVENT_CALLBACK &callback); 
    virtual ~EventListener(); 
    const std::string &getEventName(){return this->_eventName; }
    void *getTarget(){return this->_target; }
    const EVENT_CALLBACK &getCallback(){return this->_callback;}
    int getRefCount(){ return this->_refCount ; }
    int incRefCount(){ ++(this->_refCount); return this->_refCount ; }
    inline void decRefCount(){ 
        --(this->_refCount) ; 
        if (this->_refCount <= 0) {
            delete this ; 
        }
    }

private : 
    EventListener(void *target , const std::string &eventName , const EVENT_CALLBACK &callback); 
    
    std::string _eventName ; 
    void* _target ;  
    //std::function<void(Event *event)> _callback ; 
    EVENT_CALLBACK _callback;
    int _refCount ; 
}; 


class EventManager{
public: 
    static EventManager *getInstance(); 
    static void destroyInstance(); 
    bool addEventListener(void *target , const char *eventName , const EVENT_CALLBACK &callback);
    bool dispatchEvent(const char *eventName , void *userData = nullptr ) ;
    bool dispatchEventAsync(const char *eventName, void *userData = nullptr , void (*release)(void*) = nullptr);  
    void setAsyncEventCallback(bool (*asynchEventCallback)(const char *eventName , void *userData , void (*release)(void*))); 
    bool removeEventListener(void *target, const char *eventName = nullptr); 
    void dispose(); 

protected:
    EventManager(); 
    ~EventManager(); 
    bool hasEventListener(void *target, const char *eventName); 
    bool addEventListenerToEventMap(EventListener *listener); 
    bool addEventListenerToTargetMap(EventListener *listener); 
    bool removeEventListenerFromEventMap(EventListener *listener) ; 
    bool removeEventListenerFromEventMap(const char *eventName, void *target); 
    bool removeEventListenerFromTargetMap(void *target, const char *eventName); 
    

private:
    static EventManager *s_Instance ; 
    std::map<std::string , std::list<EventListener *>> _eventMap ; 
    std::map<void *, std::list<EventListener*>> _targetMap ; 
    bool (*_asyncEventCallback)(const char *eventName , void *userData , void (*release)(void*)); 
}; 
END_HUNE_CORE
#endif /* _HUNE_CORE_EVENT_MANAGER_H_ */