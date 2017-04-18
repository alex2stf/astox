/*
 * HTTPServer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: alexandru
 */

#include "HTTPServer.h"
#include "Socket.h"

namespace astox {

    using namespace std;

    HTTPServer::HTTPServer() {
        using namespace std;
        // TODO Auto-generated constructor stub
        cout << " HELLO WORLD IN PORT???" << endl;
        config = new Object();

        urlRules = new Array();

        config->addKey("urlRules", urlRules);

        Object * testPage = new Object();
        std::map<std::wstring, astox::Object *> winapi_urls_container;

        testPage->addKey("url", new String("/test"));
        testPage->addKey("content", new String("THIS IS THE GET CONTENT"));

        urlRules->push(testPage);


        /*for(int i = 0; i < urlRules->length(); i++){
                winapi_urls_container[urlRules->itemAtIndex(i)->get("url")->toString()] = urlRules->itemAtIndex(i);

        }*/
    }

    HTTPServer::~HTTPServer() {
        // TODO Auto-generated destructor stub
    }


#ifdef ASTOX_OS_WINDOWS

DWORD HTTPServer::winapi_SendHttpResponse(IN HANDLE hReqQueue, IN PHTTP_REQUEST pRequest, IN USHORT StatusCode, IN PSTR pReason, IN PSTR pEntityString) {
        HTTP_RESPONSE response;
        HTTP_DATA_CHUNK dataChunk;
        DWORD result;
        DWORD bytesSent;
        // Initialize the HTTP response structure.
        INITIALIZE_HTTP_RESPONSE(&response, StatusCode, pReason);
        // Add a known header.
        ADD_KNOWN_HEADER(response, HttpHeaderContentType, "text/html");

        if (pEntityString) {
            // Add an entity chunk.
            dataChunk.DataChunkType = HttpDataChunkFromMemory;
            dataChunk.FromMemory.pBuffer = pEntityString;
            dataChunk.FromMemory.BufferLength = (ULONG) strlen(pEntityString);
            response.EntityChunkCount = 1;
            response.pEntityChunks = &dataChunk;
        }

        // Because the entity body is sent in one call, it is not  required to specify the Content-Length.
        result = HttpSendHttpResponse(
                hReqQueue, // ReqQueueHandle
                pRequest->RequestId, // Request ID
                0, // Flags
                &response, // HTTP response
                NULL, // pReserved1
                &bytesSent, // bytes sent  (OPTIONAL)
                NULL, // pReserved2  (must be NULL)
                0, // Reserved3   (must be 0)
                NULL, // LPOVERLAPPED(OPTIONAL)
                NULL // pReserved4  (must be NULL)
                );

        if (result != NO_ERROR) {
            wprintf(L"HttpSendHttpResponse failed with %lu \n", result);
        }

        return result;
    }

