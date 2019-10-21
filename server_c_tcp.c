/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[128];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while(1){
		newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, 
				&clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		bzero(buffer,128);
		n = read(newsockfd,buffer,128);
		printf(buffer);
		printf("%ld",strlen(buffer));
		printf("\n");
		if(n < 0){
			error("Error in reading\n");
			exit(1);
		}
		bool valid = true;
		// case with non-number
		for(int i = 0; i < strlen(buffer); i++){
			if(buffer[i] == '\n')
				break;
			if(!isdigit(buffer[i])){
		
				write(newsockfd, "Sorry, cannot compute!",22);
				valid = false;
				break;
			}
		}
		// normal cas
		if(valid){
			int sum = 0;
			for(int i = 0; i < strlen(buffer); i++){
				if(buffer[i] == '\n'){
					break;
				}
				printf("%d \n", buffer[i]);
				sum += (int)(buffer[i]);
			}
			//printf("sum %d",sum);
			char response[128];
			bzero(response, 128);
			sprintf(response, "%d", sum);
			n = write(newsockfd,response,strlen(response));
			if (n < 0) error("ERROR writing to socket");
			close(newsockfd);
		}
	}
	close(sockfd);
	return 0; 
}
