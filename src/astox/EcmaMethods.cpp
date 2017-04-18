/*
 * EcmaMethods.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: alexandru
 */



#include "EcmaMethods.h"
using namespace std;

namespace astox {
    namespace ecma {
        namespace methods {

            void getLinuxVersion(std::string pathToSearch, std::string linuxName, Object *& appendable) {
                Path p(pathToSearch);
                FileInfo * fileInfo = new FileInfo(&p);
                if (fileInfo->exist()) {
                    string text;
                    astox::stringAlg::appendStringFromFile(pathToSearch, text);
                    text = astox::stringAlg::replaceWhiteSpace(text);
                    appendable->addKey("linuxVersion", String::New(linuxName));
                    appendable->addKey("linuxVersionDetails", String::New(text));
                }

                delete fileInfo;
            };

            Value * astox_info(Value * arguments) {

                Object * response = new Object();
                Object * osDetails = new Object();
                response->addKey("osDetails", osDetails);

                Object * compilerDetails = new Object();
                response->addKey("compilerDetails", compilerDetails);

                Object * engineSupport = new Object();
                response->addKey("engineSupport", engineSupport);

                Array * otherFlags = new Array();
                response->addKey("otherFlags", otherFlags);

                Object * macros = new Object();
                response->addKey("macros", macros);


				#ifndef ASTOX_OS_WINDOWS
					getLinuxVersion("/etc/SuSE-release", "OpenSuse", osDetails);
				#endif // !ASTOX_OS_WINDOWS

               


                //			otherFlags->push(String::New("ASTX_DBG_FS"));
                //			otherFlags->push(String::New("ASTX_DBG_ECMA_GET_MEMBER"));
                //			otherFlags->push(String::New("ASTX_DBG_ECMA_OPERATE"));
                //			otherFlags->push(String::New("ASTX_DBG_ECMA_COMPARE"));


                #ifdef ASTX_ENABLE_OBJECTIVECPP
                    otherFlags->push(String::New("ASTX_ENABLE_OBJECTIVECPP"));
                #endif

                #ifdef ASTX_DBG_SSH2
                    otherFlags->push(String::New("ASTX_DBG_SSH2"));
                #endif

                #ifdef ASTX_DBG_FS
                    otherFlags->push(String::New("ASTX_DBG_FS"));
                #endif

                #ifdef ASTX_ENABLE_LIBSSH2
                    engineSupport->addKey("ssh2", Boolean::New(true));
                    otherFlags->push(String::New("ASTX_ENABLE_LIBSSH2"));
                #else
                    engineSupport->addKey("ssh2", Boolean::New(false));
                #endif

                #ifdef ASTX_ENABLE_SASS
                    engineSupport->addKey("sass", Boolean::New(true));
                    otherFlags->push(String::New("ASTX_ENABLE_SASS"));
                #else
                    engineSupport->addKey("sass", Boolean::New(false));
                #endif

                #ifdef ASTX_ENABLE_NODEJS
                    engineSupport->addKey("nodejs", Boolean::New(true));
                #else
                    engineSupport->addKey("nodejs", Boolean::New(false));
                #endif

                #ifdef ASTOX_REGEX_SUPPORT_C11
                    engineSupport->addKey("regex", new String("c++11"));
                #elif defined(ASTOX_REGEX_SUPPORT_POSIX)
                    engineSupport->addKey("regex", new String("posix"));
                #else
                    engineSupport->addKey("regex", new String("none"));
                #endif


                #ifdef __APPLE__
                    response->addKey("os", new String("Mac"));
                    osDetails->addKey("subOs", new String("__APPLE__"));
                    otherFlags->push(new String("__APPLE__"));
                #elif __MACH__
                    response->addKey("os", new String("Mac"));
                    osDetails->addKey("subOs", new String("__MACH__"));
                    otherFlags->push(new String("__MACH__"));
                #elif _WIN32
                    response->addKey("os", new String("Windows"));
                    osDetails->addKey("subOs", new String("_WIN32"));
                    otherFlags->push(new String("_WIN32"));
                #elif _WIN64
                    response->addKey("os", new String("Windows"));
                    osDetails->addKey("subOs", new String("_WIN64"));
                    otherFlags->push(new String("_WIN64"));
                #elif defined(_AIX)
                    response->addKey("os", new String("Aix"));
                #elif defined (unix)
                    response->addKey("os", new String("Unix"));
                    osDetails->addKey("subOs", new String("unix"));
                    otherFlags->push(new String("unix"));
                #elif defined (__unix)
                    response->addKey("os", new String("Unix"));
                    osDetails->addKey("subOs", new String("__unix"));
                    otherFlags->push(new String("__unix"));
                #elif defined (__unix__)
                    response->addKey("os", new String("Unix"));
                    osDetails->addKey("subOs", new String("__unix"));
                    otherFlags->push(new String("__unix");
                #elif defined (__linux__)
                    response->addKey("os", new String("Linux"));
                    osDetails->addKey("subOs", new String("__linux__"));
                    otherFlags->push(new String("__linux__");
                #elif defined (__FreeBSD__)
                    response->addKey("os", new String("FreeBSD"));
                    osDetails->addKey("subOs", new String("__FreeBSD__"));
                    otherFlags->push(new String("__FreeBSD__");
                #elif defined (linux)
                    response->addKey("os", new String("Linux"));
                    osDetails->addKey("subOs", new String("linux"));
                    otherFlags->push(new String("linux");
                #else
                    response->addKey("os", new String("Unknown"));
                #endif

                #if defined(__ANDROID_API__) || defined(ANDROID_API_LEVEL_H) || defined(ANDROID)
                    response->addKey("os", new String("Android"));
                #endif

                #ifdef __ANDROID_API__
                    macros->addKey("__ANDROID_API__", Number::New(__ANDROID_API__));
                #endif

                #ifdef _POSIX_ARG_MAX
                    macros->addKey("_POSIX_ARG_MAX", Number::New(_POSIX_ARG_MAX));
                #endif
                #ifdef _POSIX_CHILD_MAX
                    macros->addKey("_POSIX_CHILD_MAX", Number::New(_POSIX_CHILD_MAX));
                #endif
                #ifdef _POSIX_LINK_MAX
                    macros->addKey("_POSIX_LINK_MAX", Number::New(_POSIX_LINK_MAX));
                #endif

                #ifdef IPV6_ADDR_SCOPE_NODELOCAL
                    macros->addKey("IPV6_ADDR_SCOPE_NODELOCAL", Number::New(IPV6_ADDR_SCOPE_NODELOCAL));
                #endif


                #ifdef QT_VERSION
                    compilerDetails->addKey("qtVersion", Number::New(QT_VERSION));
                #endif

                //compiler name:
                #ifdef __CYGWIN__
                    compilerDetails->addKey("name", new String("cygwin"));
                #elif __CYGWIN32__
                    compilerDetails->addKey("name", new String("cygwin32"));
                #elif __MINGW32__
                    compilerDetails->addKey("name", new String("mingw32"));
                #elif __MINGW64__
                    compilerDetails->addKey("name", new String("mingw64"));
                #elif __GNUC__
                    compilerDetails->addKey("name", new String("gcc"));
                #elif GCC_VERSION
                    compilerDetails->addKey("name", new String("GCC_VERSION"));
                #elif _MSC_VER
                    compilerDetails->addKey("mscVer", Number::New(_MSC_VER));

                    #if __CLR_VER
                        compilerDetails->addKey("clrVer", Number::New(__CLR_VER));
                    #endif

                    #if (_MSC_VER == 1100)
                                compilerDetails->addKey("name", new String("Visual Studio"));
                    #elif (_MSC_VER == 1200)
                                compilerDetails->addKey("name", new String("Visual Studio"));
                    #elif (_MSC_VER == 1300)
                                compilerDetails->addKey("name", new String("Visual Studio"));
                    #elif (_MSC_VER == 1310)
                                compilerDetails->addKey("name", new String("Visual Studio 2003"));
                    #elif (_MSC_VER == 1400)
                                compilerDetails->addKey("name", new String("Visual Studio 2005"));
                    #elif (_MSC_VER == 1500)
                                compilerDetails->addKey("name", new String("Visual Studio 2008"));
                    #elif (_MSC_VER == 1600)
                                compilerDetails->addKey("name", new String("Visual Studio 2010"));
                    #elif (_MSC_VER == 1700)
                                compilerDetails->addKey("name", new String("Visual Studio 2012"));
                    #elif (_MSC_VER == 1800)
                                compilerDetails->addKey("name", new String("Visual Studio 2013"));
					#elif (_MSC_VER == 1900)
							compilerDetails->addKey("name", new String("Visual Studio 2015"));
					#elif (_MSC_VER == 1910)
                                compilerDetails->addKey("name", new String("Visual Studio 2017"));
                    #else
                                compilerDetails->addKey("name", new String("Visual Studio"));
                    #endif
                #else
                    compilerDetails->addKey("name", new String("unknown"));
                #endif

                #ifdef __DATE__
                    compilerDetails->addKey("date", new String(__DATE__));
                #endif

                #ifdef __TIME__
                    compilerDetails->addKey("time", new String(__TIME__));
                #endif

                #ifdef __TIMESTAMP__
                    compilerDetails->addKey("timestamp", new String(__TIMESTAMP__));
                #endif

                #ifdef __cplusplus
                    int n = __cplusplus;
                    compilerDetails->addKey("cppVersion", new String(n));
                #endif

                #ifdef __VERSION__
                    compilerDetails->addKey("version", new String(__VERSION__));
                #endif

                #ifdef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
                    osDetails->addKey("osxEnviroment", new String(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__));
                #endif


                #ifdef __amd64__
                    macros->addKey("__amd64__", Boolean::New(true));
                #endif
                #ifdef _M_AMD64
                    macros->addKey("_M_AMD64", Boolean::New(true));
                #endif
				#ifdef _M_X64 
					macros->addKey("_M_X64 ", Number::New(_M_X64));
				#endif
				#ifdef _M_ARM_FP  
					macros->addKey("_M_ARM_FP  ", Number::New(_M_ARM_FP));
				#endif
                #ifdef _M_ARM
                    osDetails->addKey("_M_ARM", Boolean::New(true));
                #endif
				#ifdef __cplusplus_winrt 
					macros->addKey("__cplusplus_winrt ", String::New(__cplusplus_winrt));
				#endif

				#ifdef __STDCPP_THREADS__  
					macros->addKey("__STDCPP_THREADS__  ", Number::New(__STDCPP_THREADS__));
				#endif

				#ifdef _NATIVE_WCHAR_T_DEFINED 
					macros->addKey("_NATIVE_WCHAR_T_DEFINED", Boolean::New(true));
				#endif // _NATIVE_WCHAR_T_DEFINED 

				#ifdef _MSC_VER  
					macros->addKey("_MSC_VER ", Number::New(_MSC_VER));
				#endif // _MSC_VER 

				#ifdef _MSC_FULL_VER   
					macros->addKey("_MSC_FULL_VER  ", Number::New(_MSC_FULL_VER));
				#endif // _MSC_VER 


                #ifdef __INT_MAX__
                    compilerDetails->addKey("maxInt", new String(__INT_MAX__));
                #elif _INTEGRAL_MAX_BITS
                    compilerDetails->addKey("intMaxBits", new String(_INTEGRAL_MAX_BITS));
                #else
                    compilerDetails->addKey("maxInt", new String("unknown"));
                #endif

                #if defined(__arm__)
                    #if defined(__ARM_ARCH_7A__)
                        #if defined(__ARM_NEON__)
                            #if defined(__ARM_PCS_VFP)
                                #define ABI "armeabi-v7a/NEON (hard-float)"
                            #else
                                #define ABI "armeabi-v7a/NEON"
                            #endif
                        #else
                            #if defined(__ARM_PCS_VFP)
                                #define ABI "armeabi-v7a (hard-float)"
                            #else
                                #define ABI "armeabi-v7a"
                            #endif
                        #endif
                    #else
                        #define ABI "armeabi"
                    #endif
                    #elif defined(__i386__)
                        #define ABI "x86"
                    #elif defined(__x86_64__)
                     #define ABI "x86_64"
                    #elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
                        #define ABI "mips64"
                    #elif defined(__mips__)
                        #define ABI "mips"
                    #elif defined(__aarch64__)
                        #define ABI "arm64-v8a"
                    #else
                        #define ABI "unknown"
                #endif

                osDetails->addKey("abi", String::New(ABI));


                return response;
            };

            Value * print(Value * arguments) {
                #ifdef ASTX_DBG_ECMA_JS_PRINT
                    astox_debug_start_with(__LINE__, __FILENAME__, "jsprint:: ");
                #endif
                if (arguments->getType() == ASTOX_VALUE_OBJECT) {
                    printf("%s", lang::toJSON(arguments, "", "").c_str());
                    astox_debug_end();
                    return Value::npos;
                }

                if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                    Array * args = Array::cast(arguments);
                    for (int i = 0; i < args->length(); i++) {
                        printf("%s ", args->itemAtIndex(i)->toString().c_str());
                    }
                    astox_debug_end();
                    return Value::npos;
                }
                printf("%s", arguments->toString().c_str());
                astox_debug_end();
                return Value::npos;
            };

            void map_prototype(Value * prototypeObject, Value * response) {
                if (prototypeObject->getType() == ASTOX_VALUE_OBJECT) {
                    std::map<std::string, Value *>::iterator it;
                    Object * crnt = Object::cast(prototypeObject);
                    //				cout <<  "map protoprop " << lang::toJSON(crnt) << endl;
                    for (it = crnt->getObjectList()->begin(); it != crnt->getObjectList()->end(); ++it) {
                        cout << "protoprop " << lang::toJSON(it->second) << endl;
                        Function::cast(it->second)->protoype = response;
                        response->addKey(it->first, it->second);
                    }
                }
            };

            Value * String_constructor(Value * arguments, Value * prototypeObject) {
                String * response = new String(arguments->toString());
                response->setNumCalls(1);
                map_prototype(prototypeObject, response);
                return response;
            }

            Value * Object_constructor(Value * arguments, Value * prototypeObject) {
                Object * response = new Object();
                map_prototype(prototypeObject, response);
                return response;
            };

            Value * Boolean_constructor(Value * arguments, Value * prototypeObject) {
                Boolean * response = Boolean::New(arguments->toBoolean());
                response->setNumCalls(1);
                map_prototype(prototypeObject, response);
                return response;
            };

            Value * Number_constructor(Value * arguments, Value * prototypeObject) {
                Number * response = Number::New(arguments->toDouble());
                response->setNumCalls(1);
                map_prototype(prototypeObject, response);
                return response;
            };

            Value * Array_constructor(Value * arguments, Value * prototypeObject) {
                //			cout <<  "------> array_create called with args: " << lang::toJSON(arguments) << endl;
                //			cout <<  "------>           and prototypeObject: " << lang::toJSON(prototypeObject, "", "") << endl;
                Array * response;
                if (arguments->isNumber()) {
                    response = new Array(arguments->toInt());
                    response->setNumCalls(1);
                }
                else if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                    response = new Array(Array::cast(arguments));
                }
                else {
                    cout << " create new array" << endl;
                    response = new Array();
                }

                map_prototype(prototypeObject, response);
                //response->setNumCalls(1);
                return response;
            };

            Value * Date_constructor(Value * arguments, Value * prototypeObject) {
                return new Date();
            };

            Value * Require_constructor(Value * arguments, Value * prototypeObject) {
                astox_debug(__LINE__, __FILENAME__, "Require_constructor(%s)", arguments->toString().c_str());
                return Value::npos;
            };

            Value * array_reverse(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->reverse();
                }
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->reverse();
                }
                return Value::npos;
            }

