/*
 * EcmaMethods.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: alexandru
 */



#include "astox.h"

using namespace std;

namespace astox {
    namespace ecma {
        namespace methods {

            void getLinuxVersion(std::string pathToSearch, std::string linuxName, Object *& appendable, const lexic &l) {
                Path p(pathToSearch);
                FileInfo * fileInfo = new FileInfo(&p);
                if (fileInfo->exist()) {
                    string text;
                    astox::stringAlg::appendStringFromFile(pathToSearch, text);
                    text = astox::stringAlg::replaceWhiteSpace(text);
                    appendable->addKey("linuxVersion", String::New(linuxName, l));
                    appendable->addKey("linuxVersionDetails", String::New(text, l));
                }

                delete fileInfo;
            };

            Value * astox_info(Value * arguments, const lexic &lxc) {

                Object * response = Object::New(lxc);
                Object * osDetails = Object::New(lxc);
                response->addKey("osDetails", osDetails);

                Object * compilerDetails = Object::New(lxc);
                response->addKey("compilerDetails", compilerDetails);

                Object * engineSupport = Object::New(lxc);
                response->addKey("engineSupport", engineSupport);

                Array * otherFlags = Array::New(lxc); //TODO modify
                response->addKey("otherFlags", otherFlags);

                Object * macros = Object::New(lxc);
                response->addKey("macros", macros);


				#ifndef ASTOX_OS_WINDOWS
					getLinuxVersion("/etc/SuSE-release", "OpenSuse", osDetails, lxc);
				#endif // !ASTOX_OS_WINDOWS



                #ifdef ASTX_ENABLE_OBJECTIVECPP
                    otherFlags->push(String::New("ASTX_ENABLE_OBJECTIVECPP", lxc));
                #endif

                #ifdef ASTX_DBG_SSH2
                    otherFlags->push(String::New("ASTX_DBG_SSH2", lxc));
                #endif

                #ifdef ASTX_DBG_FS
                    otherFlags->push(String::New("ASTX_DBG_FS", lxc));
                #endif

                #ifdef ASTX_ENABLE_LIBSSH2
                    engineSupport->addKey("ssh2", Boolean::New(true, lxc));
                    otherFlags->push(String::New("ASTX_ENABLE_LIBSSH2", lxc));
                #else
                    engineSupport->addKey("ssh2", Boolean::New(false, lxc));
                #endif

                #ifdef ASTX_ENABLE_SASS
                    engineSupport->addKey("sass", Boolean::New(true, lxc));
                    otherFlags->push(String::New("ASTX_ENABLE_SASS", lxc));
                #else
                    engineSupport->addKey("sass", Boolean::New(false, lxc));
                #endif

                #ifdef ASTX_ENABLE_NODEJS
                    engineSupport->addKey("nodejs", Boolean::New(true, lxc));
                #else
                    engineSupport->addKey("nodejs", Boolean::New(false, lxc));
                #endif

                #ifdef ASTOX_REGEX_SUPPORT_C11
                    engineSupport->addKey("regex", String::New("c++11", lxc));
                #elif defined(ASTOX_REGEX_SUPPORT_POSIX)
                    engineSupport->addKey("regex", String::New("posix", lxc));
                #else
                    engineSupport->addKey("regex", String::New("none", lxc));
                #endif


                #ifdef __APPLE__
                    response->addKey("os", String::New("Mac", lxc));
                    osDetails->addKey("subOs", String::New("__APPLE__", lxc));
                    otherFlags->push(String::New("__APPLE__", lxc));
                #elif __MACH__
                    response->addKey("os", String::New("Mac", lxc));
                    osDetails->addKey("subOs", String::New("__MACH__", lxc));
                    otherFlags->push(String::New("__MACH__", lxc));
                #elif _WIN32
                    response->addKey("os", String::New("Windows", lxc));
                    osDetails->addKey("subOs", String::New("_WIN32", lxc));
                    otherFlags->push(String::New("_WIN32", lxc));
                #elif _WIN64
                    response->addKey("os", String::New("Windows", lxc));
                    osDetails->addKey("subOs", String::New("_WIN64", lxc));
                    otherFlags->push(String::New("_WIN64", lxc));
                #elif defined(_AIX)
                    response->addKey("os", String::New("Aix", lxc));
                #elif defined (unix)
                    response->addKey("os", String::New("Unix", lxc));
                    osDetails->addKey("subOs", String::New("unix", lxc));
                    otherFlags->push(String::New("unix", lxc));
                #elif defined (__unix)
                    response->addKey("os", String::New("Unix", lxc));
                    osDetails->addKey("subOs", String::New("__unix", lxc));
                    otherFlags->push(String::New("__unix", lxc));
                #elif defined (__unix__)
                    response->addKey("os", String::New("Unix", lxc));
                    osDetails->addKey("subOs", String::New("__unix", lxc));
                    otherFlags->push(String::New("__unix", lxc);
                #elif defined (__linux__)
                    response->addKey("os", String::New("Linux", lxc));
                    osDetails->addKey("subOs", String::New("__linux__", lxc));
                    otherFlags->push(String::New("__linux__", lxc);
                #elif defined (__FreeBSD__)
                    response->addKey("os", String::New("FreeBSD", lxc));
                    osDetails->addKey("subOs", String::New("__FreeBSD__", lxc));
                    otherFlags->push(String::New("__FreeBSD__", lxc);
                #elif defined (linux)
                    response->addKey("os", String::New("Linux", lxc));
                    osDetails->addKey("subOs", String::New("linux", lxc));
                    otherFlags->push(String::New("linux", lxc);
                #else
                    response->addKey("os", String::New("Unknown", lxc));
                #endif

                #if defined(__ANDROID_API__) || defined(ANDROID_API_LEVEL_H) || defined(ANDROID)
                    response->addKey("os", String::New("Android", lxc));
                #endif

                #ifdef __ANDROID_API__
                    macros->addKey("__ANDROID_API__", Number::New(__ANDROID_API__, lxc));
                #endif

                #ifdef _POSIX_ARG_MAX
                    macros->addKey("_POSIX_ARG_MAX", Number::New(_POSIX_ARG_MAX, lxc));
                #endif
                #ifdef _POSIX_CHILD_MAX
                    macros->addKey("_POSIX_CHILD_MAX", Number::New(_POSIX_CHILD_MAX, lxc));
                #endif
                #ifdef _POSIX_LINK_MAX
                    macros->addKey("_POSIX_LINK_MAX", Number::New(_POSIX_LINK_MAX, lxc));
                #endif

                #ifdef IPV6_ADDR_SCOPE_NODELOCAL
                    macros->addKey("IPV6_ADDR_SCOPE_NODELOCAL", Number::New(IPV6_ADDR_SCOPE_NODELOCAL, lxc));
                #endif


                #ifdef QT_VERSION
                    compilerDetails->addKey("qtVersion", Number::New(QT_VERSION, lxc));
                #endif

                //compiler name:
                #ifdef __CYGWIN__
                    compilerDetails->addKey("name", String::New("cygwin", lxc));
                #elif __CYGWIN32__
                    compilerDetails->addKey("name", String::New("cygwin32", lxc));
                #elif __MINGW32__
                    compilerDetails->addKey("name", String::New("mingw32", lxc));
                #elif __MINGW64__
                    compilerDetails->addKey("name", String::New("mingw64", lxc));
                #elif __GNUC__
                    compilerDetails->addKey("name", String::New("gcc", lxc));
                #elif GCC_VERSION
                    compilerDetails->addKey("name", String::New("GCC_VERSION", lxc));
                #elif _MSC_VER
                    compilerDetails->addKey("mscVer", Number::New(_MSC_VER, lxc));

                    #if __CLR_VER
                        compilerDetails->addKey("clrVer", Number::New(__CLR_VER, lxc));
                    #endif

                    #if (_MSC_VER == 1100)
                                compilerDetails->addKey("name", String::New("Visual Studio", lxc));
                    #elif (_MSC_VER == 1200)
                                compilerDetails->addKey("name", String::New("Visual Studio", lxc));
                    #elif (_MSC_VER == 1300)
                                compilerDetails->addKey("name", String::New("Visual Studio", lxc));
                    #elif (_MSC_VER == 1310)
                                compilerDetails->addKey("name", String::New("Visual Studio 2003", lxc));
                    #elif (_MSC_VER == 1400)
                                compilerDetails->addKey("name", String::New("Visual Studio 2005", lxc));
                    #elif (_MSC_VER == 1500)
                                compilerDetails->addKey("name", String::New("Visual Studio 2008", lxc));
                    #elif (_MSC_VER == 1600)
                                compilerDetails->addKey("name", String::New("Visual Studio 2010", lxc));
                    #elif (_MSC_VER == 1700)
                                compilerDetails->addKey("name", String::New("Visual Studio 2012", lxc));
                    #elif (_MSC_VER == 1800)
                                compilerDetails->addKey("name", String::New("Visual Studio 2013", lxc));
					#elif (_MSC_VER == 1900)
							compilerDetails->addKey("name", String::New("Visual Studio 2015", lxc));
					#elif (_MSC_VER == 1910)
                                compilerDetails->addKey("name", String::New("Visual Studio 2017", lxc));
                    #else
                                compilerDetails->addKey("name", String::New("Visual Studio", lxc));
                    #endif
                #else
                    compilerDetails->addKey("name", String::New("unknown"));
                #endif

                #ifdef __DATE__
                    compilerDetails->addKey("date", String::New(__DATE__, lxc));
                #endif

                #ifdef __TIME__
                    compilerDetails->addKey("time", String::New(__TIME__, lxc));
                #endif

                #ifdef __TIMESTAMP__
                    compilerDetails->addKey("timestamp", String::New(__TIMESTAMP__, lxc));
                #endif

                #ifdef __cplusplus
                    int n = __cplusplus;
                    compilerDetails->addKey("cppVersion", String::New(n, lxc));
                #endif

                #ifdef __VERSION__
                    compilerDetails->addKey("version", String::New(__VERSION__, lxc));
                #endif

                #ifdef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
                    osDetails->addKey("osxEnviroment", String::New(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__, lxc));
                #endif


                #ifdef __amd64__
                    macros->addKey("__amd64__", Boolean::New(true, lxc));
                #endif
                #ifdef _M_AMD64
                    macros->addKey("_M_AMD64", Boolean::New(true, lxc));
                #endif
				#ifdef _M_X64
					macros->addKey("_M_X64 ", Number::New(_M_X64, lxc));
				#endif
				#ifdef _M_ARM_FP
					macros->addKey("_M_ARM_FP  ", Number::New(_M_ARM_FP, lxc));
				#endif
                #ifdef _M_ARM
                    osDetails->addKey("_M_ARM", Boolean::New(true));
                #endif
				#ifdef __cplusplus_winrt
					macros->addKey("__cplusplus_winrt ", String::New(__cplusplus_winrt, lxc));
				#endif

				#ifdef __STDCPP_THREADS__
					macros->addKey("__STDCPP_THREADS__  ", Number::New(__STDCPP_THREADS__, lxc));
				#endif

				#ifdef _NATIVE_WCHAR_T_DEFINED
					macros->addKey("_NATIVE_WCHAR_T_DEFINED", Boolean::New(true, lxc));
				#endif // _NATIVE_WCHAR_T_DEFINED

				#ifdef _MSC_VER
					macros->addKey("_MSC_VER ", Number::New(_MSC_VER, lxc));
				#endif // _MSC_VER

				#ifdef _MSC_FULL_VER
					macros->addKey("_MSC_FULL_VER  ", Number::New(_MSC_FULL_VER, lxc));
				#endif // _MSC_VER


                #ifdef __INT_MAX__
                    compilerDetails->addKey("maxInt", String::New(__INT_MAX__, lxc));
                #elif _INTEGRAL_MAX_BITS
                    compilerDetails->addKey("intMaxBits", String::New(_INTEGRAL_MAX_BITS, lxc));
                #else
                    compilerDetails->addKey("maxInt", String::New("unknown", lxc));
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

                osDetails->addKey("abi", String::New(ABI, lxc));


                return response;
            };

            Value * print(Value * arguments, const lexic &lxc) {
                printf("..................print %s", blurt(arguments).c_str());
                astox_debug_end();
                return arguments;
            };

            void map_prototype(Value * prototypeObject, Value * response) {
            	method_not_implemented();
//                if (prototypeObject->type() == ASTOX_VALUE_OBJECT) {
//                    std::map<std::string, Value *>::iterator it;
//                    Object * crnt = Object::cast(prototypeObject);
//                    //				cout <<  "map protoprop " << lang::toJSON(crnt) << endl;
//                    for (it = crnt->getObjectList()->begin(); it != crnt->getObjectList()->end(); ++it) {
////                        cout << "protoprop " << lang::toJSON(it->second) << endl;
//                        Function::cast(it->second)->protoype = response;
//                        response->addKey(it->first, it->second);
//                    }
//                }
            };

            Value * String_constructor(Value * a, Value * p, const lexic &l) {
                String * response = String::New(a->str(), l);
                return response;
            }

            Value * Object_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                Object * response = Object::New(lxc);
                return response;
            };

            Value * Boolean_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                Boolean * response = Boolean::New(arguments->bol(), lxc);
                return response;
            };

            Value * Number_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                Number * response = Number::New(arguments->dbl(), lxc);
                response->setNumCalls(1);
                map_prototype(prototypeObject, response);
                return response;
            };

            Value * Array_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                //			cout <<  "------> array_create called with args: " << lang::toJSON(arguments) << endl;
                //			cout <<  "------>           and prototypeObject: " << lang::toJSON(prototypeObject, "", "") << endl;
                Array * response;
                if (arguments->isNumber()) {
                    response = new Array(arguments->toInt(), lxc);
                    response->setNumCalls(1);
                }
                else if (arguments->type() == ASTOX_VALUE_ARRAY) {
                    response = new Array(Array::cast(arguments), lxc);
                }
                else {
                    cout << " create new array" << endl;
                    response = Array::New(lxc); //TODO modify
                }

                map_prototype(prototypeObject, response);
                //response->setNumCalls(1);
                return response;
            };

