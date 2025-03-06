#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX 50
char num[MAX], bin[MAX], oct[MAX], hex[MAX];

struct my_msg {
	long int type;
	char data[MAX];
};

void toBases(int num) {
	int i =0, n = num;
	while(n > 0) {
		bin[i++] = (n % 2) + '0';
		n /= 2;
	}
	bin[i] = '\0';
	
	for(int j = 0; j < i/2; j++) {
		char temp = bin[i];
		bin[i] = bin[i-j-1];
		bin[i-j-1] = temp;
	}
	
	sprintf(oct, "%o", num);
	sprintf(hex, "%X", num);
	// puts(bin);
	// puts(hex);
	// puts(oct);
}

int main() {
	int msgid = msgget((key_t) 12345, 0666 | IPC_CREAT);
	int n;
	printf("Enter Decimal Number = ");
	scanf("%d", &n);
	
	toBases(n);
	
	while(1) {
		struct my_msg message1;
		message1.type = 1;
		strcpy (message1.data, "Decimal to Binary : ");
		strcat (message1.data, bin);
		msgsnd (msgid, (void *) &message1, sizeof (message1.data),0);
		
		struct my_msg message2;
		message2.type = 2;
		strcpy (message2.data, "Decimal to Octal : ");
		strcat (message2.data, oct);
		msgsnd (msgid, (void *) &message2, sizeof (message2.data),0);
		
		struct my_msg message3;
		message3.type = 3;
		strcpy (message3.data, "Decimal to Hexadecimal : ");
		strcat (message3.data, hex);
		msgsnd (msgid, (void *) &message3, sizeof (message3.data),0);
	}
	msgctl (msgid, IPC_RMID, 0);
	
	return 0;
}
