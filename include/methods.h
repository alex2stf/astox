/*
 * EcmaMethods.h
 *
 *  Created on: Jul 21, 2015
 *      Author: alexstf
 */

#ifndef SRC_ASTOX_ECMAMETHODS_H_
    #define SRC_ASTOX_ECMAMETHODS_H_

   // #include "ExtApi.h"
    #include "mingui.h"
    #include "filesystem.h"


namespace astox {
    namespace ecma {
        namespace methods {
        			Value * print(Value * arguments, const lexic &lxc);
                    Value * astox_info(Value * arguments, const lexic &lxc);
                    void map_prototype(Value * prototypeObject, Value * response);

                    //constructors
                    Value * Array_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Object_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * String_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Number_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Boolean_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Date_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Error_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);
                    Value * Require_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc);

                    //array
                    Value * array_reverse(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_join(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_sort(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_push(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_shift(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_pop(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_unshift(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_concat(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_forEach(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_splice(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_slice(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_indexOf(Value * arguments, Value * object, const lexic &lxc);
                    Value * array_lastIndexOf(Value * arguments, Value * object, const lexic &lxc);

                    //string
                    Value * string_charAt(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_trim(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_killWhiteSpace(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_split(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_splitByWhiteSpace(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_splitByChar(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_match(Value * arguments, Value * object, const lexic &lxc);
                    Value * string_replace(Value * arguments, Value * object, const lexic &lxc);

                    //regexp
                    Value * regexp_exec(Value * arguments, Value * object, const lexic &lxc);

                    //generic prototypes
                    Value * proto_toString(Value * arguments, Value * object, const lexic &lxc);
                    Value * proto_toJSON(Value * arguments, Value * object, const lexic &lxc);


                    Value * setInterval(Value * arguments, Value * object, const lexic &lxc);

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
                Value * fs_joinFiles(Value * arguments, Value * object,  const lexic &lxc);
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
