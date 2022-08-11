#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DiewithError(char *errorMessage);
int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in echoServerAddr;
  unsigned short echoServPort;
  char *servIP;
  char *echoString;
  char echoBuffer[RCVBUFSIZE];
  unsigned int echoStringLen;
  int bytesRcvd, totalBytesRcvd;

  if((argc < 3) || (argc > 4)){
    fprintf(stderr,"Usage: %s<Server IP> <Echo Word>[<Echo Port>]¥n", argv[0]);
    exit(1);
  };

  servIP = argv[1];
  echoString = argv[2];

  if(argc == 4){
    echoServPort = atoi(argv[3]);
  }else{
    echoServPort = 7;
  }

  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    DiewithError("socket() failed");
  }

  memset(&echoServerAddr,0,sizeof(echoServerAddr));
  echoServerAddr.sin_family = AF_INET;
  echoServerAddr.sin_addr.s_addr = inet_addr(servIP);
  echoServerAddr.sin_port = htons(echoServPort);

if(connect(sock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0){
  DiewithError("connect() failed");
}

  echoStringLen = stlen(echoString);

  if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
    DiewithError("send() sent a different number of bytes than expected");
  }

  totalBytesRcvd = 0;
  printf("Recevid: ");
  while (totalBytesRcvd < echoStringLen){
    if((bytesRcvd = recv(sock,echoBuffer,RCVBUFSIZE -1, 0)) <= 0){
      DiewithError("recv() failed or connection closed prematurely");
    }
    totalBytesRcvd += bytesRcvd;
    echoBuffer[bytesRcvd] = '¥0';
    printf(echoBuffer);
  }
  printf("¥n");
  close(sock);
  exit(0);
}