#include "FtpClient.h"


using namespace std;


/*
Panaudota ismintis ir kodo pavyzdziai is:
    http://tools.ietf.org/html/rfc959

    http://stackoverflow.com/questions/13762673/how-to-program-an-ftp-client-in-c
    http://forums.devshed.com/ftp-help-113/ftp-program-84623.html
    http://www.freefire.org/articles/ftpexample.php
server:
    hostinger.lt
*/


int main()
{
    char c;
    FtpClient ftpClient = FtpClient("21", "31.170.164.222");
    ftpClient.login();
    //ftpClient.listCurrentDirectory();
    //ftpClient.uploadFile("ftp_example.c");
    ftpClient.getFile("uploaded.c");
    cin >> c;
    ftpClient.exit();





    return 0;
}
