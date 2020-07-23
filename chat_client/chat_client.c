#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	char deststr[20];
	int tcp_port = 22629;
	int sock;
	int s;
	int numrcv;
	ssize_t n;
	char buf[255];
	struct sockaddr_in server;

	if(argc == 1) {
		printf("\nError : Input address\n\n");
		return 1;
	}

	strcpy(deststr, argv[1]);

	sock			  = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port	  = htons(22629);
	s				  = inet_pton(AF_INET, deststr, &server.sin_addr);
	if(s != 1) {
		printf("\nError : Failed to Connect\n\n");
		return 1;
	} else {
		/* IPv4 */
		if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
			printf("\nError : Failed to Connect\n\n");
			return 1;
		}

		memset(buf, 0, sizeof(buf));
		n = read(sock, buf, sizeof(buf));
		printf("%s\n", buf);

		while(1) {
			char comment[250];
			char writeComment[255];
			char buf[255];
			printf("a : ");
			scanf("%s", comment);
			sprintf(writeComment, "%s\r\n", comment);
			write(sock, writeComment, strlen(writeComment));
			if(strcmp(comment, ":q") == 0) {
				printf("closed\n");
				break;
			}
			numrcv = recv(sock, buf, 255, 0);
			printf("%s\n", buf);
		}
	}

	return 0;
}
