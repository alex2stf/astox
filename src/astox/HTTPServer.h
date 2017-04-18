/*
 * HTTPServer.h
 *
 *  Created on: Apr 17, 2015
 *      Author: alexandru
 */

#ifndef HTTPSERVER_H_
    #define HTTPSERVER_H_
    #include "Socket.h"
    #include "Util.h"
    #include "astox.h"

    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif

    #ifdef ASTOX_OS_WINDOWS
        #pragma comment(lib, "httpapi.lib")
//#include <specstrings.h>
        #include <http.h>
//#include <stdio.h>



    #endif



namespace astox {


    #ifdef ASTOX_OS_WINDOWS

        #define INITIALIZE_HTTP_RESPONSE( resp, status, reason )    \
		do                                                      \
		{                                                       \
			RtlZeroMemory( (resp), sizeof(*(resp)) );           \
			(resp)->StatusCode = (status);                      \
			(resp)->pReason = (reason);                         \
			(resp)->ReasonLength = (USHORT) strlen(reason);     \
		} while (FALSE)

        #define ADD_KNOWN_HEADER(Response, HeaderId, RawValue)\
		do { \
			(Response).Headers.KnownHeaders[(HeaderId)].pRawValue = (RawValue);\
			(Response).Headers.KnownHeaders[(HeaderId)].RawValueLength =  (USHORT) strlen(RawValue); \
		} while(FALSE)

        #define ALLOC_MEM(cb) HeapAlloc(GetProcessHeap(), 0, (cb))

        #define FREE_MEM(ptr) HeapFree(GetProcessHeap(), 0, (ptr))
        #define MAX_ULONG_STR ((ULONG) sizeof("4294967295"))
    #endif

    class HTTPServer {
    private:
    #ifdef ASTOX_OS_WINDOWS
        std::map<std::wstring, Object *> url_map;
        std::wstring winapi_generate_url(std::wstring host, int port, std::wstring url);
        ULONG winapi_run_server();
        DWORD winapi_DoReceiveRequests(HANDLE hReqQueue);
        void winapi_processUrl(PCWSTR &url);
        DWORD winapi_SendHttpResponse(IN HANDLE hReqQueue, IN PHTTP_REQUEST pRequest, IN USHORT StatusCode, IN PSTR pReason, IN PSTR pEntity);
        DWORD winapi_SendHttpPostResponse(IN HANDLE hReqQueue, IN PHTTP_REQUEST pRequest);
    #endif

    public:

        Object * config;
        Array * urlRules;

        HTTPServer();
        void run(int port);
        void runWin();

        std::string parseHeader(char* data);

        virtual ~HTTPServer();




    };


} /* namespace astox */
#endif /* HTTPSERVER_H_ */
