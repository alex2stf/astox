/*
 * socket.h
 *
 *  Created on: 7 Jul 2017
 *      Author: alex
 */


#ifndef ASTOX_SOCKET_H_
#define ASTOX_SOCKET_H_

#include "macros.h"

//winsock
#ifdef ASTOX_OS_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif // WIN32_LEAN_AND_MEAN
    #ifndef _WINDOWS_H
        #include <windows.h>
    #endif // _WINDOWS_H
    #ifndef _WINSOCK2_H
        #include <winsock2.h>
    #endif // _WINSOCK2_H

    #ifdef ASTOX_COMPILER_MSC
        #pragma comment (lib, "Ws2_32.lib")
    #endif
    // Need to link with Ws2_32.lib

    // #pragma comment (lib, "Mswsock.lib")

    #define STX_SOCKET SOCKET
    #define STX_SOCKET_ADDRESS_IN WSADATA

    #define stx_socket_init()  {  WSADATA wsaData; int wsaresult = -1;  wsaresult = WSAStartup(MAKEWORD(2, 2), &wsaData); if (wsaresult != 0) { printf("WSAStartup failed with error: %d\n", wsaresult); } }
    #define stx_socket_cleanup() { WSACleanup(); }

    #define stx_exit_socket_failed() {  printf("socket failed with error: %i\n", WSAGetLastError()); WSACleanup(); }

    #define stx_socket_ioctl_err() { std::cout << "Time Server: Error at ioctlsocket(): " << WSAGetLastError() << std::endl; }


    #define STX_INVALID_SOCK INVALID_SOCKET

//UNICES SOCKET
#else

    #include <stdio.h>
    #include <string.h>   //strlen
    #include <stdlib.h>

    #include <errno.h>
    #include <unistd.h>   //close
    #include <arpa/inet.h> //close
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
    #include <signal.h>
    #include <fcntl.h>  //for non blocking sockets
  #include <sys/ioctl.h>

    #define STX_SOCKET int
    #define STX_SOCKET_ADDRESS_IN sockaddr_in

    #define stx_socket_init() { printf("socket api init"); }
    #define stx_socket_cleanup() { printf("socket api cleaned"); }
    #define stx_exit_socket_failed() {   perror("socket failed");   exit(EXIT_FAILURE);  }
    #define stx_socket_ioctl_err() { printf("err ioctl"); }

    #define STX_INVALID_SOCK 0
#endif

/**
useful methods:
**/
static inline bool stx_socket_is_invalid(STX_SOCKET sock){
    #ifdef ASTOX_OS_WINDOWS
        return sock == INVALID_SOCKET;
    #else
        return sock == 0;
    #endif
}


static inline bool stx_sock_result_has_error(int result){
    #ifdef ASTOX_OS_WINDOWS
        return result == SOCKET_ERROR;
    #else
        return result < 0;
    #endif
}


static inline void stx_exit_socket_error(STX_SOCKET sock){
    #ifdef ASTOX_OS_WINDOWS
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
    #else
        perror("socket err");
        exit(EXIT_FAILURE);
    #endif
}

static inline void stx_socket_close(STX_SOCKET sock){
    #ifdef ASTOX_OS_WINDOWS
        closesocket(sock);
    #else
        close( sock );
    #endif
}

//https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking

static inline bool stx_set_socket_blocking_enabled(STX_SOCKET fd, bool blocking) {
   if (fd < 0) {
	   return false;
   }
   unsigned long mode = blocking ? 0 : 1;

	#ifdef ASTOX_OS_WINDOWS
	   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
	#else
	   return ioctl(fd, (int)FIONBIO, (char *)&mode);
	#endif
}


#include <iostream>



#define TEST_HTTP_MSG "HTTP/1.1 200 OK\nDate: Sun, 10 Oct 2010 23:26:07 GMT\nServer: Apache/2.2.8 (Ubuntu) mod_ssl/2.2.8 OpenSSL/0.9.8g\nLast-Modified: Sun, 26 Sep 2010 22:04:35 GMT\nContent-Length: 12\nConnection: close\nContent-Type: text/html\n\nHello world!"
//#define DEFAULT_PORT 27015
#define DEFAULT_BUFLEN 512
#define MAX_CLIENTS 50

namespace astox {

    const int EMPTY = 0;
    const int LISTEN  = 1;
    const int RECEIVE = 2;
    const int IDLE = 3;
    const int SEND = 4;

     struct SocketState {
        STX_SOCKET socket;
        //deprecated
        //int	recv = EMPTY;
        //int send = -1;
        int state;
     };

     /*
     class AbstractServer {

         protected:
            unsigned int port;
            unsigned int maxClients;
         public:
            AbstractServer & setPort(unsigned int p);
           // AbstractServer & setMaxClients(unsigned int m);
            unsigned int getPort() const;
            //unsigned int getMaxClients() const;
            virtual void start() = 0;
     };
     */



class TCPSocketServer  {


    int len;
    bool active;
    STX_SOCKET master_socket;
    int select_activity;
   // SOCKET client_fd;
    sockaddr_in server;
    //table for clients sockets
unsigned int port;


    private:
        void start_server();
	public:
	    unsigned int getPort() const;
        TCPSocketServer & setPort(unsigned int p);
		void start();
		TCPSocketServer()   {
			active = true;
			select_activity = 0;
			len = 0;
			master_socket = STX_INVALID_SOCK;
		}
	};



}




#endif /* SOCKET_H_ */
