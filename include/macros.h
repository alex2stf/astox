#ifndef ASTOX_STMACROS_H
#define ASTOX_STMACROS_H
	
	//used for test_result_fail, throw_error
	#define ASTX_MAX_BUFSZ 1024

    #ifdef ASTX_ENABLE_NODEJS
        #include <v8.h>
        #include <node.h>
    #endif

    #include <sstream>
    #include <ostream>
    #include <iostream>
    #include <string>
    #include <time.h>
    #include <vector>
    #include <algorithm>
    #include <cctype>
    #include <functional>
    #include <fstream>
    #include <iterator>
    #include <algorithm>
    #include <map>
    #include <stdio.h>
    #include <ctime>
    #include <stdlib.h>
    #include <string>
    #include <cstring>
    #include <iterator>
    #include <math.h>
	#include <set>
/* keep in this place the main windows.h import, otherwise shit will happen: */


// Define NaN (Not-a-Number) on Windows
    #ifdef WIN32
        #ifndef NAN
            static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
            #define NAN (*(const float *) __nan)
        #endif
    #endif

    #ifdef _MSC_VER
        #define ASTOX_COMPILER_MSC
        //fix isnan and isinf methods:
        #define isnan(x) _isnan(x)
        #define isinf(x) (!_finite(x))
        //#include <limits.h>
        #include <cmath>
        #include <float.h>

        static inline double cbrt(double x) {
            if (fabs(x) < DBL_EPSILON) return 0.0;
            if (x > 0.0) return pow(x, 1.0 / 3.0);
            return -pow(-x, 1.0 / 3.0);
        }

    #elif defined(__CYGWIN__) || defined(__CYGWIN32__)
        #define ASTOX_COMPILER_CYGWIN
    #elif defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__)
        #define ASTOX_COMPILER_GNU
    #endif



    #if defined(_WIN32) || defined (__WIN32__) || defined(WIN32) || defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__TOS_WIN__) || defined(__WINDOWS__)// note the underscore: without it, it's not msdn official!
        #define ASTOX_OS_WINDOWS 1
        #define ASTOX_SOCK_WIN 1

        //include winsock only once
        #ifdef ASTX_ENABLE_LIBSSH2
            #include <libssh2.h>
            #include <libssh2_sftp.h>
        #else
          
            #include <windows.h>
        #endif


        /*includes for FileSystem.h*/
        #include <direct.h>
        #include <comdef.h>
        #include <tchar.h>
        #include <wchar.h>
        #include <comutil.h>
    #elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__) // all unices, not all compilers
        #define ASTOX_OS_LINUX
        #define ASTOX_SOCK_UNIX
    #elif defined(__APPLE__) || defined(__MACH__)
        #define ASTOX_OS_MAC
        #define ASTOX_SOCK_UNIX
    #endif

        
    #ifdef ASTOX_INOTIFY_SUPPORT
        #include <sys/inotify.h>
    #endif

    #if defined(_UNICODE) || defined(UNICODE)
        #define ASTOX_COMPILER_UNICODE
    #endif


    #ifdef ASTOX_OS_WINDOWS
        #define ASTOX_OS_SEPARATOR '\\'
        #define ASTOX_OS_SEPARATOR_OTHER '/'
    #else
        #define ASTOX_OS_SEPARATOR '/'
        #define ASTOX_OS_SEPARATOR_OTHER '\\'
    #endif

    #ifdef __cplusplus
        #define ASTOX_COMP_CPP
    #else
        #define ASTOX_COMP_C
    #endif



    #ifdef ASTOX_SOCK_WIN

        #define astox_socket_close(socket)\
	{\
		closesocket(socket);\
	};

        #define astox_socket_clean()\
	{\
		 WSACleanup() ;\
	};

        #define astox_close_thread(thread)\
	{\
		CloseHandle(thread);\
	};

    #elif defined(ASTOX_SOCK_UNIX)

        #define astox_socket_close(socket)\
	{\
			free(socket);\
	};

        #define astox_socket_clean()\
	{\
		 ;;\
	};

        #define astox_close_thread(thread)\
	{\
		;;\
	};


    #endif

