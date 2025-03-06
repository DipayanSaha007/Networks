#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX 25

struct my_msg {
	long int type;
	char d_name[MAX][25];
	int d_roll[MAX];
	int N;
};

int main() {
	char name[MAX][25];
	int roll[MAX];
	int msgid = msgget((key_t) 12345, 0666 | IPC_CREAT);
	int N;

	printf("Enter Number of Students = ");
	scanf("%d", &N);

	for(int i = 0; i < N; i++) {
		printf("Enter Student %d Name: ", i + 1);
		scanf("%s", name[i]);
		printf("Enter Student %d Roll: ", i + 1);
		scanf("%d", &roll[i]);
	}

	// Send Names
	struct my_msg msg1;
	msg1.type = 1;
	msg1.N = N;
	for (int i = 0; i < N; i++) {
		strcpy(msg1.d_name[i], name[i]);
	}
	if (msgsnd(msgid, (void *) &msg1, sizeof(msg1) - sizeof(msg1.type), IPC_NOWAIT) == -1) {
		perror("msgsnd for names failed");
		exit(EXIT_FAILURE);
	}

	// Send Roll Numbers
	struct my_msg msg2;
	msg2.type = 2;
	msg2.N = N;
	memcpy(msg2.d_roll, roll, N * sizeof(int));
	if (msgsnd(msgid, (void *) &msg2, sizeof(msg2) - sizeof(msg2.type), IPC_NOWAIT) == -1) {
		perror("msgsnd for roll numbers failed");
		exit(EXIT_FAILURE);
	}

	printf("Messages Sent Successfully!\n");
	
	// Receive Sorted Roll Numbers
	struct my_msg msg4;
	if (msgrcv(msgid, (void*) &msg4, sizeof(msg4) - sizeof(msg4.type), 4, 0) == -1) {
		perror("msgrcv failed");
		exit(EXIT_FAILURE);
	}
	printf("Sorted Roll Numbers: ");
	for(int i = 0; i < N; i++) {
		printf("%d\t", msg4.d_roll[i]);
	}
	printf("\n");
	
	// Receive Sorted Names
	struct my_msg msg3;
	if (msgrcv(msgid, (void*) &msg3, sizeof(msg3) - sizeof(msg3.type), 3, 0) == -1) {
		perror("msgrcv failed");
		exit(EXIT_FAILURE);
	}
	printf("Sorted Names: ");
	for(int i = 0; i < N; i++) {
		puts(msg3.d_name[i]);
	}
	// msgctl (msgid, IPC_RMID, 0);

	return 0;
}

