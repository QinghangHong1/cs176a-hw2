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
		if(n < 0){
			error("Error in reading\n");
			exit(1);
		}
		bool valid = true;
		// case with non-number
		for(int i = 0; i < strlen(buffer); i++){
			if(!isdigit(buffer[i])){
				printf("index %d\n", i);
				write(newsockfd, "Sorry, cannot compute!",22);
				valid = false;
				break;
			}
		}
		// normal case
		if(valid){
			int sum = 0;
			for(int i = 0; i < strlen(buffer); i++){
				sum += (int)(buffer[i]);
			}
			int temp = 0;
			int i = 0;
			// store digit of sum in reverse order
			while(sum > 0){
				buffer[i] = sum % 10;
				sum = sum / 10;
				i++;
			}
			char response[i];
			for(int j = 0; j < i; j++){
				response[j] = buffer[i - j - 1];
			}
			n = write(newsockfd,response, i);
			if (n < 0) error("ERROR writing to socket");
			close(newsockfd);
		}
	}
	close(sockfd);
	return 0; 
}
