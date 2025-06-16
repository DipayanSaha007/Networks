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
	int server_fd, server_len, client_len, checkres;
	struct sockaddr_in server_address, client_address;
	char data[MAX], res[MAX];
	
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(10021);
	server_len = sizeof(server_address);
	bind(server_fd, (struct sockaddr *)&server_address, server_len);
	printf("Server is Running...\n");
	
	strcpy(res, "Hello from Server");
	client_len = sizeof(client_address);
	checkres = recvfrom(server_fd, data, MAX, 0, (struct sockaddr *)&client_address, &client_len);
	if(checkres == -1) {
		puts("Client not Found!!");
		perror("sendto");
		exit(1);
	} else {
		printf("\nReceived from Client: %s\n", data);
		sendto(server_fd, res, strlen(res)+1, 0, (struct sockaddr *)&client_address, client_len);
		printf("Server Send: %s\n", res);
		strcpy(data, "");
		strcpy(res, "");
	}
	strcpy(res, "Message Received");
	
	while(1) {
		printf("\nWaiting for Client...\n");
		recvfrom(server_fd, data, MAX, 0, (struct sockaddr *)&client_address, &client_len);
		printf("\nClient Send: %s\n", data);
		if(strcmp(data, "end") == 0) {
			printf("\nExiting...\n");
			break;
		}
		printf("Server sending ' %s ' to Client...\n", res);
		sendto(server_fd, res, strlen(res)+1, 0, (struct sockaddr *)&client_address, client_len);
	}
	
	close(server_fd);
	
	return 0;
}