            Value * Date_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                return new Date();
            };

            Value * Error_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc){
            	if(arguments->isArray()){
            		Array * args = Array::cast(arguments);
            		if(args->length() == 2){
            			return new Error(args->at(0)->str(),
            					args->at(1)->str().c_str(),
            					ASTX_CURRENT_FUNCTION, __LINE__);
            		}
            		if(args->length() == 3){
						return new Error(args->at(0)->str(),
								args->at(1)->str().c_str(),
								args->at(2)->str().c_str(),
								__LINE__);
					}
            		return new Error(args->at(0)->str(),
									args->at(1)->str().c_str(),
									args->at(2)->str().c_str(),
									args->at(3)->toInt());
            	}

            	if(arguments->isObject()){
            		Object * obj = Object::cast(arguments);
					log_stack("%s from %s", obj->getStr("message", "default").c_str(), blurt(arguments).c_str());
					return new Error(obj->getStr("message"),
            						obj->getStr("source", ASTX_LOCATION).c_str(),
            						obj->getStr("method", ASTX_CURRENT_FUNCTION).c_str(),
            						obj->getInt("line", __LINE__) );
            	}

            	return new Error(arguments->str(), ASTX_LOCATION, ASTX_CURRENT_FUNCTION, __LINE__);
            };

            Value * Require_constructor(Value * arguments, Value * prototypeObject, const lexic &lxc) {
                astox_debug(__LINE__, __FILENAME__, "Require_constructor(%s)", arguments->str().c_str());
                return Value::npos;
            };

