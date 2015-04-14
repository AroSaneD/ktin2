#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "SocketObject.h"

class FtpClient{
public:
    FtpClient(string port, string address);
    bool login();


protected:

private:
    SocketObject serverSocket;

};

#endif // FTPCLIENT_H
