/*
 * SelfConfigurer.cpp
 *
 *  Created on: Jul 14, 2015
 *      Author: alexstf
 */

#include "Autoconf.h"


namespace astox {

    using namespace std;


    namespace SelfConfigure {


        static Properties * sysProps = new Properties();

        FoundableData::FoundableData(std::string p_name,
                std::string p_propname) {
            name = p_name;
            propname = p_propname;
        };

        void findByExec(std::string command, std::string propnamefile) {

            std::string response = ecma::methods::externs::fs_exec(command);
            response = astox::stringAlg::trim(response);
            if (!response.empty()) {
                astox_debug(__LINE__, __FILENAME__, "found %s at %s", propnamefile.c_str(), response.c_str());
                sysProps->add(propnamefile, response);
                sysProps->saveToFile(_sysPropsFileName);
            }
        };

        void start(bool checkSysFile, std::vector<std::string> dirlist) {
            FoundableData libsshi1("libssh2.h", "includes.libssh2.file");
            FoundableData libsshi2("libssh2_sftp.h", "includes.libssh2_sftp.file");

            findByExec("which sassc", "bin.witch.sassc.path");
            findByExec("which sass", "bin.witch.sass.path");
            findByExec("which fixjsstyle", "bin.witch.fixjsstyle.path");
        };

        void saveMakefile(std::string srcDir) {
            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::saveMakefile digest data for %s", _makefileName);
            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::saveMakefile %s: %s", _selfConfArg_srcDir, srcDir.c_str());



            std::map<std::string, std::string> includes;
            std::map<std::string, std::string> libs;




            sysProps->saveToFile(_sysPropsFileName);

            makefileData.clear();
            __libsLineContent.clear();
            __objsLineContent.clear();
            __compileLibsContent.clear();

            makefileData.append("PLATFORM=$(shell uname)\n");
            makefileData.append("COMPILER=g++\n");
            makefileData.append("CCOMPILER=gcc\n");
            makefileData.append("BUILD_DIR = build/\n");


            makefileData.append("CODE_SRC=");
            makefileData.append(srcDir);
            makefileData.append("\n");

            srcDir.append("astox");
            srcDir += (ASTOX_OS_SEPARATOR);

            makefileData.append("ASTOX_SRC=");
            makefileData.append(srcDir);
            makefileData.append("\n");


            makefileData.append("BIN_DIR=bin/\n");
            makefileData.append("LIBS=-lpthread -lm\n");
            makefileData.append("LIB_PATHS=\n");
            makefileData.append("INCLUDES=\n");
            makefileData.append("DEFINES=\n");
            makefileData.append("OTHER_FLAGS=\n");
            makefileData.append("\n\n");

            makefileData.append("#external libs and flags\n");
            makefileData.append("\n\n");


#ifdef ASTOX_OS_MAC
            makefileData.append("OTHER_FLAGS :=$(OTHER_FLAGS) -framework Cocoa -x objective-c\n");
            makefileData.append("DEFINES :=$(DEFINES) -DASTX_ENABLE_OBJECTIVECPP\n");
#else
#endif
            //APPEND TO VARS IN HERE:

            if (sysProps->hasKey("includes.libssh21") && sysProps->hasKey("libs.libssh2")) {

                if (sysProps->hasKey("libs.libssh2.static")) {

                    string includeFldr = sysProps->getKey("includes.libssh21");
                    if (includes.find(includeFldr) == includes.end()) {
                        makefileData.append("INCLUDES :=$(INCLUDES) -I");
                        makefileData.append(includeFldr);
                        makefileData.append("\n");
                        includes[includeFldr] = includeFldr;
                    }

                    makefileData.append("LIBS :=$(LIBS) -lssh2");
                    makefileData.append("\n");

                    string libPath = sysProps->getKey("libs.libssh2");
                    if (libs.find(libPath) == libs.end()) {
                        makefileData.append("LIB_PATHS :=$(LIB_PATHS) -L");
                        makefileData.append(libPath);
                        makefileData.append("\n");
                        libs[libPath] = libPath;
                    }


                    makefileData.append("DEFINES :=$(DEFINES) -DASTX_ENABLE_LIBSSH2");
                    makefileData.append("\n");

                }
            }


            //SASS
            if (sysProps->hasKey("includes.libsass1") && sysProps->hasKey("includes.libsass2")) {

                if (sysProps->hasKey("libs.libsass.static")) {

                    string includeFldr = sysProps->getKey("includes.libsass1");

                    if (includes.find(includeFldr) == includes.end()) {
                        makefileData.append("INCLUDES :=$(INCLUDES) -I");
                        makefileData.append(includeFldr);
                        makefileData.append("\n");
                        includes[includeFldr] = includeFldr;
                    }



                    makefileData.append("LIBS :=$(LIBS) -lsass -ldl");
                    makefileData.append("\n");




                    string libPath = sysProps->getKey("libs.libsass");
                    if (libs.find(libPath) == libs.end()) {
                        makefileData.append("LIB_PATHS :=$(LIB_PATHS) -L");
                        makefileData.append(libPath);
                        makefileData.append("\n");
                        libs[libPath] = libPath;
                    }


                    makefileData.append("DEFINES :=$(DEFINES) -DASTX_ENABLE_SASS");
                    makefileData.append("\n");

                    makefileData.append("OTHER_FLAGS :=$(OTHER_FLAGS) -Wall -O2 -std=c++0x -fPIC");
                    makefileData.append("\n");
                }

            }

            makefileData.append("#end external libs and flags\n");
            makefileData.append("\n\n");

            makefileData.append("OBJ_SETTINGS= $(DEFINES) $(INCLUDES) -c\n");
            makefileData.append("\n\n");

            //iterate src folder
            EventDispatcher::addEventListener(Event::ON_FS_FTWR, new Function(makeFileCallback));
            ecma::methods::externs::fs_ftwr(String::New(srcDir), new Object());
            EventDispatcher::removeEventListener(Event::ON_FS_FTWR);

            makefileData.append("ASTOX_OBJS=");
            makefileData.append(__objsLineContent);
            makefileData.append("\n\n");

            //		//tags:
            makefileData.append("all: Libs astox\n");
            makefileData.append("\n");
            //
            //
            //

            makefileData.append("astox:\n\t$(COMPILER) $(CODE_SRC)ecma-sample.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astox\n");
            makefileData.append("\n");
            makefileData.append("astxSelfConfigure:\n\t$(COMPILER) $(CODE_SRC)astxSelfConfigure.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astxSelfConfigure\n\n");
            makefileData.append("astxUnitTests:\n\t$(COMPILER) $(CODE_SRC)astxSelfConfigure.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astxUnitTests\n");

            makefileData.append("\nLibs: ");
            makefileData.append(__libsLineContent);
            makefileData.append("\n\n");



            makefileData.append(__compileLibsContent);




            astox::stringAlg::saveStringToFile(makefileData, _makefileName);

        };

