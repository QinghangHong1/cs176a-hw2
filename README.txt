Partner: Jing Su 6839039
	 Qinghang Hong 5775481

TCP Client/Server:
	1. Server socket is created, binded to the port specified by the command line argument, then it is listening to requests.
	2. Client socket is created, then it establish a connection with the server socket.
	3. Server socket creates a socket for the client and establish a connection with the client.
	4. Client gets a string from the user and "writes" it to the server socket
	5. Server "reads" the string and store it in buffer
	6. Server adds up the digits, "writes" the sum to the client
	7. Server checks if the sum < 10, if not, wait 0.01s and repeats step 6. If so, close the socket for the client but continue to listen to other connections
	8. Client reads the data from server one by one and output to the screen until server closes the connection.
	9. Client closes its socket.
	
UDP Client/Server:
	1. There's no handshaking between the client and server
	2. Server listens on the destinated port, waiting for the message from client
	3. Client sends message to the server port
	4. Server processe the message and send back multiple outputs
	5. When the client doesn't receive any message, it closes the socket.
	6. Server keeps listening on the same port, waiting for messages from other clients.
