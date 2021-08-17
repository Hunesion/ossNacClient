#include <new>
#include "EventManager.h"

BEGIN_HUNE_CORE
EventManager* EventManager::s_Instance = nullptr ; 


Event *Event::create(const char *eventName, void* userData){
    return new (std::nothrow) Event(eventName, userData ); 
}

Event *Event::create(const std::string &eventName , void* userData){
    return new (std::nothrow) Event(eventName, userData ); 
}

Event::Event(const std::string &eventName, void *userData):
_name(eventName),
_userData(userData) {
    this->_refCount = 1 ; 
} 

Event::~Event(){

}



EventListener *EventListener::create(void *target , const char *eventName ,  const EVENT_CALLBACK &callback){
    return new(std::nothrow)EventListener(target, eventName, callback); 
}
EventListener *EventListener::create(void *target, const std::string &eventName , const EVENT_CALLBACK &callback){
    return new(std::nothrow)EventListener(target, eventName, callback); 
}

EventListener::EventListener(void *target , const std::string &eventName , const EVENT_CALLBACK &callback):
_target(target), 
_eventName(eventName), 
_callback(callback){
    this->_refCount = 1 ; 
}

EventListener::~EventListener(){

} 


EventManager* EventManager::getInstance(){
    EventManager *rv = s_Instance; 
    if (!rv){
        rv = new (std::nothrow) EventManager(); 
        s_Instance = rv ; 
    }
    return rv ; 
}

void EventManager::destroyInstance(){
    if (s_Instance){
        delete s_Instance; 
        s_Instance = nullptr; 
    }
}

EventManager::EventManager():
_asyncEventCallback(nullptr){

}
EventManager::~EventManager(){
    this->dispose(); 
}


bool EventManager::hasEventListener(void *target, const char *eventName){
    bool rv = true ; 
    auto it = this->_targetMap.find(target); 
    if (it !=  this->_targetMap.end()){
        for (auto listIt = it->second.begin() ; listIt != it->second.end() ; ++listIt){
            if ((*listIt)->getEventName().compare(eventName) == 0) return rv ; 
        }
    }
    rv = false ; 
    return rv ; 
}
bool EventManager::addEventListenerToEventMap(EventListener *listener){
    bool rv = false ;
    
    if (listener == nullptr) return rv ; 

    auto it = this->_eventMap.find(listener->getEventName()); 
    if (it == this->_eventMap.end()){
        std::list<EventListener *> list ; 
        list.push_back(listener); 
        this->_eventMap.insert(std::pair<std::string, std::list<EventListener*> >(listener->getEventName(), list)); 
    }else {
        it->second.push_back(listener); 
    }
    listener->incRefCount(); 
    rv = true ;
    return rv ; 
}

bool EventManager::addEventListenerToTargetMap(EventListener *listener){
    bool rv = false ;
    void *target = nullptr ;
    if (listener == nullptr) return rv ; 
    target = listener->getTarget() ; 

    auto it = this->_targetMap.find(target); 
    if (it == this->_targetMap.end()){
        std::list<EventListener *> list ; 
        list.push_back(listener); 
        this->_targetMap.insert(std::pair<void *, std::list<EventListener*> >(target, list)); 
    }else {
        it->second.push_back(listener); 
    }
    listener->incRefCount(); 
    rv = true ;
    return rv ; 
} 


bool EventManager::addEventListener(void *target , const char *eventName , const EVENT_CALLBACK &callback){
    bool rv = false ;  
    EventListener *listener = nullptr ; 

    if (target == nullptr || eventName == nullptr) return rv ; 

    if (this->hasEventListener(target, eventName)) return rv ; 
    listener = EventListener::create(target, eventName, callback); 
    if (listener == nullptr) return rv ; 

    if (this->addEventListenerToEventMap(listener)){
        if (this->addEventListenerToTargetMap(listener)){
            rv = true ; 
        }else {
            removeEventListenerFromEventMap(listener); 
        }
    }

    listener->decRefCount() ; 

    return rv ; 
}

bool EventManager::removeEventListenerFromEventMap(EventListener *listener) {
    bool rv = false ; 
    if (!listener) return rv ; 
    rv = this->removeEventListenerFromEventMap(listener->getEventName().c_str(), listener->getTarget()); 
    return rv ; 
}

