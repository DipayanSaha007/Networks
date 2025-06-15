#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100

int main() {
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	
	unlink("server_socket");
	
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	
	if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) == -1) {
		perror("bind");
		exit(1);
	}
	
	listen(server_sockfd, 5);
	
	char res[MAX];
	strcpy(res, "Server got Message");
	
	while(1) {
		char data[MAX];
		printf("Waiting for Client...\n");
		
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		if(client_sockfd == -1) {
			perror("accept");
			break;
		}
		
		read(client_sockfd, &data, MAX);
		printf("Client Send: %s\n", data);
		if(strcmp(data, "end") == 0) {
			puts("Exiting...");
			break;
		}
		write(client_sockfd, &res, strlen(res)+1);
		printf("Server Send: ' %s ' back to the Client...\n", res);
		
		close(client_sockfd);
	}
	
	return 0;
}
