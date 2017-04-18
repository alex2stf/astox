/*
 * Socket.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: alexandru
 */

#include "Socket.h"

namespace astox {

    using namespace std;

    int Socket::nofSockets_ = 0;

    bool Socket::StartServices() {
#ifdef ASTOX_SOCK_WIN
        if (!nofSockets_) {
            INT Ret;
            WSADATA wsaData;
            if ((Ret = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) { //if ((Ret = WSAStartup(0x0202,&wsaData)) != 0) {
                printf("WSAStartup() failed with error %d\n", Ret);
                WSACleanup();
                return false;
            }

            printf("WSAStartup() is fine!\n");
            ++nofSockets_;
        }
        return true;
#else
        return true;
#endif
    }

    void Socket::EndServices() {
#ifdef ASTOX_SOCK_WIN
        WSACleanup();
#else
        ;
        ;
#endif
    }


#ifdef ASTOX_SOCK_WIN

    SocketInfo Socket::getSockInfo(sockaddr_in &address, SockId _mSock, SockId _cSock, SocketClients _clients) {
        SocketInfo r;
        r.remoteIp = inet_ntoa(address.sin_addr);
        //	r.remoteIp = inet_ntop(address.sin_addr);

        r.currentSocket = _cSock;
        r.clients = _clients;
        r.remotePort = ntohs(address.sin_port);
        r.masterSocket = _mSock;
        //printf("New connection, ip is : %s , port : %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        return r;
    };
#else

    SocketInfo Socket::getSockInfo(sockaddr_storage &remoteaddr, int _mSock, int _cSock, SocketClients _clients) {
        SocketInfo r;
        char remoteIP[INET6_ADDRSTRLEN];
        r.remoteIp = inet_ntop(remoteaddr.ss_family, Socket::GetInAddr((struct sockaddr*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN);
        r.currentSocket = _cSock;
        r.clients = _clients;
        r.masterSocket = _mSock;
        return r;
    };
#endif

    Socket::Socket() {
        StartServices();
        _onConnect = NULL;
        _onReceive = NULL;
        _onDisconnect = NULL;
        sockConnected = false;
        // UDP: use SOCK_DGRAM instead of SOCK_STREAM
    }

    Socket::~Socket() {
        Close();
        //  --nofSockets_;
        //  if (!nofSockets_) End();
    }

    void Socket::onConnect(Socket_onConnect_Handler _onConnectParam) {
        _onConnect = _onConnectParam;
    };

    void Socket::onReceive(Socket_onReceive_Handler _onReceiveParam) {
        _onReceive = _onReceiveParam;
    };

    void Socket::onDisconnect(Socket_onDisconnect_Handler _onDisconnectParam) {
        _onDisconnect = _onDisconnectParam;
    };

    void Socket::SetNonBlock(int socket) {
#ifdef ASTOX_SOCK_UNIX
        int flags = fcntl(socket, F_GETFL, 0);
        //assert(flags != -1);
        fcntl(socket, F_SETFL, flags | O_NONBLOCK);
#else
        // If iMode!=0, non-blocking mode is enabled.
        u_long iMode = 1;
        ioctlsocket(socket, FIONBIO, &iMode);
#endif
    };

    int Socket::runNonBlocking() {
#ifdef ASTOX_SOCK_WIN
        return runNonBlock_Win();
#else
        return runNonBlock_Posix();
#endif
    }

#ifdef ASTOX_SOCK_WIN

int Socket::runNonBlock_Win() {

        SOCKET master, new_socket, s;

        struct sockaddr_in server, address;
        int activity, addrlen, i, valread;

        fd_set readfds;


        //Create a socket
        if ((master = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            printf("Could not create socket : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        printf("Socket created.\n");


        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(9001);


        //Bind
    #ifdef __cluplusplus
        if (::bind(master, (struct sockaddr *) &server, sizeof (server)) == SOCKET_ERROR) {
    #else
        if (::bind(master, (struct sockaddr *) &server, sizeof (server)) == SOCKET_ERROR) {
    #endif
            printf("Bind failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        puts("Bind done");

        //Listen to incoming connections
        listen(master, 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        sockConnected = true;
        s_ = master;
        addrlen = sizeof (struct sockaddr_in);

        _connectedClients.push_back(master);

        std::string receivedData;

        while (TRUE) {
            //clear the socket fd set
            FD_ZERO(&readfds);

            //add master socket to fd set
            FD_SET(master, &readfds);

            //add child sockets to fd set
            for (i = 0; i < _connectedClients.size(); i++) {
                s = _connectedClients.at(i);
                if (s > 0) {
                    FD_SET(s, &readfds);
                }
            }

            //wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
            activity = select(0, &readfds, NULL, NULL, NULL);

            if (activity == SOCKET_ERROR) {
                printf("select call failed with error code : %d", WSAGetLastError());
                return 1;
            }

            //If something happened on the master socket , then its an incoming connection
            if (FD_ISSET(master, &readfds)) {
                if ((new_socket = accept(master, (struct sockaddr *) &address, (int *) &addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                if (_onConnect != NULL) {
                    _onConnect(new_socket, getSockInfo(address, 0, 0, _connectedClients));
                }

                //Socket::SetNonBlock(new_socket);

                printf("Adding to list of sockets at index %d \n", i);
                _connectedClients.push_back(new_socket);
            }

            //else its some IO operation on some other socket :)
            for (i = 0; i < _connectedClients.size(); i++) {
                s = _connectedClients.at(i);
                //if client presend in read sockets
                if (FD_ISSET(s, &readfds)) {
                    //get details of the client
                    getpeername(s, (struct sockaddr*) &address, (int*) &addrlen);

                    receivedData.clear();
                    valread = Socket::Receive(s, receivedData);
                    if (valread < 0) {
                        int error_code = WSAGetLastError();
                        if (error_code == WSAECONNRESET) {
                            //Somebody disconnected , get his details and print
                            printf("Host disconnected unexpectedly , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                            closesocket(s);
                            _connectedClients.erase(_connectedClients.begin() + i);
                        } else {
                            printf("recv failed with error code : %d", error_code);
                        }
                    }
                    if (valread == 0) {
                        //Somebody disconnected , get his details and print
                        printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                        //Close the socket
                        closesocket(s);
                        //erase client
                        _connectedClients.erase(_connectedClients.begin() + i);
                    }

                    else {
                        if (_onReceive != NULL && !receivedData.empty()) {
                            //dispatch the message that came in
                            _onReceive(receivedData, getSockInfo(address, master, s, _connectedClients));
                        }
                    }
                }
            }
        }

        closesocket(s);
        WSACleanup();

        return 0;

    }

#else

    int Socket::runNonBlock_Posix() {
        const char * ch_portNumber = "9001";

        fd_set master; // master file descriptor list
        fd_set read_fds; // temp file descriptor list for select()
        int fdmax; // maximum file descriptor number
        int listener; // listening socket descriptor
        int newfd; // newly accept()ed socket descriptor
        struct sockaddr_storage remoteaddr; // client address
        socklen_t addrlen;


        int nbytes;

        // char remoteIP[INET6_ADDRSTRLEN];

        int yes = 1; // for setsockopt() SO_REUSEADDR, below
        int i, rv;

        struct SOCKINET_ADDR hints, *ai, *p;

        FD_ZERO(&master); // clear the master and temp sets
        FD_ZERO(&read_fds);

        // get us a socket and bind it
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;


        if ((rv = getaddrinfo(NULL, ch_portNumber, &hints, &ai)) != 0) {
            fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
            exit(1);
        }

        for (p = ai; p != NULL; p = p->ai_next) {
            listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (listener < 0) {
                continue;
            }

            // lose the pesky "address already in use" error message
            setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));

            if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
                close(listener);
                continue;
            }

            break;
        }

        // if we got here, it means we didn't get bound
        if (p == NULL) {
            fprintf(stderr, "selectserver: failed to bind\n");
            exit(2);
        }

        freeaddrinfo(ai); // all done with this

        // listen
        if (listen(listener, 10) == -1) {
            perror("listen"); // trateaza erorile din manpage
            exit(3);
        }

        // add the listener to the master set
        FD_SET(listener, &master);

        // keep track of the biggest file descriptor
        fdmax = listener; // so far, it's this one
        sockConnected = true;
        s_ = listener;
        // main loop
        for (;;) {
            read_fds = master; // copy it
            if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
                perror("select");
                return 1;
            }

            // run through the existing connections looking for data to read
            for (i = 0; i <= fdmax; i++) {
                char buf[1256]; // buffer for client data
                if (FD_ISSET(i, &read_fds)) { // we got one!!
                    if (i == listener) {
                        // handle new connections
                        addrlen = sizeof remoteaddr;
                        newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);

                        if (newfd == -1) {
                            perror("accept");
                        } else {
                            FD_SET(newfd, &master); // add to master set
                            if (newfd > fdmax) { // keep track of the max
                                fdmax = newfd;
                            }

                            if (_onConnect != NULL) {
                                _onConnect(i, getSockInfo(remoteaddr, 0, 0, _connectedClients));
                            }
                        }

                    } else {
                        std::string inmessage;
                        nbytes = Socket::Receive(i, inmessage, 0);
                        if (nbytes <= 0) {

                            if (_onDisconnect != NULL) {
                                _onDisconnect(i);
                            }

                            close(i); // bye!
                            FD_CLR(i, &master); // remove from master set

                        } else {

                            _connectedClients.clear();
                            for (int j = 0; j <= fdmax; j++) {
                                if (FD_ISSET(j, &master)) {
                                    _connectedClients.push_back(j);
                                }
                            }

                            if (_onReceive != NULL) {
                                _onReceive(inmessage, getSockInfo(remoteaddr, listener, i, _connectedClients));
                            }

                            buf[strlen(buf)] = '\0';
                            // FD_ZERO(&buf);
                        }
                    } // END handle data from client
                } // END got new incoming connection
            } // END looping through file descriptors
        } // END for(;;)--and you thought it would never end!


        return 0;


    }
#endif

    void *Socket::GetInAddr(struct sockaddr *sa) {
#ifdef ASTOX_SOCK_UNIX
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*) sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*) sa)->sin6_addr);
#else
        return 0;
#endif
    };

    void Socket::Close() {
#ifdef ASTOX_SOCK_WIN
        closesocket(s_);
#else
        close(s_);
#endif
    };

    int Socket::runClient(const char * hostname, unsigned short int portNumber) {
#ifdef ASTOX_SOCK_WIN
        StartServices();

        SockId _sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_sockClient == INVALID_SOCKET) {
            std::cout << "Winsock error - Socket creation Failed!\r\n";
            WSACleanup();
            system("PAUSE");
            return 0;
        }


        struct hostent *host;
        if ((host = gethostbyname(hostname)) == NULL) {
            std::cout << "Failed to resolve hostname.\r\n";
            WSACleanup();
            system("PAUSE");
            return 0;
        }


        SOCKADDR_IN SockAddr;
        SockAddr.sin_port = htons(portNumber);
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_addr.s_addr = *((unsigned long*) host->h_addr);


        // Attempt to connect to server
        if (connect(_sockClient, (SOCKADDR*) (&SockAddr), sizeof (SockAddr)) != 0) {
            std::cout << "Failed to establish connection with server\r\n";
            WSACleanup();
            system("PAUSE");
            return 0;
        }
        double iTimeout = 0.5;
        setsockopt(_sockClient, SOL_SOCKET, SO_RCVTIMEO, (const char *) &iTimeout, sizeof (iTimeout));
        cout << " CONNECTION DONE " << endl;

        if (_onConnect != NULL) {
            SocketInfo n;
            n.remotePort = portNumber;
            _onConnect(_sockClient, n);
        }
#else
        struct sockaddr_in serveraddr;
        int sockfd;
        struct hostent *server;
        /* socket: create the socket */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("ERROR opening socket");
        }

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(hostname);
        if (server == NULL) {
            fprintf(stderr, "ERROR, no such host as %s\n", hostname);
            exit(0);
        }
        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof (serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *) server->h_addr, (char *) &serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(portNumber);

        /* connect: create a connection with the server */
        if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof (serveraddr)) < 0) {
            perror("ERROR connecting");
            return 1;
        }
        if (_onConnect != NULL) {
            SocketInfo n;
            n.remotePort = portNumber;
            _onConnect(sockfd, n);
        }

        close(sockfd);
#endif
        return 0;
    };

#ifdef ASTOX_SOCK_WINDOWS

int Socket::Receive(SockId socket, std::string &_line, int lastReadSize) {

        const int SIZE = 1;
        char *buffer = new char[1];
        bzero(buffer, SIZE);
        size_t sizeOfBuffer = sizeof (buffer) / sizeof (char);
        cout << "ReceiveBytes start " << sizeOfBuffer << endl;
        buffer[1] = '\0';
        Socket::SetNonBlock(socket);
        while (recv(socket, buffer, 1, 0) >= 0) {
            _line.append(buffer);
            bzero(buffer, SIZE);
        }
        //	cout << " received: " << _line.c_str() << endl;
        return 1;
    };
#else

    #define CHUNK_SIZE 10

    int Socket::Receive(SockId socket, std::string &_line, int lastReadSize) {

        //std::cout << "Socket::Receive (" << socket << "," << _line.c_str() << "," <<lastReadSize  << endl;

        char *buffer;
        const int SIZE = 10;
        buffer = new char[SIZE];
        bzero(buffer, SIZE);
        buffer[SIZE] = '\0';
        int result;
        size_t sizeOfBuffer = sizeof (buffer) / sizeof (char);

        result = recv(socket, buffer, sizeOfBuffer, 0);

        if (result < 0) {
            return result;
        }

        if (result < 1 && lastReadSize == 0) {
            return result; //send first read error
        }

        if (result < 1 && lastReadSize > 0) {
            return lastReadSize; // second read needs no more appending
        }
        buffer[result] = '\0';
        _line.append(buffer);

        if (result == (int) sizeOfBuffer) {
            return Receive(socket, _line, result);
        }

        if (lastReadSize > 0) {
            return lastReadSize;
        }

        return result;

    };
#endif

    void Socket::Send(SockId socket, std::string _line) {
        send(socket, _line.c_str(), _line.length(), 0);
    };










} /* namespace astox */
