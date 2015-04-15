#include "SocketObject.h"

using namespace std;

SocketObject::SocketObject(string port, string address){

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        //handle errors
        cout << "Error creating socket" << endl;
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port.c_str()));
    server.sin_addr.s_addr = inet_addr(address.c_str()); //add adress of ftp server
}

SocketObject::SocketObject(){

}

int SocketObject::connectToServer(){
    int returnableValue;
    if((returnableValue = connect(serverSocket, (struct sockaddr *) &server, sizeof(server))) < 0){
        //handle errors
        cout << "Error while connecting to the server" << endl;
    }

    return returnableValue;

}


void SocketObject::sendMessage(string message){
    //char* messageToServer = new char[512];
    //size_t alloc = message.length();


    //char* messageToServer = (char*) malloc(alloc + 1);

    char* messageToServer[512];
    memset(&messageToServer, 0, sizeof(messageToServer));
    //strcpy(messageToServer, message.c_str());


    //strncpy(messageToServer, message.c_str(), sizeof(messageToServer));
    //messageToServer[sizeof(messageToServer) - 1] = 0;

    //memcpy(messageToServer, message.c_str(), alloc+1);

    cout << "Message sent: " << message << endl;
    send(serverSocket, message.c_str(), message.length(), 0);

    //delete[] messageToServer;
    //send(serverSocket, message, message.length(), 0);

    //free(messageToServer);
}

int SocketObject::receiveResponse(string &response){

    char messageFromServer[512];

    memset(&messageFromServer, 0, sizeof(messageFromServer));

    int messageLength = recv(serverSocket, messageFromServer, sizeof(messageFromServer), 0);

    if(messageLength < 0){
        //possible error handling
    }
    else{
        response = messageFromServer;
    }

    return messageLength;
}
