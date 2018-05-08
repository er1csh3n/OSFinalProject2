/*
OS FINAL
Server.CPP
*/
/*
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT8080
int main(int argc, char const *argv[])
{
  int server_fd, new_socket, valread;
  struct sockaddr
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


FILE *fp;
const int PATH_MAX = 1024;
char path[PATH_MAX];

#define PORT 4444

int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Connection Failed\n");
		exit(1);
	}
	printf("!!! Server Developed !!!\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("!!! Binding Error !!!\n");
		exit(1);
	}
	printf("!!! Binded !!! %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("!!! Waiting for Client :\n");
	}else{
		printf("!!! Binding Error !!!\n");
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
				recv(newSocket, buffer, 1024, 0);

				if(strcmp(buffer, ":exit\n") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					close(newSocket);
					break;
				}
				else if(strcmp(buffer, "pwd\n") == 0){
					fp = popen("pwd","r");
					while (fgets(path,PATH_MAX,fp) != NULL) {
						send(newSocket,path,strlen(path),0);
					}

					pclose(fp);
				}
				else if(strcmp(buffer,":timer\n")==0){
						send(newSocket,buffer,strlen(buffer),0);
				}
				else{
					printf("Client: %s\n", buffer);
					/*
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
					*/
					fp = popen(buffer,"r");
					while (fgets(path,PATH_MAX,fp) != NULL) {
						send(newSocket,path,strlen(path),0);
					}
					fp=popen("","r");
					pclose(fp);
				}
				
			}
		}

	}

	close(newSocket);


	return 0;
}
