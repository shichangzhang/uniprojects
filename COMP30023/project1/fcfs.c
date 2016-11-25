/* COMP30023 Computer Systems
* Project 1
* 
* fcfs.c
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: 
* 
* Last modified: 12/4/16
*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"
#include "memory.h"


fcfs_ptr fcfs_init_queue(){
	fcfs_ptr queue = (fcfs_ptr)malloc(sizeof(fcfs_t));
	if(queue == NULL){
		fprintf(stderr,"Error: Failed to initialize fcfs queue\n");
		exit(EXIT_FAILURE);	
	}
	queue->process_q = NULL;
	queue->last = NULL;
	
	return queue;
}

void fcfs_add_process(fcfs_ptr queue, process_t process){
	if(queue == NULL){
		fprintf(stderr,"Error: Queue is NULL\n");
		exit(EXIT_FAILURE);	
	}
	if((queue->last)!=NULL){
		(queue->last)->next = (node_ptr)malloc(sizeof(node_t));
		if((queue->last)->next==NULL){
			fprintf(stderr,"Error: Failed to add process\n");
			exit(EXIT_FAILURE);
		}
		queue->last = (queue->last)->next;
	} else {
		queue->process_q = (node_ptr)malloc(sizeof(node_t));
		if((queue->process_q)==NULL){
			fprintf(stderr,"Error: Failed to add process\n");
			exit(EXIT_FAILURE);
		}
		queue->last = queue->process_q;
	}
	
	(queue->last)->next = NULL;
	(queue->last)->process = process;
	(queue->last)->remaining_job_time = process.job_time;
	(queue->last)->level = LEVEL1;
}

void fcfs_remove_process(fcfs_ptr queue, node_ptr process){
	
	if(queue == NULL){
		fprintf(stderr,"Error: Queue is NULL\n");
		exit(EXIT_FAILURE);	
	}
	if(process == NULL){
		fprintf(stderr,"Error: Process is NULL\n");
		exit(EXIT_FAILURE);	
	}
	
	node_ptr temp = process->next;
	node_ptr p = queue->process_q;
	
	while(p!=NULL){
		if(p->next != NULL){
			if(p->next == process){
				p->next = temp;
				free(process);
				return;
			}
			p = p->next;	
		}
	}
	fprintf(stderr,"Error: Process not found in queue\n");
	exit(EXIT_FAILURE);	
}

void fcfs_enqueue(fcfs_ptr queue, node_ptr process){
	if(queue == NULL){
		fprintf(stderr,"Error: Queue is NULL\n");
		exit(EXIT_FAILURE);	
	}
	if(process == NULL){
		fprintf(stderr,"Error: Process is NULL\n");
		exit(EXIT_FAILURE);	
	}
	if((queue->last)!=NULL){
		(queue->last)->next = process;
		queue->last = (queue->last)->next;
	} else {
		queue->process_q = process;
		queue->last = queue->process_q;
	}	
}

node_ptr fcfs_get_process(fcfs_ptr queue){
	if(queue == NULL){
		fprintf(stderr,"Error: Queue is NULL\n");
		exit(EXIT_FAILURE);	
	}
	node_ptr temp = NULL;
	if(queue->process_q != NULL){
		temp = queue->process_q;
		queue->process_q = temp->next;
		if(queue->process_q == NULL){
			queue->last = NULL;
		}	
		temp->next = NULL;
	}

	return temp;
}