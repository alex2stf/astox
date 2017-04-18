/*
 * SelfConfigurer.h
 *
 *  Created on: Jul 14, 2015
 *      Author: alexstf
 */

#ifndef SRC_ASTOX_SELFCONFIGURER_H_
    #define SRC_ASTOX_SELFCONFIGURER_H_

    #include "ExtApi.h"
    #include "MinGUI.h"
    #include "Thread.h"
    #include "EcmaMethods.h"
    #include "Properties.h"
    #include <iostream>


namespace astox {


    #define _sysPropsFileName "astox_sys_props.properties"
    #define _sysPropsBrowserKey "browser"
    #define _sysPropsEditorKey "editor"
    #define _makefileName "MakefileSelfGen.am"
    #define _selfConfArg_srcDir "--src-dir"

    namespace SelfConfigure {

        class FoundableData {
        public:
            FoundableData(
                    std::string p_name,
                    std::string p_propname);
        private:
            std::string name;
            std::string propname;
        };

        static std::string makefileData;
        static std::string __libsLineContent;
        static std::string __objsLineContent;
        static std::string __compileLibsContent;

        Value * selfConfigureCallback(Value * object);
        Value * makeFileCallback(Value * object);
        void findByExec(std::string command, std::string propnamefile);


        void start(bool checkSysFile, std::vector<std::string> dirlist);
        void scan(bool checkSysFile, std::vector<std::string> dirlist);

        //		std::vector<FileInfo *> scanForAstoxSrc(std::string path);
        std::string getParentDir(std::string input);
        void saveMakefile(std::string srcDir);
    }
}



#endif /* SRC_ASTOX_SELFCONFIGURER_H_ */
