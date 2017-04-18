/*
 * EventDispatcher.h
 *
 *  Created on: Aug 5, 2015
 *      Author: alexstf
 */


#ifndef SRC_ASTOX_EVENTDISPATCHER_H_
    #define SRC_ASTOX_EVENTDISPATCHER_H_

    #include "Macros.h"
    #include "astox.h"

namespace astox {

    struct EventData {
        Function * method;
        Value * arguments;
        int pid;
    };

    class Event {
    public:
        static std::string ON_FS_FTWR;
        static std::string ON_FS_FILE_WATCH_CHANGED;
        static std::string ON_FS_FILE_WATCH;
    };

    class EventDispatcher {
    private:
        static std::map<std::string, std::vector<astox::Function *> > _eventListeners;
        static int pids;
    public:
        EventDispatcher();
        static void addEventListener(std::string name, astox::Function * method);
        static void dispatch(std::string eventName, astox::Value * arguments);
        static void removeEventListener(std::string index);
    };

} /* namespace astox */

#endif /* SRC_ASTOX_EVENTDISPATCHER_H_ */
