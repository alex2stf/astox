/*
 * socket.cpp
 *
 *  Created on: 7 Jul 2017
 *      Author: alex
 */

#include "socket.h"
#include <list>
namespace astox {

using namespace std;


/**
unsigned int AbstractServer::getMaxClients() const {
    return maxClients;
}


AbstractServer &AbstractServer::setMaxClients(unsigned int m) {
    maxClients = m;
    return *this;
}
*/

unsigned int TCPSocketServer::getPort() const {
    return port;
}


TCPSocketServer & TCPSocketServer::setPort(unsigned int p) {
    port = p;
    cout << "3333" << endl;
    return *this;
}




void TCPSocketServer::start_server(){
    int wsaresult, i = 1;
    stx_socket_init();

    int final_port = getPort();
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(final_port);

    // Create a SOCKET for connecting to server
    master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(stx_socket_is_invalid(master_socket)) {
        stx_exit_socket_failed();
    }

    wsaresult = setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i));

    //exit on setsockopt failure
    if(stx_sock_result_has_error(wsaresult)) {
        stx_exit_socket_failed();
    }

    //Binding part
    wsaresult = bind(master_socket, (sockaddr*)&server, sizeof(server));

    //close socket on bind failed
    if(stx_sock_result_has_error(wsaresult)) {
        stx_exit_socket_error(master_socket); //close socket
    }

    // Setup the TCP listening socket
    wsaresult = listen(master_socket, 5);

    //exit on listen failed
    if(stx_sock_result_has_error(wsaresult)) {
        stx_exit_socket_failed();
    }


    //make it non blocking
    //ioctlsocket(master_socket, FIONBIO, &b);
    bool blocking = stx_set_socket_blocking_enabled(master_socket, true);

    //close socket on ioctl failed
    if(stx_socket_is_invalid(master_socket)){
        stx_exit_socket_error(master_socket); //close socket
    }

}



bool addSocketToList(std::vector<SocketState> &states, STX_SOCKET &socket, int currentState) {
    SocketState s;
    s.socket = socket;
    s.state = currentState;
    states.push_back(s);
    cout << " states size = " << states.size() << endl;
	return true;
}


void acceptConnectionFromVector(int index, std::vector<SocketState> states){
    STX_SOCKET socket = states.at(index).socket;
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	STX_SOCKET msgSocket;

	#ifdef ASTOX_OS_WINDOWS
        msgSocket = accept(socket, (struct sockaddr *)&from, &fromLen);
	#else
        msgSocket = accept(socket, (struct sockaddr *)&from, (socklen_t*)&fromLen);
	#endif

	if(stx_socket_is_invalid(msgSocket)){
        stx_exit_socket_failed();
        return;
	}

	unsigned long flag = 1;
	if(!stx_set_socket_blocking_enabled(msgSocket, true)){
		stx_socket_ioctl_err();
	}


	if (addSocketToList(states, msgSocket, RECEIVE) == false) {
		cout<<"\t\tToo many connections, dropped!\n";
		stx_socket_close(msgSocket);
	}
	return;
}







void sendMessageFromVector(int index,  std::vector<SocketState> states){
    cout << "SEND MESSAGE " << endl;
    send(states.at(index).socket, TEST_HTTP_MSG, strlen(TEST_HTTP_MSG), 0);
    states.at(index).state = IDLE;
}


void receiveMessageFromVector(int index, std::vector<SocketState> states){


	STX_SOCKET msgSocket = states.at(index).socket;

	//TODO read all
	char buffer[1025];  //data buffer of 1K
	int valread = recv( msgSocket , buffer, 1025, 0);

	if(valread > 0)
	{
		cout << "RECEIVED: " << endl;
		cout << buffer << endl;
	}

	states.at(index).state = SEND;


	cout << " RECEIVED DONE " << endl;

	sendMessageFromVector(index, states);
}








