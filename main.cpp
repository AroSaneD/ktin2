#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



using namespace std;



int main()
{
    int serverSocket;
    sockaddr_in server;
    char messageFromServer[512];
    char messageToServer[512];



    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        //handle errors
        cout << "Error creating socket" << endl;
        return -1;
    }

    memset(&server, 0, sizeof(server));
    memset(&messageFromServer, 0, sizeof(messageFromServer));

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi("21"));
    server.sin_addr.s_addr = inet_addr("31.170.164.62"); //add adress of ftp server



    if(connect(serverSocket, (struct sockaddr *) &server, sizeof(server) ) < 0){
        //handle errors
        cout << "Error while connecting to the server" << endl;
        return -1;
    }


    int messageLength = recv(serverSocket, messageFromServer, sizeof(messageFromServer), 0);

    if(messageLength > 0){
        cout << messageFromServer << endl;
    }


    return 0;
}
