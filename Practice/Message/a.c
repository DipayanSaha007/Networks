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

	char d[MAX];
	printf("Enter a Message: ");
	fgets(d, MAX, stdin);
	
	struct my_msg msg;
	msg.type = 1;
	strcpy(msg.data, "The Message is: ");
	strcat(msg.data, d);
	if(msgsnd(msgid, (void *)&msg, sizeof(msg.data), 0) == -1){
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
	else {
		puts("Message sent successfully!!");
	}
	
	return 0;
}
