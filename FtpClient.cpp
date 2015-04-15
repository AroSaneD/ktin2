#include "FtpClient.h"

void responeError(){
    cout << "An error occurred while receiving repsonse" << endl;
}

FtpClient::FtpClient(string port, string address){

    serverSocket = SocketObject(port, address);

}

bool FtpClient::login(){

    string messageFromServer;


    if(serverSocket.connectToServer() < 0){
        cout << "Connection failed";
        return false;
    }

    //Wait for connection verification (Code 220)
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    //Send user name and then wait for its verification
    serverSocket.sendMessage("USER u363531489\r\n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    //Send password and then wait for its verification
    serverSocket.sendMessage("PASS memftl6TmE\n"); //Kazkodel serveris nepriima slaptazodzio kai siunciu dvi uzklausaus
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    return true;
}


bool FtpClient::listCurrentDirectory(){
    string messageFromServer;

    serverSocket.sendMessage("LIST \n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;
}
