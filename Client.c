#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int clientSocket, ret,n=0;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Message from Server - %s\n", buffer);
			if(strstr(buffer, "Already Register.") != NULL){
				close(clientSocket);
				printf("[-]Disconnected from server.\n");
				exit(1);
			}
			else if(strstr(buffer, "Login Failed") != NULL){
				close(clientSocket);
				printf("[-]Disconnected from server.\n");
				exit(1);
		}

		}

		bzero(buffer, sizeof(buffer));
		printf("Enter message for Server - ");
		//gets(buffer);
		//fgets(buffer, 100, stdin);
		n=0;
	        while ((buffer[n++] = getchar()) != '\n');
		send(clientSocket, buffer, strlen(buffer), 0);
		if(strstr(buffer, "exit") != NULL){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			//signal(SIGINT);
			exit(1);
		}
		bzero(buffer, sizeof(buffer));	
	}

	return 0;
}