void TCPSocketServer::start(){

    cout << "33333" << endl;
    //define clients
    //int max_connections = getMaxClients();


//TODO old    SocketState socketClients[max_connections];


    cout << "Server starting..." << endl;


     std::vector<SocketState> socketClients;
    //start the server and do basic tcp setup ------------------
    start_server();
//TODO old addSocket(socketClients, master_socket, LISTEN, max_connections);
    addSocketToList(socketClients, master_socket, LISTEN);
    //set of socket descriptors
    int max_fd = master_socket;
    //the main loop
    while (active) {
        int max_connections = socketClients.size();
        cout << " LOOP INIT " << max_connections << endl;

        fd_set waitRecv;
        fd_set waitSend;
            //clear the socket set
        FD_ZERO(&waitRecv);
        FD_ZERO(&waitSend);




       bool hasRecv = false;
       bool hasSend = false;

        for (int i = 0; i < max_connections; i++)
		{
			if ((socketClients.at(i).state == LISTEN) || (socketClients.at(i).state == RECEIVE)) {
                    cout << " put in recv" << max_fd << endl;
                FD_SET(socketClients.at(i).socket, &waitRecv);
                hasRecv = true;
			}
			else if (socketClients.at(i).state == SEND){
			     cout << " put in send" << max_fd << endl;
                FD_SET(socketClients.at(i).socket, &waitSend);
                hasSend = true;
			} else {
                cout << " YOU SHOULD NOT REACH HERE" << endl;
                exit(-1);
			}

			if(socketClients.at(i).socket > max_fd){
                 max_fd = socketClients.at(i).socket;
			}
		}



		 cout << " max_fd =  " << max_fd  << endl;

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely

        timeval time;
        time.tv_sec = 0;
        time.tv_usec = 1;

        if(hasRecv && hasSend){
             select_activity = select( max_fd + 1 , &waitRecv , &waitSend , NULL , &time);
        }
        else if(hasRecv && !hasSend) {
             select_activity = select( max_fd + 1 , &waitRecv , NULL , NULL , &time);
        }

        else if(!hasRecv && hasSend) {
             select_activity = select( max_fd + 1 , NULL , &waitSend , NULL , &time);
        }



        if (select_activity <= 0) {
            printf("select() returned with error %d\n", WSAGetLastError());

            //fprintf(stderr, "%s\n", explain_select(max_fd + 1, waitRecv, waitSend, NULL, NULL));

            printf("select() returned with error");\
             cout << " LOOP SELECT DONE select_activity =  " << select_activity << endl;

            return;
        }
           // cou  cout << " SECOND ITERATION DONE " << endl;t << "SELECT ACTIVITY SUCCESS " << select_activity << endl;

        cout << " LOOP SELECT DONE select_activity =  " << select_activity << endl;


        for(int j = 0; j < max_connections && select_activity > 0; j++)
        {
            if (FD_ISSET(socketClients.at(j).socket, &waitRecv))
			{
				select_activity--;
				switch (socketClients.at(j).state)
				{
				case LISTEN:
				     cout << " listen" << j << endl;
					acceptConnectionFromVector(j, socketClients);
					break;

				case RECEIVE:
				    cout << " receive" << j << endl;
					receiveMessageFromVector(j, socketClients);
					break;
				}
			}
        }

        //select_activity = 1;


        cout << "SECOND ITERATION max_connections = "<< max_connections << " select_activity=" << select_activity  << endl;

        for (int i = 0; i < max_connections && select_activity > 0; i++)
		{
        	cout << "socketClients[i].send = " << socketClients.at(i).state << endl;
			if (FD_ISSET(socketClients.at(i).socket, &waitSend))
			{
				select_activity--;
				switch (socketClients.at(i).state)
				{
				case SEND:
					sendMessageFromVector(i, socketClients);
					break;
				}
			}
		}

         cout << " SECOND ITERATION DONE " << endl;

    }//end while

    // Closing connections and Winsock.
	cout << "Time Server: Closing Connection.\n";
	stx_socket_close(master_socket);
	stx_socket_cleanup();

}//end start

}

//http://www2.mta.ac.il/~hbinsky/intro%20comp%20comm/TCP%20server%20NonBlocking.cpp



