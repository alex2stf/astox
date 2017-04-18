/*
 * EventDispatcher.cpp
 *
 *  Created on: Aug 5, 2015
 *      Author: alexstf
 */

#include "EventDispatcher.h"


namespace astox {

    std::string Event::ON_FS_FTWR = "ON_FS_FTWR";
    std::string Event::ON_FS_FILE_WATCH_CHANGED = "ON_FS_FILE_WATCH_CHANGED";
    std::string Event::ON_FS_FILE_WATCH = "ON_FS_FILE_WATCH";

    std::map<std::string, std::vector<astox::Function *> > EventDispatcher::_eventListeners;
    int EventDispatcher::pids;

    EventDispatcher::EventDispatcher() {
        // TODO Auto-generated constructor stub
    }

    void EventDispatcher::addEventListener(std::string name, astox::Function * method) {
        pids++;
#ifdef ASTX_DBG_EVENT_DISPATCHER
        astox_debug(__LINE__, __FILENAME__, "EventDispatcher::addEventListener === %s", name.c_str());
#endif
        if (_eventListeners.find(name) == _eventListeners.end()) {
            std::vector<astox::Function *> _namedEvents;
            _eventListeners[name] = _namedEvents;
        }
        _eventListeners[name].push_back(method);
    };

    void EventDispatcher::dispatch(std::string eventName, astox::Value * arguments) {
        for (unsigned int i = 0; i < _eventListeners[eventName].size(); i++) {
            _eventListeners[eventName][i]->setNumCalls(0);
            _eventListeners[eventName][i]->call(arguments);
        }
    };

    void EventDispatcher::removeEventListener(std::string index) {
        if (_eventListeners.find(index) != _eventListeners.end()) {
#ifdef ASTX_DBG_EVENT_DISPATCHER
            astox_debug(__LINE__, __FILENAME__, "EventDispatcher::removeEventListener = %s", index.c_str());
#endif
            _eventListeners.erase(_eventListeners.find(index));
        }
    };

} /* namespace astox */
