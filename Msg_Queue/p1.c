#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX 100

struct my_msg {
    long int type;
    char data[MAX];
};

int main() {
    int msgid = msgget((key_t) 1234, 0666 | IPC_CREAT);
    char bit[MAX];
    while(1) {
        printf("Enter 8 byte bit stream(Enter 'end' to exit): ");
        fgets(bit, MAX, stdin);  // Use fgets instead of scanf to read spaces

        bit[strcspn(bit, "\n")] = '\0';  // Remove newline character

        struct my_msg msg1;
        msg1.type = 1;
        memset(msg1.data, 0, MAX);
        strcpy(msg1.data, bit);
        if(msgsnd(msgid, (void*)&msg1, sizeof(msg1.data), 0) == -1) {
            perror("msgsnd failed!!\n");
            exit(EXIT_FAILURE);
        }
        if(strcmp(msg1.data, "end") == 0) {
            puts("Exiting Code!!");
            break;
        }
    }

    return 0;
}