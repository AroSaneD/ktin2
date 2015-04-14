#include "SocketObject.h"


using namespace std;


/*
to read up on:
    http://stackoverflow.com/questions/13762673/how-to-program-an-ftp-client-in-c
    http://tools.ietf.org/html/rfc959
server:
    hostinger.lt
*/

bool login(SocketObject serverSocket){
    string messageFromServer;


    if(serverSocket.connectToServer() < 0){
        cout << "Connection failed";
        return -1;
    }

    //Wait for connection verification (Code 220)
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        cout << "An error occurred while receiving the respoonse";
        return -1;
    }

    cout << messageFromServer << endl;

    //Send user name and then wait for its verification
    serverSocket.sendMessage("USER u363531489\nPASS aiVn1EfacW\n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        cout << "An error occurred while receiving the response";
        return -1;
    }

    cout << messageFromServer << endl;

    //Send password and then wait for its verification
    //serverSocket.sendMessage("PASS memftl6TmE\n"); //Kazkodel serveris nepriima slaptazodzio kai siunciu dvi uzklausaus
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        cout << "An error occurred while receiving the response";
        return -1;
    }

    cout << messageFromServer << endl;

    return 1;
}

int main()
{

    SocketObject serverSocket = SocketObject("21", "31.170.164.222");

    login(serverSocket);





    return 0;
}
