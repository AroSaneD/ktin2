#ifndef SOCKETOBJECT_H
#define SOCKETOBJECT_H


#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


using namespace std;



class SocketObject {
public:
    SocketObject(string port, string address);
    SocketObject();

    void sendMessage(string message);
    int receiveResponse(string &response);
    int connectToServer();

protected:

private:
    int serverSocket;
    sockaddr_in server;
};




#endif // SOCKETOBJECT_H