        void scan(bool checkSysFile, std::vector<std::string> dirlist) {
            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::scan %s started, this may take a while", __FILENAME__);
            bool sysFileExist = false;

            for (unsigned int i = 0; i < dirlist.size(); i++) {
                astox_debug(__LINE__, __FILENAME__, "SelfConfigure::scan dirlist[%i]=%s", i, dirlist.at(i).c_str());
            }

            if (checkSysFile) {
                Path p(_sysPropsFileName);
                FileInfo * info = new FileInfo(&p);
                sysFileExist = info->exist();
            } else {
                astox_debug(__LINE__, __FILENAME__, "SelfConfigure::scan without checking existing %s", _sysPropsFileName);
            }


            if (checkSysFile && sysFileExist) {
                sysProps->loadFile(_sysPropsFileName);
                astox_debug(__LINE__, __FILENAME__, "SelfConfigure::scan sysProps file found");
            } else {
                EventDispatcher::addEventListener(Event::ON_FS_FTWR, new Function(selfConfigureCallback));

                for (int i = 0; i < dirlist.size(); i++) {
                    ecma::methods::externs::fs_ftwr(String::New(dirlist.at(i)), Object::npos);
                }



                EventDispatcher::removeEventListener(Event::ON_FS_FTWR);

            }
        }

        std::string getParentDir(std::string input) {
            return "";
        };