            Value * array_reverse(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->reverse();
                }
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->reverse();
                }
                return Value::npos;
            }

            Value * array_sort(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    if (arguments->type() == ASTOX_VALUE_FUNCTION) {
                        Function * sortm = Function::cast(arguments);
                        return Array::cast(object)->sort(sortm);
                    }
                    return Array::cast(object)->sort(NULL);
                }

                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->sort();
                }
                return Value::npos;
            }

            Value * array_forEach(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    if (arguments->type() == ASTOX_VALUE_FUNCTION) {
                        Function * foreachm = Function::cast(arguments);
                        Array::cast(object)->forEach(foreachm);
                    }
                }
                return Value::npos;
            }

            Value * array_join(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {

                    if (arguments->type() == ASTOX_VALUE_UNDEFINED) {
                        return Array::cast(object)->join(",");
                    }
                    //				cout<< " array_join ARGUMENTS: " << lang::toJSON(arguments) << endl;
                    return Array::cast(object)->join(arguments->str());

                }
                return Value::npos;
            }

            Value * array_push(Value * arguments, Value * object, const lexic &lxc) {
            	if (object->type() == ASTOX_VALUE_ARRAY) {
//            		log_stack("push in array %s", blurt(arguments));

            		if(arguments->isNumber()){
            			Array::cast(object)->push(Number::New(arguments));
            		} else {
            			Array::cast(object)->push(arguments);
            		}



                    return Value::npos;
                }
                return Value::npos;
            };

            Value * array_shift(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->shift();
                }
                return Value::npos;
            };

            Value * array_pop(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->pop();
                }
                return Value::npos;
            };

            Value * array_unshift(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->unshift(arguments);
                }

                return Value::npos;
            };

            Value * array_concat(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY && arguments->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->concat(Array::cast(arguments));
                }

                return Value::npos;
            };

            Value * array_slice(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->slice(Array::cast(arguments));
                }
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->slice(arguments);
                }

                return Value::npos;
            };

            Value * array_splice(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_ARRAY) {
                    return Array::cast(object)->splice(Array::cast(arguments));
                }

                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->substr(arguments);
                }

                return Value::npos;
            };

            Value * array_indexOf(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->indexOf(arguments->str());
                }
                return Value::npos;
            };

            Value * array_lastIndexOf(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->lastIndexOf(arguments->str());
                }
                return Value::npos;
            };

            Value * string_charAt(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return object->at(arguments->toInt());
                }
                return Value::npos;
            };

            Value * string_trim(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->trim();
                }
                return Value::npos;
            };

            Value * string_killWhiteSpace(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->killWhiteSpace();
                }
                return Value::npos;
            };

            Value * string_split(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->split(arguments);
                }
                return Value::npos;
            };

            Value * string_splitByWhiteSpace(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->splitByWhiteSpace(arguments);
                }
                return Value::npos;
            };

            Value * string_splitByChar(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->splitByChar(arguments);
                }
                return Value::npos;
            };

            Value * string_match(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->match(arguments);
                }
                return Value::npos;
            };

            Value * string_replace(Value * arguments, Value * object, const lexic &lxc) {
                if (object->type() == ASTOX_VALUE_STRING) {
                    return String::cast(object)->replace(arguments);
                }
                return Value::npos;
            };

            Value * regexp_exec(Value * arguments, Value * object, const lexic &lxc){
            	  if (object->type() == ASTOX_VALUE_REGEXP) {
								return RegExp::cast(object)->exec(arguments);
				  }
				  return Value::npos;
            };

            Value * proto_toJSON(Value * arguments, Value * object, const lexic &lxc) {
                return String::New(serializers::to_json(object), lxc);
            }

            Value * proto_toString(Value * arguments, Value * object, const lexic &lxc) {
                return String::New(object->str(), lxc);
            }

            Value * setInterval(Value * arguments, Value * object, const lexic &lxc) {
                if (arguments->type() == ASTOX_VALUE_ARRAY) {
                    Array * args = Array::cast(arguments);
                    Function * callable = Function::cast(args->at(0));
                    callable->call(Value::npos, Function::npos);
#ifdef ASTOX_OS_WINDOWS
                    Sleep(1000);
#else
                    sleep(1);
#endif
                    setInterval(arguments, object, lxc);
                }
                return Value::npos;
            };


            namespace externs {

                Function * getExternContext() {

                    return NULL;
                };

                std::string fs_exec(std::string cmd) {
#ifdef ASTOX_OS_WINDOWS
        #ifdef SUPPORT_CPP_11
            //todo
            FILE* pipe = NULL;
        #else
                    FILE* pipe = _popen(cmd.c_str(), "r");

                    #endif
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
#ifdef SUPPORT_CPP_11

#else
                    _pclose(pipe);
                    #endif // SUPPORT_CPP_11
#else
                    pclose(pipe);
#endif
                    return result;
                }

                Value * console_log(Value * arguments, Value * object) {

                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        for (int i = 0; i < args->length(); i++) {
                            printf("%s ", args->at(i)->str().c_str());
                        }
                        printf("\n");
                    } else {
                        printf("%s\n", arguments->str().c_str());
                    }
                    return Value::npos;
                };

                Value * fs_exec(Value * arguments, Value * object) {
                    return NULL;
                };

                Value * fs_fileWatchAdd(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_STRING) {
                        Path p(arguments->str());
                        FileInfo info(&p);
                        fs_filesToWatch[arguments->str()] = info.getLastSavedTime();
                    }
                    if (arguments->type() == ASTOX_VALUE_OBJECT) {
                        string s = Object::cast(arguments)->getValue("path")->str();
                        Path p(s);
                        FileInfo info(&p);
                        fs_filesToWatch[arguments->str()] = info.getLastSavedTime();
                    }
                    return Value::npos;
                };

                Value * fs_fileWatchRemove(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_STRING) {
                        std::map<std::string, int>::iterator it = fs_filesToWatch.find(arguments->str());
                        if (it != fs_filesToWatch.end()) {
                            fs_filesToWatch.erase(it);
                        };
                    }
                    return Value::npos;
                };

                Value * fs_fileWatchStart(Value * arguments, Value * object) {
                    FileSystem::createWatcher((char *) arguments->str().c_str());
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
                	//method_not_implemented();
//                    Object * data = new Object();
//                    data->addKey("path", String::New(info->getPath()->toString()));
//                    data->addKey("filename", String::New(info->getPath()->getFileName()));
//                    data->addKey("extension", String::New(info->getPath()->getExtension()));
//                    data->addKey("lastModifiedTime", Number::New(info->getLastSavedTime()));
//                    data->addKey("isDirectory", Boolean::New(info->isDirectory()));
//                    data->addKey("isDot", Boolean::New(info->getPath()->isDot()));
                    //					cout << info->getLastSavedTime() << endl;
//                    Array * tokens = new Array();
//                    std::vector<std::string> tokensstr = info->getPath()->getTokens(); //Path::tokenizePath(info->getPath()->toString(), info->isDirectory(), true);
//                    for (unsigned int i = 0; i < tokensstr.size(); i++) {
////                        tokens->push(String::New(tokensstr.at(i)));
//                    }

//                    data->addKey("tokens", tokens);
                    return Object::New(Syntax::JS);
                };

                Value * fs_ftwr(Value * arguments, Value * object) {
                    std::string path;
                    std::string method = "unset";


                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        path = arguments->at(0)->str();
#ifndef ASTOX_OS_WINDOWS
                        method = arguments->at(1)->str();
#endif
                    }
                    if (arguments->type() == ASTOX_VALUE_STRING) {
                        path = arguments->str();
                    }
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::extern::fs_ftwr(%s, %s)", path.c_str(), method.c_str());
#endif

                    int mode = FileSystem::ftwrMethod(method);

                    int result = FileSystem::ftwr(path, mode);
                    return Value::npos;
                };

                Value * event_addEventListener(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        std::string eventName = args->at(0)->str();
                        Function * callback = Function::cast(args->at(1));
//                        EventDispatcher::addEventListener(eventName, callback);
                    }
                    return Value::npos;
                };