////////////////////////C++98/03 - 199711L
//C++11 - 201103L

    #if __cplusplus < 199711L
        #if defined ASTOX_OS_MAC
            #define ASTOX_REGEX_SUPPORT_POSIX
            #include <regex.h>
        #elif defined ASTOX_OS_LINUX
            #define ASTOX_REGEX_SUPPORT_POSIX
            #include <regex.h>
        #endif
    #elif __cplusplus == 199711L &&  defined ASTOX_OS_WINDOWS
        #define ASTOX_REGEX_SUPPORT_C11 4
        #include <regex>
    #else
        #define ASTOX_REGEX_SUPPORT_POSIX
        #include <regex.h>
    #endif



    #ifdef ASTOX_OS_WINDOWS
        enum Astox_Console_Colors {
            FG_UNDERLINE = 236,
            FG_BLINK = 206,
            FG_INVERT = 240,
            FG_GREY_1 = 8,
            FG_RED_1 = 4,
            FG_GREEN_1 = 2,
            FG_YELLOW_1 = 6,
            FG_BLUE_1 = 1,
            FG_CYAN_1 = 5,
            FG_TURQOISE_1 = 3,
            FG_DEFAULT = 15,
            FG_GREY_2 = 7,
            FG_RED_2 = 12,
            FG_GREEN_2 = 10,
            FG_YELLOW_2 = 14,
            FG_BLUE_2 = 9,
            FG_CYAN_2 = 13,
            FG_TURQOISE_2 = 11,
            BG_BLUE_1 = 31,
            BG_GREEN_1 = 47,
            BG_TURQOISE_1 = 63,
            BG_RED_1 = 79,
            BG_CYAN_1 = 95,
            BG_YELLOW_1 = 111,
            BG_GREY_1 = 143,
            BG_GREY_2 = 112,
            BG_BLUE_2 = 144,
            BG_GREEN_2 = 160,
            BG_TURQOISE_2 = 176,
            BG_RED_2 = 192,
            BG_CYAN_2 = 208,
            BG_YELLOW_2 = 244,
            BG_DEFAULT = 15
        };


        #define getcwd  _getcwd


        #define astox_console_write(x, ...) if ((x)) {  \
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;\
            WORD saved_attributes;\
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);\
            saved_attributes = consoleInfo.wAttributes;\
            SetConsoleTextAttribute(hConsole, x);\
            printf(__VA_ARGS__); \
            SetConsoleTextAttribute(hConsole, saved_attributes); }
    #else
        #include <unistd.h>
        enum Astox_Console_Colors {
            FG_UNDERLINE = 4,  FG_BLINK = 5, FG_INVERT = 7,
            FG_GREY_1 = 2,
            FG_RED_1 = 31,
            FG_GREEN_1 = 32,
            FG_YELLOW_1 = 33,
            FG_BLUE_1 = 34,
            FG_CYAN_1 = 35,
            FG_TURQOISE_1 = 36,
            FG_DEFAULT = 39,
            FG_GREY_2 = 37,
            FG_RED_2 = 91,
            FG_GREEN_2 = 92,
            FG_YELLOW_2 = 93,
            FG_BLUE_2 = 94,
            FG_CYAN_2 = 95,
            FG_TURQOISE_2 = 96,
            BG_RED_1 = 41,
            BG_GREEN_1 = 42,
            BG_YELLOW_1 = 43, BG_BLUE_1 = 44, BG_CYAN_1 = 45, BG_TURQOISE_1 = 46,
            BG_GREY_1 = 47,  BG_GREY_2 = 100, BG_RED_2 = 101,
            BG_GREEN_2 = 102, BG_YELLOW_2 = 103,  BG_BLUE_2 = 104,
            BG_CYAN_2 = 105,  BG_TURQOISE_2 = 106, BG_DEFAULT = 49
        };

        #define _getcwd  getcwd


        #define astox_console_write(x, ...) if ((x)) { \
                if (isatty(fileno(stdin))) { printf("\033[1;%dm", x); } \
                printf(__VA_ARGS__); \
                if (isatty(fileno(stdin))) { printf("\033[0m"); }\
        }

        #define astox_console_clear(){ write(1,"\E[H\E[2J",7); }
        #define astox_console_clearline(){ printf("\n\033[F\033[J"); }
