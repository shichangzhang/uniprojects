/* queue.h
 *
 * COMP30023 Computer Systems
 * Project 2
 * Semester 1, 2016
 *
 * by Shi Chang Zhang, 695434, shiz2
 *
 * Last modified: 25/5/2016
 *
 * Description: Implementation of a queue for
 * storing client threads.
 */

#define MAX_THREADS 300

typedef struct node{
	pthread_t tid;
	int sockfd;		
	struct node *next;
} node_t;

typedef node_t *node_ptr;

typedef struct queue{
	int count;
	node_ptr head;
	node_ptr tail;
} queue_t;

typedef queue_t *queue_ptr;

queue_ptr q_init();
void q_add(queue_ptr q, pthread_t tid, int sockfd);
void q_delete(queue_ptr q, pthread_t tid);


