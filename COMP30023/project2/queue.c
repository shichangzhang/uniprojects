/* queue.c
 *
 * COMP30023 Computer Systems
 * Project 2
 * Semester 1, 2016
 *
 * by Shi Chang Zhang, 695434, shiz2
 *
 * Last modified: 27/5/2016
 * 
 * Description: Implementation for a queue to store 
 * client tids.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "queue.h"

queue_ptr q_init(){
	queue_ptr q = malloc(sizeof(queue_t));
	if(q==NULL){
		printf("[ERROR] Failed to malloc\n");
		exit(1);
	}
	q->count = 0;
	q->head = NULL;
	q->tail = NULL;
	
	return q;
}

void q_add(queue_ptr q, pthread_t tid, int sockfd){
	if(q==NULL){
		printf("[ERROR] Cannot add to q\n");
		exit(1);
	}
	node_ptr temp = q->tail;
	if(temp == NULL){
		q->tail = malloc(sizeof(node_t));
		if(q->tail == NULL){
			printf("[ERROR] Failed to malloc\n");
			exit(1);	
		}
		memcpy((&(q->tail)->tid),&tid,sizeof(tid));
		(q->tail)->sockfd = sockfd;
		(q->tail)->next = NULL;
		q->head = q->tail;
	} else {
		q->tail = malloc(sizeof(node_t));
		if(q->tail == NULL){
			printf("[ERROR] Failed to malloc\n");
			exit(1);	
		}
		memcpy(&((q->tail)->tid),&tid,sizeof(tid));
		(q->tail)->sockfd = sockfd;	
		(q->tail)->next = NULL;
		temp->next = q->tail;
	}	
}

void q_delete(queue_ptr q, pthread_t tid){	
	if(q==NULL){
		printf("[ERROR] Cannot add to q\n");
		exit(1);
	}
	node_ptr temp = q->head;
	node_ptr temp2;
	while(temp!=NULL){
		if(temp->next != NULL && pthread_equal((temp->next)->tid,tid)){
			temp2 = temp->next;
			temp->next = temp2->next;
			if(q->tail == temp2){
				q->tail = temp;
			}
			free(temp2);	
		} else if(pthread_equal(temp->tid,tid)){
			q->head = temp->next;
			if(q->tail == temp){
				q->tail = NULL;
			}
			free(temp);
		}
		temp = temp->next;
	}	
}
