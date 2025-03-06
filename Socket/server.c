#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

char upper[25];
char lower[25];

void uppercase(char *w) {
    for (int i = 0; w[i] != '\0'; i++)
        upper[i] = toupper(w[i]);
    upper[strlen(w)] = '\0';
}

void lowercase(char *w) {
    for (int i = 0; w[i] != '\0'; i++)
        lower[i] = tolower(w[i]);
    lower[strlen(w)] = '\0';
}

int main() {
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

    unlink("my_socket");
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "my_socket");
    server_len = sizeof(server_address);
    if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) == -1) {
        perror("Binding Failed!!");
        close(server_sockfd);
		exit(1);
    }
    listen(server_sockfd, 5);

	printf("Server is waiting...\n");
    while(1) {
        char word[25];

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        if (client_sockfd == -1) {
			perror("Accept failed");
			continue;
		}

        read(client_sockfd, &word, 25);
        puts("Recived signal from Client...");
        if(strcmp(word, "end") == 0) {
            puts("Exiting Server!!");
            break;
        }

        uppercase(word);
        puts(upper);
        lowercase(word);
        puts(lower);

        close(client_sockfd);
    }

    return 0;
}