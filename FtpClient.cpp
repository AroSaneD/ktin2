#include "FtpClient.h"


int transcribeInfo(char * buf, char * hostname, int * port){
    //the host and port will be in the client class

    unsigned int i,t=0;
    int flag=0,decCtr=0,tport1,tport2;
    char tmpPort[6];
    //example data in quotes below:
    //"227 Listening on (149,122,52,162,4,20)"
    //4 * 256 + 20 = 1044
    for(i=0;i<strlen(buf);i++) {
        if(buf[i]=='(') {
            flag = 1;
            i++;
        }
        if(buf[i]==')') {
            hostname[t]='\0';
            break;
        }
        if(flag==1) {
            if(buf[i] != ',') {
                hostname[t]=buf[i];
                t++;
            } else {
                hostname[t]='.';
                t++;
            }
        }
    }
    t=0;
    for(i=0;i<strlen(hostname);i++) {
    if(hostname[i]=='.')
        decCtr++;
    if(decCtr==4 && hostname[i]!='.') {
        tmpPort[t]=hostname[i];
        t++;
        if(hostname[i+1]=='.') {
            tmpPort[t]='\0';
            tport1=atoi(tmpPort);
            t=0;
        }
    }
    if(decCtr==5 && hostname[i]!='.') {
        tmpPort[t]=hostname[i];
        t++;
        if(hostname[i+1]=='\0') {
            tmpPort[t]='\0';
            tport2=atoi(tmpPort);
            t=0;
        }
    }
    }
    *port = tport1 * 256 + tport2;
    decCtr=0;
    for(i=0;i<strlen(hostname);i++) {
    if(hostname[i]=='.') {
        decCtr++;
    }
    if(decCtr>3)
        hostname[i]='\0';
    }
    return 0;
}


int sendFile(char * buf, char * host, int port){
    //the host and port will be taken form the client class
    //ftpsendfile

       int sd;
       struct sockaddr_in pin;
       struct hostent *hp;

       if ((hp = gethostbyname(host)) == 0) {
          perror("gethostbyname");
          return -1;
       }
       memset(&pin, 0, sizeof(pin));
       pin.sin_family = AF_INET;
       pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
       pin.sin_port = htons(port);
       if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
          perror("socket");
          return -1;
       }
       if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
          perror("connect");
          return -1;
       }
       if(send(sd, buf, strlen(buf), 0) == -1) {
          perror("send");
          return -1;
       }
       close(sd); //close the socket
       return 0;

}

int recvFile(char * buf, char * host, int port){
    //the host and port will be taken form the client class
    //ftpsendfile

       int sd;
       struct sockaddr_in pin;
       struct hostent *hp;

       if ((hp = gethostbyname(host)) == 0) {
          perror("gethostbyname");
          return -1;
       }
       memset(&pin, 0, sizeof(pin));
       pin.sin_family = AF_INET;
       pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
       pin.sin_port = htons(port);
       if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
          perror("socket");
          return -1;
       }
       if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
          perror("connect");
          return -1;
       }

       if (recv(sd, buf, sizeof(buf), 0) < 0){
            //handle errors
       }


       close(sd); //close the socket
       return 0;

}

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
    serverSocket.sendMessage("USER u363531489\n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    //Send password and then wait for its verification
    serverSocket.sendMessage("PASS L9XO8VBU82\n"); //Kazkodel serveris nepriima slaptazodzio kai siunciu dvi uzklausaus
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;
    return true;
}


bool FtpClient::listCurrentDirectory(){
    string messageFromServer;

    serverSocket.sendMessage("NLST \n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;
    return true;
}

bool FtpClient::changeDirectory(string directory){
    string messageFromServer;


    serverSocket.sendMessage("CWD " + directory + "\n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;
    return true;
}

bool FtpClient::uploadFile(string fileName){
    string messageFromServer;
    char buf[1024], hostname[256], file[256];
    int port;


    serverSocket.sendMessage("PASV \n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    strncpy(buf, messageFromServer.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = 0;


    transcribeInfo(buf, hostname, &port);

    serverSocket.sendMessage("STOR " + fileName +"\n");

    strncpy(file, fileName.c_str(), sizeof(file));
    file[sizeof(file) - 1] = 0;

    sendFile("Insert file text here", hostname, port);

    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    return true;
}

bool FtpClient::getFile(string fileName){
    string messageFromServer;
    char buf[1024], hostname[256], file[256];
    int port;

    serverSocket.sendMessage("PASV \n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;

    strncpy(buf, messageFromServer.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = 0;


    transcribeInfo(buf, hostname, &port);

    serverSocket.sendMessage("RETR " + fileName + "\n");


    recvFile(file, hostname, port);
    cout << file << endl;

    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }
    cout << messageFromServer << endl;

    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }
    cout << messageFromServer << endl;

    return true;
}

bool FtpClient::exit(){
    string messageFromServer;

    serverSocket.sendMessage("QUIT \n");
    if(serverSocket.receiveResponse(messageFromServer) < 0){
        responeError();
        return false;
    }

    cout << messageFromServer << endl;
    return true;
}

//end of file
