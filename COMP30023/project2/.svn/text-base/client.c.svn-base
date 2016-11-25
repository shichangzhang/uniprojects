/* client.c
 *
 * COMP30023 Computer Systems
 * Project 2 
 * Semester 1, 2016
 *
 * by Shi Chang Zhang, 695434, shiz2
 *
 * Last modified: 24/5/2016
 *
 * Description: A client for the game Mastermind. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "mastermind.c"

#define NUMARGS 3
#define BUFF_LEN 256

int makeprintable(char *buffer, int len);

int main(int argc, char **argv){
	
	// Used sample code from LMS as reference
	int sockfd, portno, n;	
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[BUFF_LEN];

	if(argc==NUMARGS){
		portno = atoi(argv[2]);
		// Translate host name into IP address.
		server = gethostbyname(argv[1]);
		if(server == NULL){
			printf("[ERROR] No such host\n");
			exit(1);
		}
	} else {
		printf("Usage: ./client [IP_address] [port_number]\n");
		exit(1);
	}
		
	// Build data structure for socket.
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, 
		(char *) &serv_addr.sin_addr.s_addr,
		server->h_length);

	serv_addr.sin_port = htons(portno);

	// Create TCP socket.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		printf("[ERROR] Opening socket failed.\n");
		exit(1);	
	}
	
	// Connect to server.
	if(connect(sockfd,(struct sockaddr *)&serv_addr,
		 sizeof(serv_addr)) < 0 ){
		printf("[ERROR] Couldn't connect.\n");
		exit(1);
	}

	// Get server's welcome message.
	n = read(sockfd,buffer,BUFF_LEN-1);
	while(n>0){
		printf("%s",buffer);
		bzero(buffer,BUFF_LEN);
		if(n == 2){
			write(sockfd,buffer,BUFF_LEN-1);
			break;
		}
		n = write(sockfd,buffer,BUFF_LEN-1);
		n = read(sockfd,buffer,BUFF_LEN-1);
	}

	// Print instructions.
	printf("Please enter a %d colour code\n"
		"Valid colours are (%s)\n"
		"You have %d tries.\n",CODE_SIZE,VALID_COLOURS,MAX_ATTEMPTS);
		
	// Read input for game.
	while(1){
		bzero(buffer,BUFF_LEN);
		fgets(buffer,BUFF_LEN-1,stdin);	
		
		n = write(sockfd,buffer,strlen(buffer));

		if(n < 0){
			printf("[ERROR] Writing to socket failed.\n");
			exit(1);
		}
		
		// Get server's response.
		bzero(buffer,BUFF_LEN);
		n = read(sockfd,buffer,BUFF_LEN-1);
		if(n < 0){
			printf("[ERROR] Reading from socket failed.\n");
		}
		printf("%s\n",buffer);

		if(strcmp(buffer,SUCCESS) == 0){
			printf("\n\n~Congratulations! You have won!~\n\n");
			break;
		} else if(strcmp(buffer,GAME_OVER) == 0){
			printf("\n\nIt's all over! You're out of moves!\n\n");
			break;
		}
	}

	close(sockfd);	

	return 0;
}
