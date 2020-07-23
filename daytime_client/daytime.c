#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv){
	char deststr[20];
	int tcp_port = 13;
	int sock;
	int s;
	ssize_t n;
	char buf[255];
	struct sockaddr_in server;

	if(argc == 1){
		printf("\nError : Input address\n\n");
		return 1;
	}

	strcpy(deststr, argv[1]);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(tcp_port);
	s = inet_pton(AF_INET, deststr, &server.sin_addr);
	if(s != 1){
		/* Not IPv4 */
		struct sockaddr_in6 server_in6;
		sock = socket(AF_INET6, SOCK_STREAM, 0);
		server_in6.sin6_family = AF_INET6;
		server_in6.sin6_port = htons(tcp_port);
		s = inet_pton(AF_INET6, deststr, &server_in6.sin6_addr);
		if(s != 1){
			/* Neither IPv4 or IPv6 */
			printf("\nError : Invalid Address\n\n");
			return 1;
		}

		/* IPv6 */
		if(connect(sock, (struct sockaddr *)&server_in6, sizeof(server_in6))){
			printf("\nError : Failed to Connect\n\n");
		}

		memset(buf, 0, sizeof(buf));
		n = read(sock, buf, sizeof(buf));
		printf("%s\n", buf);

	}else{
		/* IPv4 */
		if(connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1){
			printf("\nError : Failed to Connect\n\n");
		}

		memset(buf, 0, sizeof(buf));
		n = read(sock, buf, sizeof(buf));
		printf("%s\n", buf);
	}

	return 0;
}
