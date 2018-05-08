#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <iomanip>


using namespace std;

#define PORT 4444

int main(){
	time_t result = time(NULL);
	int clientSocket, ret;
	/* Specifies a local or remote endpoint address to connect a socket to */
	struct sockaddr_in serverAddr;
	char buffer[1024];

	/*
		socket(int domain , int type, int protocol)
			domain = AF_INET = Designates type of addresses that socket can communicate with. AF_INET safest
			type = SOCK_STREAM = connection based protocol, conversation until termination.
			protocol = 0 = Causes socket to use an unspecified default protocol depending on socket type
	*/
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	/*End Program if No connection*/
	if(clientSocket < 0){
		printf("!!! Connection Failed !!!\n");
		exit(1);
	}
	printf("!!! Socket Connected !!!\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET; /*Type of address family*/
	serverAddr.sin_port = htons(PORT); /*Host To Network Short HToNS -> Converts values between host and network byte orders */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*IP Address of the socket*/

	/*
		Connect(int socket, const struct sockaddr *address, socklen_t address_len)
		socket = clientSocket = Specifies the file associated with the Socket
		address = serverAddr = Points to the sockaddr structure that has the peer addresses
												Length and format depends on address family (AF_INET)
		address_len = serverAddr = Specifies length of sockaddr
	*/
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	/* If there is an error in connection, ret will be -1 */
	if(ret < 0){
		printf("!!! Connection Error !!!\n");
		exit(1);
	}
	printf("!!! Connected !!!\n");
	printf("Type \":timer\" for throughput and response time\n");
	printf("Type \":exit\" to exit the client\n");

	/* Infinite Loop, until forced exit */

	while(1){
		buffer[0] = '\0';
		/*Clear Buffer*/

		printf("Client: \t");
		/* scanf(const char *format)
			 format = "%S"
			 &buffer[0] = input
		*/

		/*sscanf("%s", &buffer[0]);*/
		fgets(buffer,1024,stdin);
		/*
			send(int socket, const void *buffer, size_t llength, int flags)
			socket = clientSocket = File Descriptor
			buffer = buffer = points to buffer that contains the message that will be sent
			strlen(buffer) = Specifies string length in bytes
			Flags = 0 = Type of message transition = No flags specified
		*/
		send(clientSocket, buffer, strlen(buffer), 0);

		/* Compares the buffer string to ":Exit" if True, connection is closed */

		if(strcmp(buffer,":exit\n")==0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(strcmp(buffer, ":timer\n") == 0) {

			auto tClient = chrono::system_clock::now();
			printf("Server: \n %s\n ", buffer);
			auto tServer = chrono::system_clock::now();
			int elapsed = chrono::duration_cast<chrono::nanoseconds>(tServer-tClient).count();
/*		float elapsedS = chrono::duration_cast<chrono::seconds>(tServer-tClient).count();    */

			cout << "Time Elapsed : " << elapsed << " nanoseconds" << endl
					<< "Throughput : " << (float)1024/elapsed << " kB/ns" << endl;

			system("wget -o report.txt --report-speed=bits 'https://www.realgm.com/'");

			cout << "WGET Throughput : " << endl;
			system("tail -2 report.txt | awk -F ')' '{print $1}'");


		}

		/*
			recv(int socket, void buffer, size_t length, int flags)
			similar to send()
			Receives a message from a Socket
			If error (< 0) Error
		*/
		if(recv(clientSocket, buffer, 1024, 0) < 0 && strcmp(buffer,":timer\n") != 0){
			printf("!!! Error !!!\n");
		}else{
			printf("Server: \n%s\n ", buffer);

		}
	}

	return 0;
}
