#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[]){

	// invalid number of arguments
	if(argc != 5){
		return 1;
	}

	int port = atof(argv[2]);

	// create a socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);

	// specify an address for the socket
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	if(inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0){
		return 1;
	}

	int connection = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	// check if connection is successful
	if(connection == -1){
		return -1;
	}

	char filename[256];
	strcpy(filename, argv[3]);
	send(network_socket, filename, sizeof(filename), 0);

	// receive data from server
	char new_filename[256];
	strcpy(new_filename, argv[4]);
	
	char buff[1025];
	int b;
	b = recv(network_socket, buff, 1024, 0);
	if(b <= 0){
		//fprintf(stderr, "Nothing was sent.\n");
		return 1;
	}
	else{
		FILE *fp = fopen(new_filename, "wb");
		if(fp != NULL){
			while(b > 0){
				fwrite(buff, 1, b, fp);
				b = recv(network_socket, buff, 1024, 0);
			}
		}
		else{
			return 1;
		}
		fclose(fp);
	}
	// receive data from server
	// char server_response[256];
	// recv(network_socket, &server_response, sizeof(server_response), 0);

	// print out the server's response
	// printf("The server sent the data: %s\n", server_response);

	// close the socket
	close(network_socket);
	

	return 0;
}
