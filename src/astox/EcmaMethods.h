/*
 * EcmaMethods.h
 *
 *  Created on: Jul 21, 2015
 *      Author: alexstf
 */

#ifndef SRC_ASTOX_ECMAMETHODS_H_
    #define SRC_ASTOX_ECMAMETHODS_H_

    #include "ExtApi.h"
    #include "MinGUI.h"
    #include "FileSystem.h"


namespace astox {
    namespace ecma {
        namespace methods {
            namespace externs {

                //externe:
                Value * extapi_sassCompile(Value * arguments, Value * object = NULL);
                Value * mingui_notification(Value * arguments, Value * object = NULL);
                Value * mingui_dialog(Value * arguments, Value * object = NULL);

                void getLinuxVersion(std::string pathToSearch, std::string linuxName, Object *& appendable);




                Value * fs_copyFile(Value * arguments, Value * object = NULL);
                std::string fs_exec(std::string cmd);
                Value * fs_exec(Value * arguments, Value * object);



                static std::map<std::string, int> fs_filesToWatch;

                Object * fs_infoToObj(FileInfo *& info);
                Value * fs_joinFiles(Value * arguments, Value * object = NULL);
                Value * fs_joinFilesWithConfig(Value * arguments, Value * object = NULL);
                Value * fs_ftwr(Value * arguments, Value * object = NULL);
                Value * fs_fileWatchAdd(Value * arguments, Value * object = NULL);
                Value * fs_fileWatchRemove(Value * arguments, Value * object = NULL);
                Value * fs_fileWatchStart(Value * arguments = NULL, Value * object = NULL);



                Value * ssh2_connect(Value * arguments, Value * object = NULL);
                Value * ssh2_scpWrite(Value * arguments, Value * object = NULL);

                Value * event_addEventListener(Value * arguments, Value * object = NULL);

                Value * console_log(Value * arguments, Value * object = NULL);

                Function * getExternContext();
            }
        }
    }
}


#endif /* SRC_ASTOX_ECMAMETHODS_H_ */
