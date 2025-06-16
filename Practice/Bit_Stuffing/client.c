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
	int sockfd, len;
	struct sockaddr_in address;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(10021);
	len = sizeof(address);
	
	if(connect(sockfd, (struct sockaddr *)&address, len) == -1) {
		printf("Can not connect to Server!!\n");
		perror("connect");
		exit(1);
	} else {
		printf("Connected to Server!!\n");
	}
	
	while(1) {
		char data[MAX], res[MAX];
		printf("Enter Bit Stream: ");
		fgets(data, MAX, stdin);
		data[strcspn(data, "\n")] = '\0';
		
		write(sockfd, data, strlen(data)+1);
		printf("\nSending ' %s ' to the Server...\n", data);
		if(strcmp(data, "end") == 0) {
			puts("Exiting...");
			break;
		}
		puts("Waiting for Server...");
		read(sockfd, res, MAX);
		printf("Server Send: %s\n", res);
	}
	close(sockfd);
	
	return 0;
}