        Value * makeFileCallback(Value * object) {
            Object * data = Object::cast(object);
            std::string extension = data->getValue("extension")->toString();
            std::string fileName = data->getValue("filename")->toString();
            std::string filePath = data->getValue("path")->toString();
            std::string fileNameNoExt = data->getValue("fileNameNoExt")->toString();

#ifdef ASTOX_OS_MAC
            if (fileName == "MinGUI.mm") {
                //add to Libs:
                __libsLineContent.append(fileNameNoExt);
                __libsLineContent.append(" ");

                __objsLineContent.append("$(BUILD_DIR)");
                __objsLineContent.append(fileNameNoExt);
                __objsLineContent.append(".o ");

                //add to final compiler:
                std::string line = fileNameNoExt;
                line.append(":\n\t");

                std::string compline = ("$(COMPILER) ");
                compline.append(filePath);
                compline.append(" $(OBJ_SETTINGS) -o $(BUILD_DIR)");
                compline.append(fileNameNoExt);
                compline.append(".o");
                line.append(compline);
                line.append("\n\n");
    #ifdef ASTX_DBG_SELF_CONFIGURE
                astox_debug(__LINE__, __FILENAME__, "SelfConfigure::makeFileCallback %s ", compline.c_str());
    #endif
                __compileLibsContent.append(line);
                return Value::npos;
            }
#endif
            if (extension == "cpp" || extension == "c") {
#ifdef ASTOX_OS_MAC
                if (fileNameNoExt == "MinGUI") {
                    return Value::npos;
                }
#endif
                //add to Libs:
                __libsLineContent.append(fileNameNoExt);
                __libsLineContent.append(" ");

                __objsLineContent.append("$(BUILD_DIR)");
                __objsLineContent.append(fileNameNoExt);
                __objsLineContent.append(".o ");

                //add to final compiler:
                std::string line = fileNameNoExt;
                line.append(":\n\t");

                std::string compline = ("$(COMPILER) ");
                compline.append(filePath);
                compline.append(" $(OBJ_SETTINGS) -o $(BUILD_DIR)");
                compline.append(fileNameNoExt);
                compline.append(".o");
                line.append(compline);
                line.append("\n\n");
#ifdef ASTX_DBG_SELF_CONFIGURE
                astox_debug(__LINE__, __FILENAME__, "SelfConfigure::makeFileCallback %s ", compline.c_str());
#endif
                __compileLibsContent.append(line);
            }

            return Value::npos;
        };

