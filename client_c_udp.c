//the starter code was copied from linuxhowtos.org/data/6/client_udp.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[128];

   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr,
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   
// taking input
   printf("Enter string: ");
   bzero(buffer,128);
   fgets(buffer,128,stdin);
   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");
   bzero(buffer,128);
   
// receive data from server
   while(1){
   n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length);
   if (n < 0) break;
   printf("From Server: %s\n",buffer);
      
// check for exit conditions
   if(strlen(buffer) < 2) break;
   if(buffer[0] == 'S') break;
   bzero(buffer,128);
   }
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
