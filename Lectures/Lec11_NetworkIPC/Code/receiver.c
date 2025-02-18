#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[]){
	printf("receiver %s:%d\n", argv[1], atoi(argv[2]));
	int domain = AF_INET;//Network Protocol: TCP/IP
	int type = SOCK_DGRAM;//Connectionless
	int protocol = 0;//Default transport: UDP for Internet connectionless

	int receiver_sd;//socket descriptor ~= file descriptor
	receiver_sd = socket(domain, type, protocol);
	if (receiver_sd == -1){
		printf("error in creating socket!\n");
		exit(1);
	}
	else
		printf("socket has created for receiver with sd:%d\n", receiver_sd);

	//Binding to an address is a must for receiver!
	struct in_addr receiver_sin_address;
	receiver_sin_address.s_addr = inet_addr(argv[1]);//nslookup `hostname`
	int receiver_sin_port = htons(atoi(argv[2]));//larger than 1024

	struct sockaddr_in receiver_sin;
	receiver_sin.sin_family = domain;
	receiver_sin.sin_addr = receiver_sin_address;
	receiver_sin.sin_port = receiver_sin_port;
	int result = bind(receiver_sd, (struct sockaddr *) &receiver_sin, sizeof(receiver_sin));
	if (result == -1){
		printf("error in binding receiver to the address:port = %d:%d\n", receiver_sin.sin_addr, receiver_sin.sin_port);
		exit(1);
	}
	else
		printf("receiver bound to the address:port = %d:%d\n", receiver_sin.sin_addr, receiver_sin.sin_port);


	//wait for a message ...
	struct sockaddr_in sender_sin;//I want to know who send the message
	char mailbox[100];
	int sender_sin_len;
	setbuf(stdout, NULL);
	while(1)
	{
		result = recvfrom(receiver_sd, mailbox, sizeof(mailbox), 0, (struct sockaddr *) &sender_sin, &sender_sin_len);
		if (result == -1){
			printf("error in opening mail from sender!\n");
			exit(1);
		}
		else
			printf("the content of mail is: %s", mailbox);
	}

}
