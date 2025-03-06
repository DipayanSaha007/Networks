#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int sockfd;
	struct sockaddr_un address;
	char dataword[50], divisor[50], codeword[50];

	while (1) {
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (sockfd == -1) {
			perror("Socket creation failed");
			exit(1);
		}

		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, "server_socket");

		if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
			perror("Connection failed");
			close(sockfd);
			exit(1);
		}
		
		printf("Enter dataword (binary) (Enter 'end' to exit): ");
		scanf("%s", dataword);
		write(sockfd, dataword, sizeof(dataword));
		if (strcmp(dataword, "end") == 0) {
			break;
		}
		
		printf("Enter divisor (binary): ");
		scanf("%s", divisor);
		write(sockfd, divisor, sizeof(divisor));

		read(sockfd, codeword, sizeof(codeword));

		printf("Received codeword from server: %s\n", codeword);
		close(sockfd);
	}

	printf("Client is Closing!!\n");
	return 0;
}

