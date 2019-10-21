CC = gcc
ARGS = -Wall


# Compiling all the dependencies
all: server_c_udp client_c_udp server_c_tcp client_c_tcp

# Replace <"your_program"> with the name of your specififc program. 
# For example, the next line may look something like this: 'server_c_udp: server_c_udp.c' without quotes.
server_c_udp: server_c_udp.c
	$(CC) $(ARGS) -o server_c_udp server_c_udp.c

client_c_udp: client_c_udp.c
	$(CC) $(ARGS) -o client_c_udp client_c_udp.c

server_c_tcp: server_c_tcp.c
	$(CC) $(ARGS) -o server_c_tcp server_c_tcp.c

client_c_tcp: client_c_tcp.c
	$(CC) $(ARGS) -o client_c_tcp client_c_tcp.c

clean:
	rm -f *.o server_c_udp *~
	rm -f *.o client_c_udp *~
	rm -f *.o server_c_tcp *~
	rm -f *.o client_c_tcp *~
