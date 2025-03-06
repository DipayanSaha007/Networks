#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    while(1) {
        char word[25];
        printf("Enter Name(Enter 'end' to exit): ");
        scanf("%s", word);

        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        address.sun_family = AF_UNIX;
        strcpy(address.sun_path, "my_socket");
        len = sizeof(address);

        result = connect(sockfd, (struct sockaddr *)&address, len);
        if(result == 1) {
            perror("Client Failed!!");
            exit(1);
        }

        write(sockfd, &word, 25);
        puts("Client send signal to Server...");
        if(strcmp(word, "end") == 0) {
            puts("Exiting Client!!");
            break;
        }
    }
    close(sockfd);

    return 0;
}