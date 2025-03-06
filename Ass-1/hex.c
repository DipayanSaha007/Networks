#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#define MAX 50

struct my_msg{
	long int type;
	char data[MAX];
};

int main(){
	int flag=1;
	int msgid = msgget((key_t) 12345, 0666 | IPC_CREAT);
	struct my_msg msg;
	while(flag == 1){
		msgrcv(msgid, (void*) &msg, MAX, 3, 0);
		if(strcmp(msg.data, "end") == 0){
			flag = 0;
			continue;
		}	
		printf("%s\n" ,msg.data);
		break;
		sleep(10);
	}
	return 0;
}
