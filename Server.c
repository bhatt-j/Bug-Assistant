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

	int sockfd, ret,count=0,count2=0,count3=0,count4=0;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	char buffer2[1024];
	char temp[512];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				printf("%d\n",count);	
				if(count == 0){
					strcpy(buffer,"Hello");
					count++;
				}
				else if(count == 1){
					strcpy(buffer,"\nDo you have register?");
					count++;
				}	
				else if(count == 2 && strstr(buffer,"no") != NULL || count4 == 1){
					if(count == 2){
						strcpy(buffer,"\nEnter your mobile number for registration :: ");
						count4++;	
						count++;
					}
					else if(count4 == 1){
						FILE *fp , *fp1;
						fp = fopen("Database.txt", "r");
						while(fgets(temp, 512, fp) != NULL) {
								if((strstr(temp, buffer)) != NULL) {
									strcpy(buffer,"\nAlready Register.");
									fclose(fp);
									count3=1;
									count4=2;
									break;
								}
							}

						if(count3 == 0){
							count4=2;
							count2 = 2;
							fp = fopen("Database.txt", "a");
							fputs(buffer, fp);
							fclose(fp);
							strcat(buffer,".txt");
							fp1 = fopen(buffer, "w");
							strcpy(buffer,"\nRegister successfully");
							fclose(fp1);
						}
					}

				}
				else if(count == 2 && strstr(buffer,"yes") != NULL || count2 == 1){
					if(count == 2){
						strcpy(buffer,"\nEnter register mobile no :: ");
						count2++;
						count++;
					}
					else if(count2 == 1){
						FILE *fp , *fp1;
						fp = fopen("Database.txt", "r");
						while(fgets(temp, 512, fp) != NULL) {
								if((strstr(temp, buffer)) != NULL) {
									strcpy(buffer,"\nLogin Successfully.");
									count3=1;
									count2++;
									break;
								}
							}

						if(count3 == 0){
							strcpy(buffer,"\nLogin Failed");
							count2++;
						}
					}					
				}
				else if(count2 == 2){
					strcpy(buffer,"\nWelcome\nDo you want any help");
					strcat(buffer2,buffer);
					printf("buffer2 : %s",buffer2);
					count2++;
				}
				else if(count2 == 3 && strstr(buffer,"yes") != NULL){
					strcpy(buffer,"Tell your doubt");
					strcat(buffer2,buffer);
					printf("buffer2 : %s",buffer2);
					count2++;
				}				
				else if(count2 == 3 && strstr(buffer,"no") != NULL){
					strcpy(buffer,"Tell about you something");
					strcat(buffer2,buffer);
					printf("buffer2 : %s",buffer2);
					count2++;
				}				
				send(newSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));				
				
				recv(newSocket, buffer, 1024, 0);
				printf("%s",buffer);
			}
		}

	}

	close(newSocket);


	return 0;
}
