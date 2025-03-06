#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

char bt[25];

void addParity(char *b) {
	int i = 0, count = 0;
	fflush(stdin);
	while(b[i] != '\0') {
		if(b[i] == '1') {
			count++;
		}
		i++;
	}
	if(count%2 == 0) {
		b[i] = '0';
	}
	else {
		b[i] = '1';
	}
	b[i+1] = '\0';
}

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
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	listen(server_sockfd, 5);
	while(1) {	
		char bit[25];
		printf("Server is Waiting\n");
		
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		
		read(client_sockfd, &bit, 25);
		if(strcmp(bit, "end") == 0) {
			puts("Server is Closing!!");
			break;
		}
		fflush(stdin);
		addParity(bit);
		write(client_sockfd, &bit, 25);
		close(client_sockfd);
	}
	
	return 0;
}
