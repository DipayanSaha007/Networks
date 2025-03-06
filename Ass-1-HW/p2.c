#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>

#define MAX 25
#define MAX_NAME_LEN 25

struct my_msg {
	long int type;
	char d_name[MAX][25];
	int d_roll[MAX];
	int N;
};

void sort_names(char names[][MAX_NAME_LEN], int n) {
	char temp[MAX_NAME_LEN];
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (strcmp(names[i], names[j]) > 0) {
				strcpy(temp, names[i]);
				strcpy(names[i], names[j]);
				strcpy(names[j], temp);
			}
		}
	}
}

int main() {
	int msgid = msgget((key_t)12345, 0666 | IPC_CREAT);
	if (msgid == -1) {
		perror("msgget failed");
		exit(EXIT_FAILURE);
	}

	struct my_msg msg;
	if (msgrcv(msgid, (void*)&msg, sizeof(msg) - sizeof(msg.type), 1, 0) == -1) {
		perror("msgrcv failed");
		exit(EXIT_FAILURE);
	}

	sort_names(msg.d_name, msg.N);

	// Send Sorted Names
	struct my_msg msg1;
	msg1.type = 3;
	msg1.N = msg.N;
	memcpy(msg1.d_name, msg.d_name, sizeof(msg.d_name));

	if (msgsnd(msgid, (void*)&msg1, sizeof(msg1) - sizeof(msg1.type), IPC_NOWAIT) == -1) {
		perror("msgsnd for sorted names failed");
		exit(EXIT_FAILURE);
	}

	puts("Names are Sorted & Message Sent Successfully!!");
	return 0;
}

