#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s PORT_NUMBER\n", argv[0]);
		exit(1);
	}
	int PORT = atoi(argv[1]);

	int socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//server socketid, like file handler
	if(socket_id == -1){
		perror ( "ERROR in creating socket\n");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in address;// server address
	address.sin_family = AF_INET; //IPv4
	address.sin_addr.s_addr = INADDR_ANY; //Accept from any address public
	address.sin_port = htons( PORT );// 

	if(bind(socket_id, (struct sockaddr *) &address, sizeof(address)) < 0){
		perror("error in binding socket\n");
		exit(1);
	}
	while(1){
		struct sockaddr_in client_address;
		char message[128];
		char response[100];
		unsigned int return_len;
		if(recvfrom(socket_id, message, 128, 0, 
					(struct sockaddr*) &client_address, &return_len) < 0){
			perror("Error in receiving packets\n");
			exit(1);          
		}
		bool valid = true;
		for(int i = 0; i < strlen(message); i++){
			// case with non-number chars
			if(!isdigit(message[i])){
				strcpy(response, "From server: Sorry, cannot compute!\n");
				int response_length = strlen(response);
				valid = false;
				if(sendto(socket_id, "Sorry, cannot compute!\n", 22, 0, (const struct sockaddr*) &client_address, return_len) < 0){
					perror("Error in sending the invalid message packet\n");
					exit(1);
				};
				break;
			}
		}
		// normal case
		if(valid){
			if(sendto(socket_id, response, return_len, 0, (struct sockaddr*) &client_address, sizeof(struct sockaddr_in)) < 0){
				perror("Error in sending the packets\n");
				exit(1);
			}
			int sum = 0;
			int length = strlen(response);
			while(length > 1){
				// sum up the digit
				for(int i = 0; i < length; i++){
					sum += response[i];
				}
				int i = 0;
				// store the digit in reverse order in response
				while(sum > 0){
					response[i] = sum % 10;
					sum = sum / 10;
					i++;
				}
				char true_response[i];
				// reverse the order of the digits and store in true_response
				for(int j = 0; j < i; j++){
					true_response[j] = response[i - j - 1];
				}
				// send the true response
				if(sendto(socket_id, true_response, i, 0, (struct sockaddr*) &client_address, sizeof(struct sockaddr_in)) < 0){
					perror("Error in sending the packets\n");
					exit(1);
				}
				// send a newline
				/*char new_line[] = "\n";
				  if(sendto(socket_id, new_line, 1, 0, (struct sockaddr*) &client_address, sizeof(struct sockaddr_in)) < 0){
				  perror("Error in sending the packets\n");
				  exit(1);
				  }*/
				// update the length of response
				length = i;
			}	
		}

		close(socket_id);

	}
}
