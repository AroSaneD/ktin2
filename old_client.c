/*
    Autorius: Arunas Seniucas PS2, 2-as pogrupis
    Uzduotis: Failu serveris

    CLIENT FILE
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


/*
Naudota kodo pavyzdziai is:
	http://www.binarytides.com/winsock-socket-programming-tutorial/
	http://tldp.org/LDP/LGNET/91/misc/tranter/server.c.txt
	http://stackoverflow.com/questions/22370036/socket-programming-in-c-sending-a-file-from-server-to-client
*/

#pragma comment(lib,"Ws2_32.lib") //Winsock Library

void getip(char* , char* );
void recieve_file(int socket);

int main()
{
    WSADATA wsa;


    int s_socket;
    struct sockaddr_in server;


    char message[100];
    char reply[100];
    int message_length, reply_length, list_length;
    int choice = -1;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        getchar();
        return -1;
    }

    if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error occured while creating socket. Error code: %d", WSAGetLastError());
        getchar();
        return 1;
    }

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi("8888"));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(s_socket, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Error occured while connecting to the server. Error code: %d", WSAGetLastError());
        getchar();
        return 1;
    }

    memset(&reply, 0, sizeof(reply));
    recv(s_socket, reply, sizeof(reply), 0);


    list_length = atoi(reply);

    int i;
    for(i=0;i<list_length;i++)
    {
        memset(&reply, 0, sizeof(reply));
        recv(s_socket, reply, sizeof(reply), 0);
        printf("%d. %s.\n", i, reply);
    }



    while(choice < 0  || choice >list_length)
    {
        printf("\nEnter the number of the file you wish to download: ");
        scanf("%d", &choice);
    }

    memset(message, 0, sizeof(message));
    sprintf(message, "%d", choice);
    send(s_socket, message, strlen(message), 0);

    FILE *outputFile = fopen("output", "wb");

    if(outputFile == NULL)
    {
        fprintf(stderr, "Something went south!");
        exit(0);
    }

    char recvBuff[256];
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytesReceived = recv(s_socket, recvBuff, sizeof(recvBuff), 0);
    puts("Message received");
    while(bytesReceived != 0)
    {
      fwrite(recvBuff, bytesReceived, 1, outputFile);
      puts("Written to file");
      bytesReceived = recv(s_socket, recvBuff, 10, 0);
      if(strcmp(recvBuff, "End confirmed")) break;
    }
    puts("End confirmed");


    fclose(outputFile);










    closesocket(s_socket);
    WSACleanup();

    return 0;
}

void recieve_file(int socket)
{
    FILE *outputFile = fopen("output", "wb");

    if(outputFile == NULL)
    {
        fprintf(stderr, "Something went south!");
        exit(0);
    }

    char recvBuff[512];
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytesReceived = recv(socket, recvBuff, 10, 0);
    while(bytesReceived != 0)
    {
      fwrite(recvBuff, bytesReceived, 1, outputFile);
      bytesReceived = recv(socket, recvBuff, 10, 0);
    }

    fwrite(recvBuff, bytesReceived, 1, outputFile);

    fclose(outputFile);
}

void getip(char* hostname, char* ip)
{
    //The method gets the ip of a given website
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if(  (he = gethostbyname(hostname)) == NULL)
    {
        printf("Failed to get ip address. Error code: %d", WSAGetLastError());
        exit(0);
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }

}
