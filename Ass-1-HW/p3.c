#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>

#define MAX 25

struct my_msg {
	long int type;
	char d_name[MAX][25];
	int d_roll[MAX];
	int N;
};

void sort_rolls(int rolls[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rolls[i] > rolls[j]) {
                temp = rolls[i];
                rolls[i] = rolls[j];
                rolls[j] = temp;
            }
        }
    }
}

int main() {
	int msgid = msgget((key_t) 12345, 0666 | IPC_CREAT);
	struct my_msg msg;
	if (msgrcv(msgid, (void*) &msg, sizeof(msg) - sizeof(msg.type), 2, 0) == -1) {
		perror("msgrcv failed");
		exit(EXIT_FAILURE);
	}
	
	sort_rolls(msg.d_roll, msg.N);
	
	// Send Sorted Roll Numbers
	struct my_msg msg2;
	msg2.type = 4;
	msg2.N = msg.N;
	memcpy(msg2.d_roll, msg.d_roll, sizeof(msg.d_roll));

	if (msgsnd(msgid, (void *) &msg2, sizeof(msg2) - sizeof(msg2.type), IPC_NOWAIT) == -1) {
		perror("msgsnd for sorted roll numbers failed");
		exit(EXIT_FAILURE);
	}
	puts("Roll Numbers are Sorted & Message Sent Successfully!!");
	return 0;
}