//                ssh2_data ssh2_globalData;

                Value * ssh2_connect(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        string username = args->at(0)->str();
                        string password = args->at(1)->str();
                        string host = args->at(2)->str();
//                        ssh2_globalData.username = username.c_str();
//                        ssh2_globalData.password = password.c_str();
//                        ssh2_globalData.servername = host.c_str();
//                        ssh2_scpConnect(&ssh2_globalData);
                    }
                    return Value::npos;

                };

                Value * ssh2_scpWrite(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        string localfile = args->at(0)->str();
                        string remotefile = args->at(1)->str();
//                        ssh2_scpWrite(&ssh2_globalData, localfile.c_str(), remotefile.c_str());
                    }
                    return Value::npos;
                };

                Value * mingui_dialog(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                    	int n = 0;
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
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() > 1) {
                            string s1 = args->at(0)->str();
                            string s2 = args->at(1)->str();
                            FileSystem::copyFile(s1, s2);
                        }
                    }
                    return Value::npos;
                };

                Value * mingui_notification(Value * arguments, Value * object) {
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() > 1) {
                            string s1 = args->at(0)->str();
                            string s2 = args->at(1)->str();
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
//                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::extapi_sassCompile (%s)", lang::toJSON(arguments, "", "").c_str());
#endif
                    if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);
                        if (args->length() == 2) {
                            string s1 = args->at(0)->str();
                            string s2 = args->at(1)->str();
                            const char * input = s1.c_str();
                            const char * output = s2.c_str();
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
                            astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::extapi_sassCompile (%s) to (%s)", input, output);
#endif
//                            extapi_sass_compileFile(s1.c_str(), s2.c_str());
                        }
                    }
                    return Value::npos;
                };

                Value * fs_joinFiles(Value * arguments, Value * object, const lexic &lxc) {
                    Array * response = Array::New(lxc); //TODO modify
                    if (arguments->type() == ASTOX_VALUE_OBJECT) {
                        Object * args = Object::cast(arguments);
                        std::string inputRoot = args->getValue("inputRoot")->str();
                        std::string outputRoot = args->getValue("outputRoot")->str();
                        std::string beforeSeparator;
                        std::string afterSeparator = "\n";

                        if (args->hasKey("beforeSeparator")) {
                            beforeSeparator = args->getValue("beforeSeparator")->str();
                        }

                        Array * joinRules = args->getArray("rules");


                        for (int i = 0; i < joinRules->length(); i++) {

                            if (joinRules->at(i)->hasKey("in")) {
                                Array * inputs = joinRules->at(i)->getArray("in");
                                string content;

                                for (int j = 0; j < inputs->length(); j++) {
                                    string inputfile = inputs->at(j)->str();
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

                                if (joinRules->at(i)->hasKey("out")) {
                                    string out = joinRules->at(i)->getValue("out")->str();
                                    string outcomplete = outputRoot;
                                    outcomplete.append(out);
                                    astox::util::write_string_to_file(content, outcomplete);
                                    response->push(String::New(outcomplete, lxc));
                                }
                            }
                        }


                    }
                    return response;
                };

                Value * fs_joinFilesWithConfig(Value * arguments, Value * object) {
#ifdef ASTX_DBG_ECMA_METHODS_EXTERNS
//                    astox_debug(__LINE__, __FILENAME__, "ecma::methods::externs::fs_joinFilesWithConfig (%s)", lang::toJSON(arguments, "", "").c_str());
#endif
                	  method_not_implemented();
                	if (arguments->type() == ASTOX_VALUE_ARRAY) {
                        Array * args = Array::cast(arguments);

                        std::string afterSeparator;
                        Object * joinFileArgs = new Object(Syntax::JS);

                        if (args->length() > 2) {
                            joinFileArgs->addKey("inputRoot", args->at(0));
                            joinFileArgs->addKey("outputRoot", args->at(1));
                            std::string configPath = args->at(2)->str();

                            string fileContent;
                            astox::stringAlg::appendStringFromFile(configPath, fileContent);

//                            joinFileArgs->addKey("rules", astox::scriptengine::fromStr(fileContent, Function::npos));

                            if (args->length() > 3) {
                                joinFileArgs->addKey("beforeSeparator", args->at(3));
                            }

                            return Value::npos;
                            //fs_joinFiles(joinFileArgs, object);
                        }
                    }
                    return Value::npos;
                }
            }

        }
    }
}