bool EventManager::removeEventListenerFromEventMap(const char *eventName, void *target){
    bool rv = false ; 
    EventListener *listener  = nullptr ; 
    if (eventName == nullptr && target == nullptr ) return rv ; 
    if (eventName){
        auto it = this->_eventMap.find(eventName); 
        if (it == this->_eventMap.end()) return rv ; 

        auto listIt = it->second.begin(); 
        while(listIt != it->second.end()){
            if (target){
                if ((*listIt)->getTarget() == target){
                    rv = true ; 
                    listener = *listIt;
                    it->second.erase(listIt++);
                    if (listener){
                        listener->decRefCount();
                    }
                }else{
                    listIt++;
                }
            }else {
                rv = true ; 
                listener = *listIt; 
                it->second.erase(listIt++);
                if (listener){
                    listener->decRefCount();
                }
            }
        }

        if (it->second.size() == 0){
            this->_eventMap.erase(it); 
        }
    }else {
        auto it = this->_eventMap.begin(); 
        while(it != this->_eventMap.end()){
            auto listIt = it->second.begin(); 
            while(listIt != it->second.end()){
                if ((*listIt)->getTarget() == target){
                    listener = *listIt ;
                    it->second.erase(listIt++); 
                    if (listener){
                        listener->decRefCount(); 
                    }
                }else {
                    listIt++ ; 
                }
            }
            if (it->second.size() == 0){
                this->_eventMap.erase(it++); 
            }else {
                it++; 
            }
        }
    }

    return rv ; 
}

bool EventManager::removeEventListenerFromTargetMap(void *target, const char *eventName){
    bool rv = false ; 
    EventListener *listener  = nullptr ; 
    if (target == nullptr ) return rv ; 
    auto it = this->_targetMap.find(target); 
    if (it == this->_targetMap.end()) return rv ; 

    auto listIt = it->second.begin(); 
    while(listIt != it->second.end()){
        if (eventName){
            if ((*listIt)->getEventName().compare(eventName) == 0){
                rv = true ; 
                listener = *listIt;
                it->second.erase(listIt++);
                if (listener){
                    listener->decRefCount();
                }
            }else{
                listIt++;
            }
        }else {
            rv = true ; 
            listener = *listIt; 
            it->second.erase(listIt++);
            if (listener){
                listener->decRefCount();
            }
        }
    }

    if (it->second.size() == 0){
        this->_targetMap.erase(it); 
    }

    return rv ; 
}

bool EventManager::dispatchEvent(const char *eventName , void *userData) {
    bool rv = false ; 
    EventListener *listener = nullptr ; 
    if (eventName == nullptr) return rv ; 

    auto it = this->_eventMap.find(eventName); 
    if (it == this->_eventMap.end() || it->second.size() == 0) return rv ; 

    Event *event = Event::create(eventName, userData); 
    if (event == nullptr) {
        return rv;
    }

    for (auto listIt = it->second.begin() ; listIt != it->second.end() ; listIt++){
        listener = *listIt ; 
        if (listener){
            event->incRefCount(); 
            listener->getCallback()(listener->getTarget(), event); 
            event->decRefCount(); 
        }

        if (this->_eventMap.find(eventName) == this->_eventMap.end()) {
            break;
        }
    }

    event->decRefCount(); 
}

void EventManager::setAsyncEventCallback(bool (*asynchEventCallback)(const char *eventName , void *userData, void (*release)(void*))){
    this->_asyncEventCallback = asynchEventCallback; 
}

bool EventManager::dispatchEventAsync(const char *eventName, void *userData , void (*release)(void*) ){
    bool rv = false ; 
    if (this->_asyncEventCallback){
        rv = this->_asyncEventCallback(eventName, userData , release); 
    }

    if (!rv){
        if (userData && release){
            release(userData); 
        }
    }

    return rv ; 
} 

bool EventManager::removeEventListener(void *target, const char *eventName){
    bool rv = false ; 
    if (target == nullptr) return rv ; 
    this->removeEventListenerFromEventMap(eventName, target); 
    this->removeEventListenerFromTargetMap(target, eventName); 
    rv = true ; 
    return rv ; 
} 

void EventManager::dispose(){
    EventListener *listener = nullptr ; 
    for (auto it = this->_eventMap.begin() ; it != this->_eventMap.end() ; it++){
        for (auto listIt = it->second.begin() ; listIt != it->second.end() ; listIt++){
            listener = *listIt ; 
            if (listener){
                listener->decRefCount(); 
            }
        }
        it->second.clear(); 
    }
    this->_eventMap.clear(); 


    for (auto it = this->_targetMap.begin() ; it != this->_targetMap.end() ; it++){
        for (auto listIt = it->second.begin() ; listIt != it->second.end() ; listIt++){
            listener = *listIt ; 
            if (listener){
                listener->decRefCount(); 
            }
        }
        it->second.clear(); 
    }
    this->_targetMap.clear(); 
}


END_HUNE_CORE
