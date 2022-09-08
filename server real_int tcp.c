#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff1[MAX];
	char buff2[100];
	int n,real=0;
	// infinite loop for chat
	for (;;) {
		bzero(buff1, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff1, sizeof(buff1));
		// print buffer which contains the client contents
		printf("From client: %s", buff1);
		bzero(buff2, MAX);
		// and send that buffer to client
		n=sizeof(buff1);
		int m=0;
		for(int i=0;i<n;i++)
		{
		if(buff1[i]=='.')
		{
		real=1;
		break;
		}
		}
		printf("To client : ");
		if(real==1)
		strcpy(buff2,"Real");
		else
		strcpy(buff2,"Integer");
		printf("from server : %s",buff2);
		//for(int i=0;i<sizeof(buff2);i++)
		//{
		//printf("%c",buff2[i]);
		//}
		printf("\n");
		write(connfd, buff2, sizeof(buff2));
		real=0;

		// if msg contains "Exit" then server exit and chat ended.
		//if (strncmp("exit", buff1, 4) == 0) {
		//	printf("Server Exit...\n");
		//	break;
		//}
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}