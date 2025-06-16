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
	int sockfd, len, result;
	struct sockaddr_in address;
	char data[MAX], res[MAX];
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(10021);
	len = sizeof(address);
	printf("Client is Running...\n");
	
	strcpy(data, "Hello from Client");
	printf("\nSending ' %s ' to the Server...\n", data);
	sendto(sockfd, data, strlen(data)+1, 0, (struct sockaddr *)&address, len);
	result = recvfrom(sockfd, res, MAX, 0, (struct sockaddr *)&address, &len);
	if(result == -1) {
		puts("Server not Found!!");
		perror("recvfrom");
		exit(1);
	} else {
		printf("Server Send: %s\n", res);
		strcpy(data, "");
		strcpy(res, "");
	}
	
	while(1) {
		printf("\nEnter Message: ");
		fgets(data, MAX, stdin);
		data[strcspn(data, "\n")] = '\0';
		
		printf("Sending ' %s ' to the Server...\n", data);
		sendto(sockfd, data, strlen(data)+1, 0, (struct sockaddr *)&address, len);
		if(strcmp(data, "end") == 0) {
			printf("\nExiting...\n");
			break;
		}
		puts("Waiting for Server...");
		recvfrom(sockfd, res, MAX, 0, (struct sockaddr *)&address, &len);
		printf("Server Send: %s\n", res);
	}
	
	close(sockfd);
	
	return 0;
}
