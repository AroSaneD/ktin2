#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "SocketObject.h"

/*
    Minimum requirements:

    TYPE - ASCII Non-print
         MODE - Stream
         STRUCTURE - File, Record
         COMMANDS - USER, QUIT, PORT,
                    TYPE, MODE, STRU,
                      for the default values
                    RETR, STOR,
                    NOOP.
*/

class FtpClient{
public:
    FtpClient(string port, string address);
    bool login();                               //USER & PASS
    bool listCurrentDirectory();                //LIST
    bool changeDirectory(string directory);     //CWD
    bool uploadFile(string fileName);           //PASV & STOR
    bool getFile(string fileName);
    bool exit();                                //QUIT

protected:

private:
    SocketObject serverSocket;

};

/*
    server responses:
        331 - user verified, need pass
        230 - Password confirmed
        425 - No data connections (list)
        250 - Successful directory change (cwd)
*/

#endif // FTPCLIENT_H
