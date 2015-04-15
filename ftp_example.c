//ftp example
int ftpRecvResponse(int sock, char * buf) {
   int i;

   if (recv(sock, buf, 5120, 0) == -1) {//receive the data
      perror("recv");
      return 0;;
   }
   for(i=(strlen(buf) - 1);i>0;i--) {
      if(buf[i]=='.' || buf[i]=='\r') {
         buf[i+1]='\0';
         break;
      }
   }
   printf("%s\n",buf); //print response to the screen
   return 0;
}

int ftpNewCmd(int sock, char * buf, char * cmd, char * param) {
   strcpy(buf,cmd);
   if(strlen(param) > 0) {
      strcat(buf," ");
      strcat(buf,param);
   }
   strcat(buf,"\r\n");
   printf("*%s",buf); //print the cmd to the screen
   if(send(sock, buf, strlen(buf), 0) == -1) {
      perror("send");
      return 0;
   }
   //clear the buffer
   return 0;
}

int ftpConvertAddy(char * buf, char * hostname, int * port) {
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

int ftpSendFile(char * buf, char * host, int port) {
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
   closesocket(sd); //close the socket
   return 0;
}

int ftpProcess(char *hostname, char *username, char *password, char *file, char *filename, char *path) {
   int sd;
   struct sockaddr_in pin;
   struct hostent *hp;
   char tmpHost[100];
   int tmpPort;
   char buf[5120];

   if ((hp = gethostbyname(hostname)) == 0) {
      perror("gethostbyname");
      return -1;
   }

   memset(&pin, 0, sizeof(pin));
   pin.sin_family = AF_INET;
   pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
   pin.sin_port = htons(FTPPORT);

   if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      return -1;
   }
   if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
      perror("connect");
      return -1;
   }
   if(ftpRecvResponse(sd,buf)==1) {  //wait for ftp server to start talking
    if(strncmp(buf,"220",3)==0) {  //make sure it ends with a 220
     if(ftpNewCmd(sd,buf,"USER",username)==1) {  //issue the command to login
      if(ftpRecvResponse(sd,buf)==1) {  //wait for response
       if(strncmp(buf,"331",3)==0) {  //make sure response is a 331
        if(ftpNewCmd(sd,buf,"PASS",password)==1) {  //send your password
         if(ftpRecvResponse(sd,buf)==1) { //wait for response
          if(strncmp(buf,"230",3)==0) { //make sure its a 230
           if(ftpNewCmd(sd,buf,"PASV","")==1) {  //notify server we want to pass a file
            if(ftpRecvResponse(sd,buf)==1) {  //wait for response
             if(strncmp(buf,"227",3)==0) {  //make sure it starts with a 227
              ftpConvertAddy(buf,tmpHost,&tmpPort); //then convert the return to usable data
              if(ftpNewCmd(sd,buf,"STOR",filename)==1) {  //set the file name AND
               ftpSendFile(file,tmpHost,tmpPort);
               if(ftpRecvResponse(sd,buf)==1) {        //wait for a response
                if(strncmp(buf,"150",3)==0) {  //make sure its a 150 so we know the server got it all
                 if(ftpRecvResponse(sd,buf)==1) {
                  if(strncmp(buf,"226",3)==0) {
                  .......
//close all if()s or do it some better way, close the socket when done.
