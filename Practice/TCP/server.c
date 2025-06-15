#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 100

int main() {
	int server_fd, client_fd, server_len, client_len;
	struct sockaddr_in server_address, client_address;
	char res[MAX];
	strcpy(res, "Message Received");
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(10021);
	server_len = sizeof(server_address);
	
	if(bind(server_fd, (struct sockaddr *)&server_address, server_len) == -1) {
		perror("bind");
		exit(1);
	}
	listen(server_fd, 5);
	
	client_len = sizeof(client_address);
	client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
	if(client_fd == -1) {
		perror("accept");
		exit(1);
	}
	
	while(1) {
		puts("Waiting for Client...");
		char data[MAX];
		
		read(client_fd, data, MAX);
		printf("\nClient Send: %s\n", data);
		if(strcmp(data, "end") == 0) {
			puts("Exiting...");
			break;
		}
		printf("Sending ' %s ' to Client\n", res);
		write(client_fd, res, strlen(res)+1);
	}
	close(server_fd);
	
	return 0;
}
