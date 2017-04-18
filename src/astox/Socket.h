/*
 * Socket.h
 *
 *  Created on: Apr 22, 2015
 *      Author: alexandru
 */

#ifndef SOCKET_H_
    #define SOCKET_H_


    #include "Macros.h"

    #ifdef ASTOX_OS_WINDOWS
        #include <stdlib.h>
        #pragma once
        #pragma comment(lib, "ws2_32.lib") //Winsock Library
        #define SOCKINET_ADDR SOCKADDR_IN
        #define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
        #define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
    #else
        #include<sys/socket.h>
        #include<arpa/inet.h> //inet_addr
        #include<unistd.h>    //write
        #include<pthread.h> //for threading , link with lpthread
        #include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
        #include <unistd.h>   //close
        #include <arpa/inet.h>    //close
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <semaphore.h>
        #include <fcntl.h> //required by ubuntu
        #define SOCKINET_ADDR addrinfo
    #endif

namespace astox {


    #ifdef ASTOX_SOCK_WIN
        #define SockId SOCKET
    #else
        #define SockId int
    #endif

    struct SocketInfo {
        std::string remoteIp;
        SockId masterSocket;
        SockId currentSocket;
        int remotePort;
        std::vector<SockId> clients;
    };

    enum TypeSocket {
        BlockingSocket, NonBlockingSocket
    };

    enum SocketStats {
        SOCK_RECV_ERROR = 1,
        SOCK_RECV_SUCCESS
    };
    typedef void (*Socket_onConnect_Handler)(SockId _socket, SocketInfo remoteAddr);
    typedef void (*Socket_onDisconnect_Handler)(SockId _socket);
    typedef void (*Socket_onReceive_Handler)(std::string buffer, SocketInfo remoteAddr);
    typedef std::vector<SockId> SocketClients;

    class Socket {
    public:
        Socket();

        volatile bool sockConnected;
        virtual ~Socket();

        int runNonBlocking();

        void Close();
        static void Send(SockId socket, std::string _line);
        //static void SendBytes(SockId socket, const std::string& _line);
        static int Receive(SockId socket, std::string &_line, int lastReadSize = 0);
        //static int ReceiveBytes(SockId socket, std::string &_line, int lastReadSize = 0);

        void onConnect(Socket_onConnect_Handler _onConnectParam);
        void onReceive(Socket_onReceive_Handler _onReceiveParam);
        void onDisconnect(Socket_onDisconnect_Handler _onDisconnectParam);

        int runClient(const char * hostname, unsigned short int portNumber);
        static void SetNonBlock(int socket);
        static bool StartServices();
    protected:


    private:

        Socket_onConnect_Handler _onConnect;
        Socket_onReceive_Handler _onReceive;
        Socket_onDisconnect_Handler _onDisconnect;
        SocketClients _connectedClients;


    #ifdef ASTOX_SOCK_WIN
        int runNonBlock_Win();
        SocketInfo getSockInfo(sockaddr_in &address, SockId _mSock, SockId _cSock, SocketClients _clients);
    #else
        int runNonBlock_Posix();
        SocketInfo getSockInfo(sockaddr_storage &remoteaddr, SockId _mSock, SockId _cSock, SocketClients _clients);
    #endif


        static void EndServices();
        static int nofSockets_;
        SockId s_;


        static void *GetInAddr(struct sockaddr *sa);

    };





} /* namespace astox */
#endif /* SOCKET_H_ */