#endif

#ifndef ASTX_LONGEST_FILENAME
	#define ASTX_LONGEST_FILENAME 19
#endif

#define astox_suspend(yes, no) { char ch; printf("press (%c) to continue, or (%c) to exit\n", yes, no); scanf("%c", &ch); if(ch == no){ std::exit(0);} }

#define astox_debug_start(lin, fil){ \
		std::string filename = fil;  if(filename.size() < ASTX_LONGEST_FILENAME){ std::string rest = std::string(ASTX_LONGEST_FILENAME - filename.size(), ' '); filename.append(rest); }; \
		std::ostringstream convert; convert << filename; convert << " ";  \
		if(lin > 10 && lin < 100) { convert << "00"; };\
		if(lin > 100 && lin < 1000) { convert << "0"; }; convert << lin; convert << ": "; \
		printf("%s", convert.str().c_str()); \
	}

#define astox_debug_start_with(lin, fil, text){ \
		astox_debug_start(lin, fil) \
		printf("%s", text);\
}

#define astox_debug_printf(...){ \
		printf(__VA_ARGS__); \
}

#define astox_debug_end(){ \
		printf("\n"); \
}
#ifdef ASTOX_COMPILER_MSC
	#define astox_sprintf(buffer, size, format, ...){ sprintf_s(buffer, size, format, __VA_ARGS__); }
#else
	#define astox_sprintf(s_, sz_, f_, ...) snprintf((s_), (sz_), (f_), __VA_ARGS__);
#endif


#define astox_debug(line, file, ...) { \
		astox_debug_start(line, file) \
		astox_debug_printf(__VA_ARGS__); \
		astox_debug_end(); \
}





#ifdef ASTOX_OS_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif


#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define ASTX_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define ASTX_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
	#define ASTX_CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define ASTX_CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define ASTX_CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define ASTX_CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define ASTX_CURRENT_FUNCTION __func__
#else
	#define ASTX_CURRENT_FUNCTION "(unknown)"
#endif

#define ASTXPRV1(x) #x
#define ASTXPRV2(x) ASTXPRV1(x)

#ifdef ASTOX_OS_WINDOWS
#define ASTX_LOCATION \
	strrchr( (__FILE__ " " ASTXPRV2(__LINE__) ), '\\') ? \
	strrchr( ( __FILE__ " " ASTXPRV2(__LINE__) ), '\\') + 1 : \
	(__FILE__ " " ASTXPRV2(__LINE__) )
#else
#define ASTX_LOCATION \
	strrchr( (__FILE__ " " ASTXPRV2(__LINE__) ), '/') ? \
	strrchr( ( __FILE__ " " ASTXPRV2(__LINE__) ), '/') + 1 : \
	(__FILE__ " " ASTXPRV2(__LINE__) )
#endif

#define stxtr std::string

#define ASTX_BLURT_THIS_FUNC printf ("[%-20s %-20s]\n", ASTX_CURRENT_FUNCTION, ASTX_LOCATION)



#ifdef __ANDROID_API__
    #include <android/log.h>
    #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "printf", __VA_ARGS__);
#endif




#if defined(__ANDROID_API__) && defined(ASTX_DBG_SCRIPT_ENGINE)
    #define stx_dgb_script(info, ... ){ __android_log_print(ANDROID_LOG_DEBUG, info, __VA_ARGS__);}
#elif defined(__ANDROID_API__)
    #define stx_dgb_script(info, ... ){ printf("%-20s ", info); printf(__VA_ARGS__); printf("\n");}
#else
	#define stx_dgb_script(txt, ...){}
#endif

#if defined(__ANDROID_API__) && defined(ASTX_TRC_SCRIPT_ENGINE)
    #define stx_trc_script(info, ... ){ __android_log_print(ANDROID_LOG_VERBOSE, info, __VA_ARGS__);}
#elif defined(ASTX_TRC_SCRIPT_ENGINE)
    #define stx_trc_script(info, ... ){ printf("%-20s ", info); printf(__VA_ARGS__); printf("\n");}
#else
	#define stx_trc_script(txt, ...){}
#endif




//allow file crawl for QT:
//ASTOX_FILE_CRAWL_ON_QT
#endif // STMACROS_H
