/* server.c
 *
 * COMP30023 Computer Systems
 * Project 2 
 * Semester 1, 2016
 *
 * by Shi Chang Zhang, 695434, shiz2
 *
 * Last modified: 24/5/2016
 *
 * Description: A server for the game Mastermind. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "mastermind.h"
#include "queue.h"

#define NUMARGS1 3
#define NUMARGS2 2
#define BUFF_LEN 256
#define SERVER -1
#define CONNECTION 0
#define GUESS 1
#define REPLY 2
#define END -1

void *clientaccepter(void *sockfd);
void *gameloop(void *newsockfd);
void logentry(char *msg, int clientfd, int entrytype);
void getclientinfo(char *buffer, int clientfd);

// Dem global variables
char secret_code[CODE_SIZE+1] = {0};
int num_successes = 0;
int num_clients = 0;
queue_ptr client_q;

// Semaphores
sem_t mutex;
sem_t mutex_successes;
sem_t mutex_client;

// Catches SIGINT and does stuff
void signalhandler(int signal){

	FILE *proc = fopen("/proc/self/statm","r");
	char buffer[BUFF_LEN] = {0};
	node_ptr temp = client_q->head;
	long int memory;
		
	if(proc == NULL){
		printf("[ERROR] Couldn't open proc file.\n");
		exit(0);
	}
	
	if(signal == SIGINT){
		printf("received SIGINT\n"
			"Time to exit...\n");
		sprintf(buffer,"Total connected clients=%d\n"
			"Number of successes=%d\n"
			"Server shutdown",
			num_clients,
			num_successes);	
		logentry(buffer,SERVER,END);
		bzero(buffer,BUFF_LEN);
		while(temp!=NULL){
			printf("%d\n",(unsigned int)temp->tid);
			write(temp->sockfd,
			"You have been disconnected by the server.",
			strlen("You have been disconnected by the server."));
			close(temp->sockfd);
			pthread_kill(temp->tid,SIGTERM);
			pthread_join(temp->tid,NULL);
			temp = temp->next;
		}

		fscanf(proc,"%ld",&memory);
		sprintf(buffer,"Virtual Memory used = %ld bytes\n",
		memory*sysconf(_SC_PAGESIZE));			
		logentry(buffer,SERVER,END);
		bzero(buffer,BUFF_LEN);

		fscanf(proc,"%ld",&memory);
		sprintf(buffer,"Physical Memory used = %ld bytes\n",
		memory*sysconf(_SC_PAGESIZE));			
		logentry(buffer,SERVER,END);	
	
		close(proc);
		exit(0);
	}
}

void thread_cleanup(int signal){
	printf("Disconnecting Client...\n");
	pthread_exit(NULL);	
}

int main(int argc, char **argv){

	signal(SIGINT, signalhandler);
	signal(SIGTERM, thread_cleanup);

	// Used sample code from LMS as a reference
	int sockfd, portno;
	int valid=1;	
	int yes=1; // For setsockopt()

	struct sockaddr_in serv_addr;
	
	if(argc==NUMARGS1 || argc == NUMARGS2){
		portno = atoi(argv[1]);
		if(0 > portno || portno > 65536){
			printf("[ERROR] Port number out of range. (0 to 65536)\n");
			exit(1);	
		}
	} else {
		printf("Usage: ./server [port_number] [secret_code]\n");
		exit(1);
	}
	
	// Checks if sercret code is provided, makes one if it isn't.
	if(argc == NUMARGS1 && (valid=validcode(argv[2]))){
		int i;
		for(i=0;i<CODE_SIZE;i++){
			secret_code[i]=(char)toupper(argv[2][i]);
		}	
		printf("Your secret code has been set to: %s\n",secret_code);	
	} else {
		if(!valid){
			printf("Your code was not valid.\n"
			"This is what you entered: %s\n"
			"A valid code has %d colours, which are"
			" one of (%s)\n",argv[2],CODE_SIZE,VALID_COLOURS);
		}
		printf("Your secret code will be randomized.\n");	
	}
	
	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		
	
	if(sockfd < 0){
		printf("[ERROR] Opening socket failed.\n");
		exit(1);
	}
	
	// Take control of a port
	if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1){
		printf("[ERROR] Can't steal port.\n");
	}	

	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	// Set up serv_addr's attributes
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// Bind address to socket
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("[ERROR] Binding socket failed.\n");
		exit(1);
	}
	
	// Init semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&mutex_successes, 0, 1);
	sem_init(&mutex_client, 0, 1);	

	// Init client tid queue.
	client_q = q_init();	

	// Make thread for accepting connections
	pthread_t listen_thread;
	pthread_create(&listen_thread,NULL,clientaccepter,&sockfd);
	pthread_join(listen_thread,NULL);	

	// Close socket
	close(sockfd);

	return 0;
}  

// Thread for accpeting new TCP connections
void *clientaccepter(void *listen_sockfd){
	int sockfd = *(int *)listen_sockfd;
	int newsockfd, clilen;
	struct sockaddr cli_addr;
	pthread_t client;
	// Listen on socket
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	// Accept loop
	while(1){
		// Accept a connection
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0){
			printf("[ERROR] Accepting connection failed.\n");
			pthread_exit(NULL);
		}
		logentry("client connected", newsockfd, CONNECTION);
		pthread_create(&client,NULL,gameloop,&newsockfd);
		num_clients++;
	}
	pthread_exit(NULL);
}

// The Mastermind Gameloop
void *gameloop(void *newsockfd){
	int clientsockfd = *(int *)newsockfd;
	
	//add tid to client_q.
	sem_wait(&mutex_client);
	q_add(client_q,pthread_self(), clientsockfd);
	sem_post(&mutex_client);	
		
	int b,m,n,response;	
	int moves=MAX_ATTEMPTS;
	FILE *fd = fdopen(clientsockfd, "w");
	FILE *welcome = fopen("welcome.txt","r");
	char buffer[BUFF_LEN] = {0};
	char game_code[CODE_SIZE+1] = {0};
	char hint[6] = {0};
	char reply_buffer[BUFF_LEN] = {0};

	// Set the secret code.
	if(validcode(secret_code)){
		strcpy(game_code,secret_code);
	} else {
		randomcode(game_code);
	}

	// Make log entry for code 
	logentry(game_code, SERVER, CONNECTION);	

	bzero(buffer,BUFF_LEN);
	
	printf("A Client has connected to the server\n");

	// Send welcome message to client.
	while(fgets(buffer,BUFF_LEN,welcome)!=NULL){
		n = write(clientsockfd,buffer,strlen(buffer));
		bzero(buffer,BUFF_LEN);
		n = read(clientsockfd,buffer,BUFF_LEN-1);
		bzero(buffer,BUFF_LEN);
	}
	
	bzero(buffer,BUFF_LEN);

	while(moves){
		// Read characters from connection
		n = read(clientsockfd,buffer,BUFF_LEN-1);
		
		if(n == 0){
			break;
		} if(n < 0){
			printf("[ERROR] Reading from socket.\n");	
			break;
		}
		
		// Get rid of newline character. 		
		if(buffer[strlen(buffer)-1] == '\n'){
			buffer[strlen(buffer)-1] = 0;
		}
			
		printf("Client's guess was: %s\n",buffer);
		
		// Log the clients guess.
		logentry(buffer, clientsockfd, GUESS);

		// Game logic.
		if(validcode(buffer) == NOT_VALID){			
			logentry(INVALID,SERVER,REPLY);	
			if(moves == 1){
				strcpy(reply_buffer,GAME_OVER);
				logentry(FAILURE,clientsockfd,REPLY);
				break;
			} else {
				strcpy(reply_buffer,INVALID);
			}
		} else {
			response = makeguess(buffer,game_code,moves);
			b = rightincode(buffer,game_code);
			m = countincode(buffer,game_code);
			sprintf(hint,"[%d:%d]",b,m-b);
			logentry(hint,SERVER,REPLY);
			
			if(response == CORRECT){
				strcpy(reply_buffer,SUCCESS);
				logentry(SUCCESS,clientsockfd,REPLY);
				sem_wait(&mutex_successes);
				num_successes++;
				sem_post(&mutex_successes);
				break;
			} else if(response == OUT_OF_MOVES){
				strcpy(reply_buffer,GAME_OVER);
				logentry(FAILURE,clientsockfd,REPLY);
				break;
			} else {
				strcpy(reply_buffer,"Server's hint ");
				strcat(reply_buffer,hint);
			}	
		}

		fprintf(fd,"%s",reply_buffer);
		fflush(fd);
		bzero(reply_buffer,BUFF_LEN);
		bzero(buffer,BUFF_LEN);

		if(moves > 0){
			moves--;
		}
	}
	
	fprintf(fd,"%s",reply_buffer);
	fflush(fd);
	close(fd);
	close(clientsockfd);
	printf("A Client has disconnected.\n");
	pthread_exit(NULL);
}

// Makes a entry in log.txt with a timestamp.
void logentry(char *msg, int clientfd, int entrytype){
	sem_wait(&mutex);
	FILE *log = fopen("log.txt","a");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char buffer[BUFF_LEN];
	
	bzero(buffer,BUFF_LEN);

	if(entrytype == END){
		strcpy(buffer,msg);
		fprintf(log,"%s\n",buffer);
	} else {	
		getclientinfo(buffer, clientfd);
		if(entrytype == GUESS){
			strcat(buffer, " client's guess = ");
		} else if(entrytype == REPLY && clientfd == SERVER){
			strcat(buffer, " server's reply = ");
		} else if(entrytype == CONNECTION && clientfd == SERVER){
			strcat(buffer, " server secret = ");
		} else {
			strcat(buffer, " ");
		} 
		
		strcat(buffer, msg);
	
		if(strcmp(msg,SUCCESS) == 0 || strcmp(msg,FAILURE) == 0){
			strcat(buffer, " game over");
		}
	
		// timestamp code from stackoverflow
		fprintf(log,"[%02d %02d %d %02d:%02d:%02d] %s\n", 
			tm.tm_mday, 
			tm.tm_mon + 1, 
			tm.tm_year + 1900, 
			tm.tm_hour, 
			tm.tm_min, 
			tm.tm_sec,
			buffer);	
	}
	
	
	fclose(log);
	sem_post(&mutex);
}

// Returns a clients IP address and socket id as a string.
void getclientinfo(char *buffer, int clientfd){
	if(clientfd < 0){
		sprintf(buffer,"(0.0.0.0)");
	} else {	
		struct sockaddr_in client_addr;
		socklen_t clilen = sizeof(client_addr);
		getpeername(clientfd, &client_addr, &clilen);
		sprintf(buffer, "(%s)(soc_id %d)",
			inet_ntoa(client_addr.sin_addr), 
			clientfd);	
	}
}