    DWORD HTTPServer::winapi_SendHttpPostResponse(IN HANDLE hReqQueue, IN PHTTP_REQUEST pRequest) {
        HTTP_RESPONSE response;
        DWORD result;
        DWORD bytesSent;
        PUCHAR pEntityBuffer;
        ULONG EntityBufferLength;
        ULONG BytesRead;
        ULONG TempFileBytesWritten;
        HANDLE hTempFile;
        TCHAR szTempName[MAX_PATH + 1];
        CHAR szContentLength[MAX_ULONG_STR];
        HTTP_DATA_CHUNK dataChunk;
        ULONG TotalBytesRead = 0;

        BytesRead = 0;
        hTempFile = INVALID_HANDLE_VALUE;

        //
        // Allocate space for an entity buffer. Buffer can be increased
        // on demand.
        //
        EntityBufferLength = 2048;
        pEntityBuffer = (PUCHAR) ALLOC_MEM(EntityBufferLength);

        if (pEntityBuffer == NULL) {
            result = ERROR_NOT_ENOUGH_MEMORY;
            wprintf(L"Insufficient resources \n");
            goto Done;
        }

        //
        // Initialize the HTTP response structure.
        //
        INITIALIZE_HTTP_RESPONSE(&response, 200, "OK");

        //
        // For POST, echo back the entity from the
        // client
        //
        // NOTE: If the HTTP_RECEIVE_REQUEST_FLAG_COPY_BODY flag had been
        //       passed with HttpReceiveHttpRequest(), the entity would
        //       have been a part of HTTP_REQUEST (using the pEntityChunks
        //       field). Because that flag was not passed, there are no
        //       o entity bodies in HTTP_REQUEST.
        //

        if (pRequest->Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS) {
            // The entity body is sent over multiple calls. Collect these in a file and send back. Create a temporary file.

            if (GetTempFileName(L".", L"New", 0, szTempName) == 0) {
                result = GetLastError();
                wprintf(L"GetTempFileName failed with %lu \n", result);
                goto Done;
            }

            hTempFile = CreateFile(
                    szTempName,
                    GENERIC_READ | GENERIC_WRITE,
                    0, // Do not share.
                    NULL, // No security descriptor.
                    CREATE_ALWAYS, // Overrwrite existing.
                    FILE_ATTRIBUTE_NORMAL, // Normal file.
                    NULL
                    );

            if (hTempFile == INVALID_HANDLE_VALUE) {
                result = GetLastError();
                wprintf(L"Cannot create temporary file. Error %lu \n",
                        result);
                goto Done;
            }

            do {
                //
                // Read the entity chunk from the request.
                //
                BytesRead = 0;
                result = HttpReceiveRequestEntityBody(
                        hReqQueue,
                        pRequest->RequestId,
                        0,
                        pEntityBuffer,
                        EntityBufferLength,
                        &BytesRead,
                        NULL
                        );

                switch (result) {
                    case NO_ERROR:

                        if (BytesRead != 0) {
                            TotalBytesRead += BytesRead;
                            WriteFile(
                                    hTempFile,
                                    pEntityBuffer,
                                    BytesRead,
                                    &TempFileBytesWritten,
                                    NULL
                                    );
                        }
                        break;

                    case ERROR_HANDLE_EOF:

                        //
                        // The last request entity body has been read.
                        // Send back a response.
                        //
                        // To illustrate entity sends via HttpSendResponseEntityBody, the response will be sent over multiple calls. To do this,  pass the HTTP_SEND_RESPONSE_FLAG_MORE_DATA flag.

                        if (BytesRead != 0) {
                            TotalBytesRead += BytesRead;
                            WriteFile(hTempFile,
                                    pEntityBuffer,
                                    BytesRead,
                                    &TempFileBytesWritten,
                                    NULL
                                    );
                        }

                        //
                        // Because the response is sent over multiple
                        // API calls, add a content-length.
                        //
                        // Alternatively, the response could have been
                        // sent using chunked transfer encoding, by
                        // passimg "Transfer-Encoding: Chunked".
                        //

                        // NOTE: Because the TotalBytesread in a ULONG
                        //       are accumulated, this will not work
                        //       for entity bodies larger than 4 GB.
                        //       For support of large entity bodies,
                        //       use a ULONGLONG.
                        //


                        sprintf_s(szContentLength, MAX_ULONG_STR, "%lu", TotalBytesRead);

                        ADD_KNOWN_HEADER(
                                response,
                                HttpHeaderContentLength,
                                szContentLength
                                );

                        result =
                                HttpSendHttpResponse(
                                hReqQueue, // ReqQueueHandle
                                pRequest->RequestId, // Request ID
                                HTTP_SEND_RESPONSE_FLAG_MORE_DATA,
                                &response, // HTTP response
                                NULL, // pReserved1
                                &bytesSent, // bytes sent-optional
                                NULL, // pReserved2
                                0, // Reserved3
                                NULL, // LPOVERLAPPED
                                NULL // pReserved4
                                );

                        if (result != NO_ERROR) {
                            wprintf(
                                    L"HttpSendHttpResponse failed with %lu \n",
                                    result
                                    );
                            goto Done;
                        }

                        //
                        // Send entity body from a file handle.
                        //
                        dataChunk.DataChunkType = HttpDataChunkFromFileHandle;
                        dataChunk.FromFileHandle.ByteRange.StartingOffset.QuadPart = 0;
                        dataChunk.FromFileHandle.ByteRange.Length.QuadPart = HTTP_BYTE_RANGE_TO_EOF;
                        dataChunk.FromFileHandle.FileHandle = hTempFile;

                        result = HttpSendResponseEntityBody(hReqQueue, pRequest->RequestId, 0, 1, &dataChunk, NULL, NULL, 0, NULL, NULL);

                        if (result != NO_ERROR) {
                            wprintf(
                                    L"HttpSendResponseEntityBody failed %lu\n",
                                    result
                                    );
                        }

                        goto Done;

                        break;


                    default:
                        wprintf(
                                L"HttpReceiveRequestEntityBody failed with %lu \n",
                                result);
                        goto Done;
                }

            } while (TRUE);
        } else {
            // This request does not have an entity body.
            //

            result = HttpSendHttpResponse(
                    hReqQueue, // ReqQueueHandle
                    pRequest->RequestId, // Request ID
                    0,
                    &response, // HTTP response
                    NULL, // pReserved1
                    &bytesSent, // bytes sent (optional)
                    NULL, // pReserved2
                    0, // Reserved3
                    NULL, // LPOVERLAPPED
                    NULL // pReserved4
                    );
            if (result != NO_ERROR) {
                wprintf(L"HttpSendHttpResponse failed with %lu \n",
                        result);
            }
        }

Done:

        if (pEntityBuffer) {
            FREE_MEM(pEntityBuffer);
        }

        if (INVALID_HANDLE_VALUE != hTempFile) {
            CloseHandle(hTempFile);
            DeleteFile(szTempName);
        }

        return result;
    }

