#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX 100

struct my_msg{
	long int type;
	char data[MAX];
};

int main() {
	int msgid = msgget((key_t) 1233, 0666 | IPC_CREAT);
	if (msgid == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	struct my_msg msg;
	if(msgrcv(msgid, (void *)&msg, MAX, 1, 0) == -1){
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}
	else {
		printf(" %s", msg.data);
	}
	
	return 0;
}
