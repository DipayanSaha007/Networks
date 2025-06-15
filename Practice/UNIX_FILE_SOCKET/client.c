#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100

int main() {
	int sockfd;
	int len;
	struct sockaddr_un address;
	
	while(1) {
		char data[MAX];
		printf("Enter Message: ");
		fgets(data, MAX, stdin);
		data[strcspn(data, "\n")] = '\0';
		
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, "server_socket");
		len = sizeof(address);
		
		if(connect(sockfd, (struct sockaddr *)&address, len) == -1) {
			perror("connect");
			exit(1);
		}
		
		printf("Sending Message to Server...\n");
		write(sockfd, &data, strlen(data)+1);
		if(strcmp(data, "end") == 0) {
			puts("Exiting...");
			break;
		}
		puts("Waiting for Server...");
		read(sockfd, &data, MAX);
		printf("Server Send: %s\n", data);
	}
	close(sockfd);
	
	return 0;
}
