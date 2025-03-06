#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void xorOperation(char *dividend, const char *divisor, int divisorLen) {
	for (int i = 0; i < divisorLen; i++) {
		dividend[i] = (dividend[i] == divisor[i]) ? '0' : '1';
	}
}

void crcCompute(char *dataword, char *divisor, char *codeword) {
	int dataLen = strlen(dataword);
	int divisorLen = strlen(divisor);
	char temp[50];

	strcpy(temp, dataword);
	memset(temp + dataLen, '0', divisorLen - 1);
	temp[dataLen + divisorLen - 1] = '\0';

	for (int i = 0; i <= dataLen - 1; i++) {
		if (temp[i] == '1') {
			xorOperation(&temp[i], divisor, divisorLen);
		}
	}

	char remainder[50];
	strncpy(remainder, &temp[dataLen], divisorLen - 1);
	remainder[divisorLen - 1] = '\0';

	strcpy(codeword, dataword);
	strcat(codeword, remainder);
}

int main() {
	int server_sockfd, client_sockfd;
	struct sockaddr_un server_address, client_address;
	socklen_t client_len;
	char dataword[50], divisor[50], codeword[50];

	unlink("server_socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_sockfd == -1) {
		perror("Socket creation failed");
		exit(1);
	}

	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");

	if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		perror("Binding failed");
		close(server_sockfd);
		exit(1);
	}

	listen(server_sockfd, 5);
	printf("Server is waiting...\n");

	while (1) {
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		if (client_sockfd == -1) {
			perror("Accept failed");
			continue;
		}

		read(client_sockfd, dataword, sizeof(dataword));
		if (strcmp(dataword, "end") == 0) {
			puts("Server is Closing!!");
			close(client_sockfd);
			break;
		}
		read(client_sockfd, divisor, sizeof(divisor));

		crcCompute(dataword, divisor, codeword);
		write(client_sockfd, codeword, sizeof(codeword));

		close(client_sockfd);
	}

	close(server_sockfd);
	unlink("server_socket");
	return 0;
}

