/* COMP30023 Computer Systems
* Project 1
* 
* queue.h
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: Contains the header files for queues
* 
* Last modified: 12/4/16
*/

#define NUM_LEVELS 3
#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define Q1 2
#define Q2 4
#define Q3 8

typedef struct node {
	struct node *next;
	process_t process;
	int remaining_job_time;
	int level; /* priority level */
} node_t;

typedef node_t *node_ptr;

typedef struct fcfs_queue{
	node_ptr process_q;
	node_ptr last; /* pointer to last process in the queue for quick access */
} fcfs_t;

typedef fcfs_t *fcfs_ptr;

typedef struct multi_queue{
	fcfs_ptr queues[NUM_LEVELS];
} multi_t;

typedef multi_t *multi_ptr;

fcfs_ptr fcfs_init_queue();
void fcfs_add_process(fcfs_ptr queue, process_t process);
void fcfs_remove_process(fcfs_ptr queue, node_ptr process);
void fcfs_enqueue(fcfs_ptr queue, node_ptr process);
node_ptr fcfs_get_process(fcfs_ptr queue);

multi_ptr multi_init_queue();
void multi_enqueue(multi_ptr queue, node_ptr process);
node_ptr multi_get_process(multi_ptr queue);