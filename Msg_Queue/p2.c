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

char newBit[MAX];

void add_1(char bit[MAX]) {
    int n = strlen(bit);
    int count = 0, j = 0;
    for(int i = 0; i <= n; i++) {
        if(bit[i] == '0') {
            count ++;
        }
        else {
            count = 0;
        }
        newBit[j++] = bit[i];
        if (count == 5) {
            newBit[j++] = '1'; // Insert '1' after five consecutive '0's
            count = 0; // Reset count after bit stuffing
        }
    }
    newBit[j] = '\0';
}

int main() {
    int msgid = msgget((key_t) 1234, 0666 | IPC_CREAT);
    struct my_msg msg2;
    while(1) {
        if(msgrcv(msgid, (void*)&msg2, MAX, 1, 0) == -1) {
            perror("msgrcv failed!!\n");
            exit(EXIT_FAILURE);
        }
        if(strcmp(msg2.data, "end") == 0) {
            puts("Exiting Code!!");
            break;
        }
        printf("8 byte bit stream recived = %s\n", msg2.data);
        
        add_1(msg2.data);
        printf("Bit stream after bit stuffing: %s\n", newBit);
    }
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}