    void HTTPServer::winapi_processUrl(PCWSTR &url) {

    #ifdef ASTOX_COMPILER_UNICODE
        wprintf(L"PROCESS %ws \n", url);
        wstring s(url);
    #else
        printf("PROCESS %s \n", url);
        string s(url)
    #endif
                std::cout << s.c_str() << std::endl;
    };

    DWORD HTTPServer::winapi_DoReceiveRequests(IN HANDLE hReqQueue) {
        ULONG result;
        HTTP_REQUEST_ID requestId;
        DWORD bytesRead;
        PHTTP_REQUEST pRequest;
        PCHAR pRequestBuffer;
        ULONG RequestBufferLength;

        //
        // Allocate a 2 KB buffer. This size should work for most
        // requests. The buffer size can be increased if required. Space
        // is also required for an HTTP_REQUEST structure.
        //
        RequestBufferLength = sizeof (HTTP_REQUEST) + 2048;
        pRequestBuffer = (PCHAR) ALLOC_MEM(RequestBufferLength);

        if (pRequestBuffer == NULL) {
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        pRequest = (PHTTP_REQUEST) pRequestBuffer;

        HTTP_SET_NULL_ID(&requestId);

        for (;;) {
            RtlZeroMemory(pRequest, RequestBufferLength);

            result = HttpReceiveHttpRequest(
                    hReqQueue, // Req Queue
                    requestId, // Req ID
                    0, // Flags
                    pRequest, // HTTP request buffer
                    RequestBufferLength, // req buffer length
                    &bytesRead, // bytes received
                    NULL // LPOVERLAPPED
                    );

            if (NO_ERROR == result) {
                winapi_processUrl(pRequest->CookedUrl.pAbsPath);
                switch (pRequest->Verb) {
                    case HttpVerbGET:
                        //pRequest->CookedUrl.pFullUrl
                        wprintf(L"Got a GET request for %ws \n", pRequest->CookedUrl.pFullUrl);
                        result = winapi_SendHttpResponse(hReqQueue, pRequest, 200, "OK", "Hey! You hit the server \r\n");
                        break;

                    case HttpVerbPOST:
                        wprintf(L"Got a POST request for %ws \n", pRequest->CookedUrl.pFullUrl);
                        result = winapi_SendHttpPostResponse(hReqQueue, pRequest);
                        break;

                    default:
                        wprintf(L"Got a unknown request for %ws \n", pRequest->CookedUrl.pFullUrl);
                        result = winapi_SendHttpResponse(hReqQueue, pRequest, 503, "Not Implemented", NULL);
                        break;
                }

                if (result != NO_ERROR) {
                    break;
                }
                HTTP_SET_NULL_ID(&requestId);
            }

            else if (result == ERROR_MORE_DATA) {

                requestId = pRequest->RequestId;
                // Free the old buffer and allocate a new buffer.
                RequestBufferLength = bytesRead;
                FREE_MEM(pRequestBuffer);
                pRequestBuffer = (PCHAR) ALLOC_MEM(RequestBufferLength);

                if (pRequestBuffer == NULL) {
                    result = ERROR_NOT_ENOUGH_MEMORY;
                    break;
                }

                pRequest = (PHTTP_REQUEST) pRequestBuffer;

            } else if (ERROR_CONNECTION_INVALID == result && !HTTP_IS_NULL_ID(&requestId)) {
                // The TCP connection was corrupted by the peer when attempting to handle a request with more buffer.  Continue to the next request.
                HTTP_SET_NULL_ID(&requestId);
            } else {
                break;
            }

        }



        if (pRequestBuffer) {
            FREE_MEM(pRequestBuffer);
        }

        return result;
    }

    wstring HTTPServer::winapi_generate_url(wstring host, int port, wstring url) {
        stringAlg::winapistringstream stream;
        stream << L"http://";
        stream << host;
        stream << port;
        stream << url;
        return stream.str();
    }

    ULONG HTTPServer::winapi_run_server() {
        ULONG retCode;
        HANDLE hReqQueue = NULL;
        int UrlAdded = 0;
        HTTPAPI_VERSION HttpApiVersion = HTTPAPI_VERSION_1;
        std::vector<std::wstring> urls_container;

        printf(" winapi_run_server() started");

        retCode = HttpInitialize(HttpApiVersion, HTTP_INITIALIZE_SERVER, NULL);

        if (retCode != NO_ERROR) {
            wprintf(L"HttpInitialize failed with %lu \n", retCode);
            return retCode;
        }

        retCode = HttpCreateHttpHandle(&hReqQueue, NULL);

        if (retCode != NO_ERROR) {
            wprintf(L"HttpCreateHttpHandle failed with %lu \n", retCode);
            goto CleanUp;
        }

        printf("urls added");


        const char * _host = "localhost:";



        for (int i = 0; i < urlRules->length(); i++) {
            Object * crule = Object::cast(urlRules->itemAtIndex(i));

            wstring currentUrlMap = stringAlg::stdString2wstring(urlRules->itemAtIndex(i)->getValue("url")->toString());
            wstring currentCompleteUrl = winapi_generate_url(stringAlg::string2wstring(_host), 8080, currentUrlMap);
            wprintf(L"listening for requests on the following url: %s\n", currentUrlMap.c_str());

            urls_container.push_back(currentCompleteUrl);
            url_map[currentUrlMap] = urlRules->itemAtIndex(i)->getObject("content");

            retCode = HttpAddUrl(hReqQueue, currentCompleteUrl.c_str(), NULL);

            if (retCode != NO_ERROR) {
                wprintf(L"HttpAddUrl failed with %lu \n", retCode);
                goto CleanUp;
            } else {
                UrlAdded++;
            }
        }


        winapi_DoReceiveRequests(hReqQueue);


CleanUp:
        for (int i = 1; i <= UrlAdded; i++) {
            HttpRemoveUrl(hReqQueue, urls_container.at(i).c_str());
        }

        if (hReqQueue) {
            CloseHandle(hReqQueue);
        }

        HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);

        return retCode;
    };
#endif

    void HTTPServer::run(int port) {
#ifdef ASTOX_OS_WINDOWS
        winapi_run_server();
#endif
    }

    std::string HTTPServer::parseHeader(char * data) {
        using namespace std;
        std::string headerData = data;
        //	cout << " HEADER" << headerData << endl;
        std::vector<std::string> _lines = stringAlg::splitByChar(headerData, '\n');

        for (unsigned int i = 0; i < _lines.size(); i++) {
            cout << "---->" << _lines.at(i) << endl;
        }


        std::string response;

        std::string htmlContent = "<html><body><H1>Hello world</H1></body></html>";
        response.append("HTTP/1.1 200 OK\n");
        response.append("Content-length: ");

        std::stringstream ss(stringstream::in | stringstream::out);
        ss << (int) htmlContent.size();

        response.append(ss.str());
        response.append("\n");
        response.append("Content-Type: text/html\n\n");
        response.append(htmlContent);
        //	cout << response << endl;
        return response;
    }


} /* namespace astox */
