#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }

   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   int sum = 100;
   while(1){
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       while(1){
       sum = 0;
       bool valid = true;
       for(int i = 0; i < strlen(buf); i++){
               if(buf[i] == '\n') break;
               if(buf[i] == '\n' && i == 0){
                       valid = false;
                       break;
               }
               if(!isdigit(buf[i])){
                       valid = false;
                       break;
               }
               if(buf[i] == '\n') break;
               sum += (int)(buf[i]);
               sum -= 48;
       }
       if(!valid){
                sendto(sock,"Sorry, cannot compute!",22,0,(struct sockaddr *)&from,fromlen);
                break;
       }
       char response[128];
       bzero(response, 128);
       sprintf(response, "%d", sum);
       n = sendto(sock,response,strlen(response),
                  0,(struct sockaddr *)&from,fromlen);
       if (n < 0) error("sendto");
       if (sum < 10) break;
       bzero(buf,128);
       sprintf(buf, "%d", sum);
       }
   }
   close(sock);
   return 0;
 }

