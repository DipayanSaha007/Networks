#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	while(1) {
		char bit[25];
		printf("Enter bit value (Enter 'end' to exit)= ");
		scanf("%s", bit);

		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, "server_socket");
		len = sizeof(address);
		
		result = connect(sockfd, (struct sockaddr *)&address, len);
		if(result == -1) {
			perror("oops: client1");
			exit(1);
		}
		
		write(sockfd, &bit, 25);
		if(strcmp(bit, "end") == 0) {
			puts("Client is Closing!!");
			break;
		}
		read(sockfd, &bit, 25);
		printf("Recived from the server with parity bit = %s\n", bit);
	}
	close(sockfd);
	
	return 0;
}
