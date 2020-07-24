#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	struct sockaddr_in server;
	char deststr[20];
	char buf[255];
	char comment[250];
	char writeComment[255];
	char userName[255];
	int tcp_port = 22629;
	int sock;
	int s;
	int numrcv;
	ssize_t n;
	fd_set readfds;

	if(argc == 1) {
		printf("\nError : Input address\n\n");
		return 1;
	}

	printf("Input your name ---> ");
	scanf("%s", &userName);

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
		printf("%s", buf);
		sprintf(writeComment, ":u %s", userName);
		write(sock, writeComment, strlen(writeComment));

		FD_ZERO(&readfds);
		while(1) {
			FD_SET(0, &readfds);
			FD_SET(sock, &readfds);
			select(sock + 1, &readfds, NULL, NULL, NULL);
			if(FD_ISSET(0, &readfds)) {
				fgets(comment, 255, stdin);
				sprintf(writeComment, "%s", comment);
				write(sock, writeComment, strlen(writeComment));
				if(strncmp(comment, ":q", 2) == 0) {
					printf("Closed\nGood Bye!\n\n");
					close(sock);
					break;
				}
			} else if(FD_ISSET(sock, &readfds)) {
				numrcv = recv(sock, buf, 255, 0);
				printf("%s", buf);
			}
		}
	}

	return 0;
}
