#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s IP_ADDRESS  PORT_NUMBER\n", argv[0]);
		exit(1);
	}
	int port = atoi(argv[2]);
	struct sockaddr_in server_address, return_address;
	char server_ip[50];
	memcpy(server_ip,argv[1],50); 
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id < 0){
		perror("Error in creating socket\n");
		exit(1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(server_ip);
	server_address.sin_port = htons(port);
	char message[128];
	printf("Enter string: ");
	scanf("%s", message);
	sendto(socket_id, message, strlen(message), 0, (struct sockaddr *) &server_address, sizeof(struct sockaddr_in));
	unsigned int return_len;
	while(recvfrom(socket_id, message, strlen(message), 0, (struct sockaddr*) &return_address, &return_len) > 0){
	       printf(message);
	}	      
	close(socket_id);
}
