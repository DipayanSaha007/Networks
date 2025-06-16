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

void xor(char* divident, char* divisor,int divisorlen) {
	for(int i = 0; i < divisorlen; i++) {
		divident[i] = (divident[i] == divisor[i]) ? '0' : '1';
	}
}

void crc(char* divident,char* divisor,char* res) {
	int divlen = strlen(divident);
	int divisorlen = strlen(divisor);
	char temp[MAX];
	
	strcpy(temp, divident);
	memset(temp + divlen, '0', divisorlen - 1);
	temp[divlen + divisorlen - 1] = '\0';
	
	for(int i = 0; i < divlen; i++) {
		if(temp[i] == '1') {
			xor(&temp[i], divisor, divisorlen);
		}
	}
	
	char remainder[MAX];
	strncpy(remainder, &temp[divlen], divisorlen - 1);
	remainder[divisorlen - 1] = '\0';
	
	strcpy(res, divident);
	strcat(res, remainder);
}

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
		char divident[MAX], divisor[MAX], res[MAX];
		
		read(client_fd, divident, MAX);
		printf("\nClient Send Divident: %s\n", divident);
		if(strcmp(divident, "end") == 0) {
			puts("Exiting...");
			break;
		}
		read(client_fd, divisor, MAX);
		printf("Client Send Divisor: %s\n", divisor);
		
		crc(divident, divisor, res);
		
		printf("Sending ' %s ' to Client\n", res);
		write(client_fd, res, strlen(res)+1);
	}
	close(server_fd);
	
	return 0;
}
