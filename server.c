#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[]){

	// invalud number of argument
	if(argc != 2) {
		return 1;
	}

	//char server_message[256] = "You have reached the server.\n";
	int port = atof(argv[1]);

	// create the server socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define the server address
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind the socket to our specified IP and port
	if (-1 == bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address))){
		return 1;
	}

	if (-1 == listen(server_socket, 2)){
		return 1;
	}

	for(;;){
		int client_socket;
		client_socket = accept(server_socket, NULL, NULL);
		
		// receive filename
		char filename[256];
		recv(client_socket, &filename, sizeof(filename), 0);
		//printf("The client sent the filename: %s\n", filename);
		
		// open the file for reading
		FILE *fptr;
		fptr = fopen(filename, "rb");
		if (fptr == NULL){
			//fprintf(stderr, "Open file for reading failed.\n");
		}
		else{
			char buffer[256];
			int b;
			while ((b = fread(buffer, 1, sizeof(buffer), fptr)) > 0){
				send(client_socket, buffer, b, 0);
			}
			fclose(fptr);
		}


		// send the message
		//send(client_socket, server_message, sizeof(server_message), 0);

		// close
		close(client_socket);
	}
	close(server_socket);
	
	return 0;
}
