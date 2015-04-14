#include "FtpClient.h"


using namespace std;


/*
to read up on:
    http://stackoverflow.com/questions/13762673/how-to-program-an-ftp-client-in-c
    http://tools.ietf.org/html/rfc959
server:
    hostinger.lt
*/


int main()
{
    FtpClient ftpClient = FtpClient("21", "31.170.164.222");
    ftpClient.login();







    return 0;
}