        Value * selfConfigureCallback(Value * object) {


            Object * data = Object::cast(object);

            std::string extension = data->getValue("extension")->toString();
            bool isDirectory = data->getValue("isDirectory")->toBoolean();
            std::string fileName = data->getValue("filename")->toString();
            std::string filePath = data->getValue("path")->toString();

#ifdef ASTOX_OS_LINUX
            //cout << lang::toJSON(object) << endl;
            if (filePath == "/usr/bin/firefox") {
                cout << " found browser" << " at " << filePath << endl;
                sysProps->add(_sysPropsBrowserKey, filePath);
                sysProps->saveToFile(_sysPropsFileName);
            }

            if (filePath == "/usr/bin/kwrite") {
                cout << " found editor" << " at " << filePath << endl;
                sysProps->add(_sysPropsEditorKey, filePath);
                sysProps->saveToFile(_sysPropsFileName);
            }
#endif

#ifdef ASTOX_OS_MAC
            if (extension == "app") {

                std::string expectedExecName;
                std::string keyName;
                if (fileName == "Google Chrome.app") {
    #ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found %s", _sysPropsBrowserKey);
    #endif
                    expectedExecName = "Google Chrome";
                    keyName = _sysPropsBrowserKey;
                }
                else if (fileName == "Safari.app") {
    #ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found %s", _sysPropsBrowserKey);
    #endif
                    expectedExecName = "Safari";
                    keyName = _sysPropsBrowserKey;
                }
                else if (fileName == "Firefox.app") {
    #ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found %s", _sysPropsBrowserKey);
    #endif
                    expectedExecName = "Firefox";
                    keyName = _sysPropsBrowserKey;
                }
                else if (fileName == "TextEdit.app") {
    #ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found %s", _sysPropsEditorKey);
    #endif
                    expectedExecName = "TextEdit";
                    keyName = _sysPropsEditorKey;
                }

                std::string execPath = data->getValue("path")->toString();
                execPath.append("/Contents/MacOS/");
                execPath.append(expectedExecName);
                Path p(execPath);
                FileInfo f(&p);

                if (f.exist() && !f.isDirectory()) {
    #ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback add %s = %s", keyName.c_str(), execPath.c_str());
    #endif
                    sysProps->add(keyName, execPath);
                    sysProps->saveToFile(_sysPropsFileName);
                }
            }

            else
#endif
                if (extension == "h") {
                std::string parentDir = filePath;
                astox::util::replace(parentDir, fileName, "");
                std::vector<std::string> tokens = astox::stringAlg::splitByChar(filePath, '/');

                if (tokens.size() > 1) {
                    if (fileName == "sass2scss.h") {
                        if (tokens.at(tokens.size() - 2) == "libsass") {
#ifdef ASTX_DBG_SELF_CONFIGURE
                            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found includes.libsass1 (sass2scss.h) at %s with parent dir name %s", filePath.c_str(), tokens.at(tokens.size() - 2).c_str());
#endif
                            astox::util::replace(parentDir, "libsass/", "");
                            sysProps->add("includes.libsass1", parentDir);
                        }
                    }

                    if (fileName == "sass_context.h") {
                        if (tokens.at(tokens.size() - 2) == "libsass") {
#ifdef ASTX_DBG_SELF_CONFIGURE
                            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found includes.libsass2 (sass2scss.h) at %s with parent dir name %s", filePath.c_str(), tokens.at(tokens.size() - 2).c_str());
#endif
                            astox::util::replace(parentDir, "libsass/", "");
                            sysProps->add("includes.libsass2", parentDir);
                        }
                    }


                    if (fileName == "libssh2.h") {
#ifdef ASTX_DBG_SELF_CONFIGURE
                        astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found includes.libssh21 (libssh2.h) at %s", filePath.c_str());
#endif
                        sysProps->add("includes.libssh21", parentDir);
                    }

                    if (fileName == "libssh2_sftp.h") {
                        astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found includes.libssh22 (libssh2_sftp.h) at %s", filePath.c_str());
                        sysProps->add("includes.libssh22", parentDir);
                    }


                    if (fileName == "curl.h") {
                        if (tokens.at(tokens.size() - 2) == "curl") {
                            astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found includes.libcurl (curl.h) at %s with parent dir name %s", filePath.c_str(), tokens.at(tokens.size() - 2).c_str());
                            astox::util::replace(parentDir, "curl/", "");
                            sysProps->add("includes.libcurl", parentDir);
                        }
                    }

                }

            }
            else if (extension == "a") {
                std::string parentDir = filePath;
                    astox::util::replace(parentDir, fileName, "");
                if (fileName == "libsass.a") {
                    sysProps->add("libs.libsass", parentDir);
                    sysProps->add("libs.libsass.static", filePath);
                    sysProps->saveToFile("sysProps.properties");
#ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libsass at %s", parentDir.c_str());
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libsass.static at %s", filePath.c_str());
#endif
                }


                if (fileName == "libssh2.a") {

                    sysProps->add("libs.libssh2", parentDir);
                    sysProps->add("libs.libssh2.static", filePath);
                    sysProps->saveToFile("sysProps.properties");
#ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libssh2 at %s", parentDir.c_str());
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libssh2.static at %s", filePath.c_str());
#endif
                }

                if (fileName == "libcurl.a") {
                    sysProps->add("libs.libcurl", parentDir);
                    sysProps->add("libs.libcurl.static", filePath);
                    sysProps->saveToFile("sysProps.properties");
#ifdef ASTX_DBG_SELF_CONFIGURE
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libcurl at %s", parentDir.c_str());
                    astox_debug(__LINE__, __FILENAME__, "SelfConfigure::selfConfigureCallback found libs.libcurl.static at %s", filePath.c_str());
#endif
                }

            }

            //libboost_chrono-mt.dylib
            //libboost_chrono-mt.a
            //libass.a



            return Value::npos;
        };
    }

}