            Value * array_sort(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    if (arguments->getType() == ASTOX_VALUE_FUNCTION) {
                        Function * sortm = Function::cast(arguments);
                        return Array::cast(object)->sort(sortm);
                    }
                    return Array::cast(object)->sort(NULL);
                }

                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->sort();
                }
                return Value::npos;
            }

            Value * array_forEach(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    if (arguments->getType() == ASTOX_VALUE_FUNCTION) {
                        Function * foreachm = Function::cast(arguments);
                        Array::cast(object)->forEach(foreachm);
                    }
                }
                return Value::npos;
            }

            Value * array_join(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {

                    if (arguments->getType() == ASTOX_VALUE_UNDEFINED) {
                        return Array::cast(object)->join(",");
                    }
                    //				cout<< " array_join ARGUMENTS: " << lang::toJSON(arguments) << endl;
                    return Array::cast(object)->join(arguments->toString());

                }
                return Value::npos;
            }

            Value * array_push(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    Array::cast(object)->push(arguments);
                    return arguments;
                }
                return Value::npos;
            };

            Value * array_shift(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->shift();
                }
                return Value::npos;
            };

            Value * array_pop(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->pop();
                }
                return Value::npos;
            };

            Value * array_unshift(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->unshift(arguments);
                }

                return Value::npos;
            };

            Value * array_concat(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY && arguments->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->concat(Array::cast(arguments));
                }

                return Value::npos;
            };

            Value * array_slice(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->slice(Array::cast(arguments));
                }
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->slice(arguments);
                }

                return Value::npos;
            };

            Value * array_splice(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->splice(Array::cast(arguments));
                }

                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->substr(arguments);
                }

                return Value::npos;
            };

            Value * array_indexOf(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->indexOf(arguments->toString());
                }
                return Value::npos;
            };

            Value * array_lastIndexOf(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->lastIndexOf(arguments->toString());
                }
                return Value::npos;
            };

            Value * string_charAt(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return object->itemAtIndex(arguments->toInt());
                }
                return Value::npos;
            };

            Value * string_trim(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->trim();
                }
                return Value::npos;
            };

            Value * string_killWhiteSpace(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->killWhiteSpace();
                }
                return Value::npos;
            };

            Value * string_split(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->split(arguments);
                }
                return Value::npos;
            };

            Value * string_splitByWhiteSpace(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->splitByWhiteSpace(arguments);
                }
                return Value::npos;
            };

            Value * string_splitByChar(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->splitByChar(arguments);
                }
                return Value::npos;
            };

            Value * string_match(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->match(arguments);
                }
                return Value::npos;
            };

            Value * string_replace(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->replace(arguments);
                }
                return Value::npos;
            };

            Value * proto_toJSON(Value * arguments, Value * object) {
                return new String(lang::toJSON(object, "", ""));
            }

            Value * proto_toString(Value * arguments, Value * object) {
                if (object->getType() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->join();
                }
                return new String(lang::toJSON(object, "", ""));
            }

            Value * setInterval(Value * arguments, Value * object) {
                if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                    Array * args = Array::cast(arguments);
                    Function * callable = Function::cast(args->itemAtIndex(0));
                    callable->call(Value::npos);
#ifdef ASTOX_OS_WINDOWS
                    Sleep(1000);
#else
                    sleep(1);
#endif
                    setInterval(arguments, object);
                }
                return Value::npos;
            };


            namespace externs {

                Function * getExternContext() {
                    Function * externContext = Function::getEcmaContext();
                    Function * minguiConstructor = new Function(ecma::methods::externs::mingui_notification, new Object());
                    externContext->addKey("mingui_notification", minguiConstructor);

                    Function * minguiDialogConstructor = new Function(ecma::methods::externs::mingui_dialog, new Object());
                    externContext->addKey("mingui_dialog", minguiConstructor);

                    Function * sassCompileConstructor = new Function(ecma::methods::externs::extapi_sassCompile, new Object());
                    externContext->addKey("extapi_sassCompile", sassCompileConstructor);

                    Function * fsjconfCompileConstructor = new Function(ecma::methods::externs::fs_joinFilesWithConfig, new Object());
                    externContext->addKey("fs_joinFilesWithConfig", fsjconfCompileConstructor);

                    Function * fs_ftwrConstructor = new Function(ecma::methods::externs::fs_ftwr, new Object());
                    externContext->addKey("fs_ftwr", fs_ftwrConstructor);

                    Function * fs_copyConstructor = new Function(ecma::methods::externs::fs_copyFile, new Object());
                    externContext->addKey("fs_copyFile", fs_copyConstructor);

                    externContext->addKey("ssh2_connect", new Function(ecma::methods::externs::ssh2_connect, new Object()));
                    externContext->addKey("ssh2_scpWrite", new Function(ecma::methods::externs::ssh2_scpWrite, new Object()));

                    externContext->addKey("event_addEventListener", new Function(ecma::methods::externs::event_addEventListener, new Object()));
                    externContext->addKey("fs_fileWatchAdd", new Function(ecma::methods::externs::fs_fileWatchAdd, new Object()));
                    externContext->addKey("fs_fileWatchRemove", new Function(ecma::methods::externs::fs_fileWatchRemove, new Object()));
                    externContext->addKey("fs_fileWatchStart", new Function(ecma::methods::externs::fs_fileWatchStart, new Object()));

                    Object * console = new Object();
                    console->addKey("log", new Function(ecma::methods::externs::console_log, new Object()));
                    externContext->addKey("console", console);

                    return externContext;
                };

                std::string fs_exec(std::string cmd) {
#ifdef ASTOX_OS_WINDOWS
                    FILE* pipe = _popen(cmd.c_str(), "r");
#else
                    FILE* pipe = popen(cmd.c_str(), "r");
#endif
                    if (!pipe) {
                        return "ERROR";
                    };
                    char buffer[128];
                    std::string result = "";
                    while (!feof(pipe)) {
                        if (fgets(buffer, 128, pipe) != NULL) {
                            result += buffer;
                        }
                    }
#ifdef ASTOX_OS_WINDOWS
                    _pclose(pipe);
#else
                    pclose(pipe);
#endif
                    return result;
                }

                Value * console_log(Value * arguments, Value * object) {

                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        for (int i = 0; i < args->length(); i++) {
                            printf("%s ", args->itemAtIndex(i)->toString().c_str());
                        }
                        printf("\n");
                    } else {
                        printf("%s\n", arguments->toString().c_str());
                    }
                    return Value::npos;
                };

                Value * fs_exec(Value * arguments, Value * object) {
                    return new String();
                };

                Value * fs_fileWatchAdd(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_STRING) {
                        Path p(arguments->toString());
                        FileInfo info(&p);
                        fs_filesToWatch[arguments->toString()] = info.getLastSavedTime();
                    }
                    if (arguments->getType() == ASTOX_VALUE_OBJECT) {
                        string s = Object::cast(arguments)->getValue("path")->toString();
                        Path p(s);
                        FileInfo info(&p);
                        fs_filesToWatch[arguments->toString()] = info.getLastSavedTime();
                    }
                    return Value::npos;
                };

                Value * fs_fileWatchRemove(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_STRING) {
                        std::map<std::string, int>::iterator it = fs_filesToWatch.find(arguments->toString());
                        if (it != fs_filesToWatch.end()) {
                            fs_filesToWatch.erase(it);
                        };
                    }
                    return Value::npos;
                };

                Value * fs_fileWatchStart(Value * arguments, Value * object) {
                    FileSystem::createWatcher((char *) arguments->toString().c_str());
                    //					if(fs_filesToWatch.size() > 0) {
                    //
                    //						std::map<std::string, int>::iterator it;
                    //						printf(" WATCH %i files\n", fs_filesToWatch.size());
                    //						for(it = fs_filesToWatch.begin(); it != fs_filesToWatch.end(); ++it) {
                    //							FileInfo * info = new FileInfo(it->first);
                    //							EventDispatcher::dispatch(Event::ON_FS_FILE_WATCH, info->toObject());
                    //							if( info->getLastSavedTime() != it->second ) {
                    //							//	printf(" CHANGED");
                    //								EventDispatcher::dispatch(Event::ON_FS_FILE_WATCH_CHANGED, info->toObject());
                    //							}
                    //							fs_filesToWatch[it->first] = info->getLastSavedTime();
                    //							delete info;
                    //						}
                    //
                    //						fs_fileWatchStart();
                    //					}
                    //#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                    //					else {
                    //						astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::fs_fileWatchStart stopped...");
                    //					}
                    //#endif
                    return Value::npos;
                };

                Object * fs_infoToObj(FileInfo *& info) {
                    Object * data = new Object();
                    data->addKey("path", new String(info->getPath()->toString()));
                    data->addKey("filename", new String(info->getPath()->getFileName()));
                    data->addKey("extension", new String(info->getPath()->getExtension()));
                    data->addKey("lastModifiedTime", Number::New(info->getLastSavedTime()));
                    data->addKey("isDirectory", Boolean::New(info->isDirectory()));
                    data->addKey("isDot", Boolean::New(info->getPath()->isDot()));
                    //					cout << info->getLastSavedTime() << endl;
                    Array * tokens = new Array();
                    std::vector<std::string> tokensstr = info->getPath()->getTokens(); //Path::tokenizePath(info->getPath()->toString(), info->isDirectory(), true);
                    for (unsigned int i = 0; i < tokensstr.size(); i++) {
                        tokens->push(new String(tokensstr.at(i)));
                    }

                    data->addKey("tokens", tokens);
                    return data;
                };

                Value * fs_ftwr(Value * arguments, Value * object) {
                    std::string path;
                    std::string method = "unset";


                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        path = arguments->itemAtIndex(0)->toString();
#ifndef ASTOX_OS_WINDOWS
                        method = arguments->itemAtIndex(1)->toString();
#endif
                    }
                    if (arguments->getType() == ASTOX_VALUE_STRING) {
                        path = arguments->toString();
                    }
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::extern::fs_ftwr(%s, %s)", path.c_str(), method.c_str());
#endif

                    int mode = FileSystem::ftwrMethod(method);

                    int result = FileSystem::ftwr(path, mode);
                    return Value::npos;
                };

                Value * event_addEventListener(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        std::string eventName = args->itemAtIndex(0)->toString();
                        Function * callback = Function::cast(args->itemAtIndex(1));
                        EventDispatcher::addEventListener(eventName, callback);
                    }
                    return Value::npos;
                };



                ssh2_data ssh2_globalData;

                Value * ssh2_connect(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        string username = args->itemAtIndex(0)->toString();
                        string password = args->itemAtIndex(1)->toString();
                        string host = args->itemAtIndex(2)->toString();
                        ssh2_globalData.username = username.c_str();
                        ssh2_globalData.password = password.c_str();
                        ssh2_globalData.servername = host.c_str();
                        ssh2_scpConnect(&ssh2_globalData);
                    }
                    return Value::npos;

                };

                Value * ssh2_scpWrite(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        string localfile = args->itemAtIndex(0)->toString();
                        string remotefile = args->itemAtIndex(1)->toString();
                        ssh2_scpWrite(&ssh2_globalData, localfile.c_str(), remotefile.c_str());
                    }
                    return Value::npos;
                };

                Value * mingui_dialog(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
//                        Array * args = Array::cast(arguments);
//                        string title = "";
//                        string message = "";
//                        int flag = DG_DEFAULT;
//                        int answ = DGANSW_OK;
//
//                        if (args->length() > 0) {
//                            title = args->itemAtIndex(0)->toString();
//                            if (args->length() > 1) {
//                                message = args->itemAtIndex(1)->toString();
//                            }
//                        }
//#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
//                        astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::mingui_dialog (%s) (%s) ", title.c_str(), message.c_str());
//#endif
//                        MINGUI_Dialog(title.c_str(), message.c_str(), NULL, flag, answ);
                    }

                    return Value::npos;
                };

                Value * fs_copyFile(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() > 1) {
                            string s1 = args->itemAtIndex(0)->toString();
                            string s2 = args->itemAtIndex(1)->toString();
                            FileSystem::copyFile(s1, s2);
                        }
                    }
                    return Value::npos;
                };

                Value * mingui_notification(Value * arguments, Value * object) {
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() > 1) {
                            string s1 = args->itemAtIndex(0)->toString();
                            string s2 = args->itemAtIndex(1)->toString();
                            const char * input = s1.c_str();
                            const char * output = s2.c_str();
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                            astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::mingui_notification (%s) (%s)", input, output);
#endif
//                            MINGUI_Notification(input, output, "", 20);
                        }
                    }
                    return Value::npos;
                };

                Value * extapi_sassCompile(Value * arguments, Value * object) {
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::extapi_sassCompile (%s)", lang::toJSON(arguments, "", "").c_str());
#endif
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() == 2) {
                            string s1 = args->itemAtIndex(0)->toString();
                            string s2 = args->itemAtIndex(1)->toString();
                            const char * input = s1.c_str();
                            const char * output = s2.c_str();
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                            astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::extapi_sassCompile (%s) to (%s)", input, output);
#endif
                            extapi_sass_compileFile(s1.c_str(), s2.c_str());
                        }
                    }
                    return Value::npos;
                };

                Value * fs_joinFiles(Value * arguments, Value * object) {
                    Array * response = new Array();
                    if (arguments->getType() == ASTOX_VALUE_OBJECT) {
                        Object * args = Object::cast(arguments);
                        std::string inputRoot = args->getValue("inputRoot")->toString();
                        std::string outputRoot = args->getValue("outputRoot")->toString();
                        std::string beforeSeparator;
                        std::string afterSeparator = "\n";

                        if (args->hasKey("beforeSeparator")) {
                            beforeSeparator = args->getValue("beforeSeparator")->toString();
                        }

                        Array * joinRules = args->getArray("rules");


                        for (int i = 0; i < joinRules->length(); i++) {

                            if (joinRules->itemAtIndex(i)->hasKey("in")) {
                                Array * inputs = joinRules->itemAtIndex(i)->getArray("in");
                                string content;

                                for (int j = 0; j < inputs->length(); j++) {
                                    string inputfile = inputs->itemAtIndex(j)->toString();
                                    string inputComplete = inputRoot;
                                    inputComplete.append(inputfile);

                                    if (!beforeSeparator.empty()) {
                                        astox::util::replace(beforeSeparator, "${file}", inputComplete);
                                        astox::util::replace(beforeSeparator, "${index}", astox::stringAlg::intToString(j));
                                        astox::util::replace(beforeSeparator, "${timestamp}", astox::stringAlg::currentDateTime());
                                        content.append(beforeSeparator + "\n");
                                    }
                                    astox::stringAlg::appendStringFromFile(inputComplete, content);
                                    content.append(afterSeparator);
                                }

                                if (joinRules->itemAtIndex(i)->hasKey("out")) {
                                    string out = joinRules->itemAtIndex(i)->getValue("out")->toString();
                                    string outcomplete = outputRoot;
                                    outcomplete.append(out);
                                    astox::stringAlg::saveStringToFile(content, outcomplete);
                                    response->push(new String(outcomplete));
                                }
                            }
                        }


                    }
                    return response;
                };

                Value * fs_joinFilesWithConfig(Value * arguments, Value * object) {
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::fs_joinFilesWithConfig (%s)", lang::toJSON(arguments, "", "").c_str());
#endif
                    if (arguments->getType() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);

                        std::string afterSeparator;
                        Object * joinFileArgs = new Object();

                        if (args->length() > 2) {
                            joinFileArgs->addKey("inputRoot", args->itemAtIndex(0));
                            joinFileArgs->addKey("outputRoot", args->itemAtIndex(1));
                            std::string configPath = args->itemAtIndex(2)->toString();

                            string fileContent;
                            astox::stringAlg::appendStringFromFile(configPath, fileContent);

                            joinFileArgs->addKey("rules", astox::ecma::fromStr(fileContent, Function::npos));

                            if (args->length() > 3) {
                                joinFileArgs->addKey("beforeSeparator", args->itemAtIndex(3));
                            }
                            return fs_joinFiles(joinFileArgs, object);
                        }
                    }
                    return Value::npos;
                }
            }

        }
    }
}

