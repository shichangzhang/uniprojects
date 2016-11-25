/* COMP30023 Computer Systems
* Project 1
* 
* main.c
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: 
* 
* Last modified: 14/4/16
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"
#include "reader.h"
#include "queue.h"
#include "memory.h"

#define NUM_ARGS 7
#define FCFS 1
#define MULTI 0

extern  int     optind;
extern  char    *optarg;

int main(int argc, char **argv){
	
	if(argc != NUM_ARGS){
		printf("Usage: ./simulation "
			"-f [input.txt] "
			"-a [fcfs or multi] "
			"-m [memsize in MB]\n");
		exit(EXIT_FAILURE);
	}
	
	char input;
	char *input_file = NULL;
	int algorithm;
	int mem_size = 0;
	int num_processes = 0;
	int i;
	int time;
	int total_wait_time = 0;
	int process_count;
	int quantum;
	int quantums[NUM_LEVELS] = {Q1,Q2,Q3}; 
	process_ptr processes;
	mem_list memory;
	seg_ptr cpu;
	node_ptr temp;
	
	/* Parse arguments */
	while ((input = getopt(argc,argv,"f:a:m:")) != EOF){
		switch (input){
		case 'f':
			input_file = optarg;
			if(input_file == NULL){
				fprintf(stderr, "Error: No input file\n");
				exit(EXIT_FAILURE);
			}
			break;
			
		case 'a':
			if(strcmp("fcfs",optarg)==0){
				algorithm = FCFS;
			} else if(strcmp("multi",optarg)==0){
				algorithm = MULTI;
			} else {
				fprintf(stderr,"Error: Algorithm should be " 
					"either 'fcfs' or 'multi'\n");
				exit(EXIT_FAILURE);
			}
			break;
			
		case 'm': 
			mem_size = atoi(optarg);
			if(mem_size <= 0){
				fprintf(stderr,"Error: Zero or "
					"negative memory\n");
				exit(EXIT_FAILURE);	
			}
			break;
			
		default:
			/* Do nothing. */
			break;
		} 
	}
	
	processes = (process_ptr)read_input(input_file,&num_processes);
	memory = memory_init(mem_size);
	time = 0;
	process_count = 0;
	cpu = NULL;
	fcfs_ptr queue;
	multi_ptr queues;
	
	if(algorithm == FCFS){
		queue = (fcfs_ptr)fcfs_init_queue();
	} else if(algorithm == MULTI){
		queues = (multi_ptr)multi_init_queue();
	}	
	while(process_count<num_processes || cpu!=NULL){
		for(i=0;i<num_processes;i++){
			if(processes[i].time_created == time){
				if(algorithm == FCFS){
					fcfs_add_process(queue, processes[i]);
				} else if(algorithm == MULTI){
					fcfs_add_process(queues->queues[LEVEL1], processes[i]);
				}	
			}
		}
		if(cpu == NULL){
			if(algorithm == FCFS){
				temp = fcfs_get_process(queue);
			} else if(algorithm == MULTI){
				temp = multi_get_process(queues);
			}
			if(temp!=NULL){
				cpu = run_process(memory, temp, time);
				if(algorithm == FCFS){
					quantum = temp->remaining_job_time;
				} else if(algorithm == MULTI){
					quantum = quantums[temp->level];
				}
				printf("time %d, %d running, "
					"numprocesses=%d, "
					"numholes=%d, "
					"memusage=%d%%\n",
					time,
					temp->process.process_id,
					memory->num_processes,
					memory->num_holes, 
					get_usage(memory));
				//print_memory(memory);
			}
		}
		time++;
		if(temp!=NULL){
			temp->remaining_job_time--;
			quantum--;
			if(temp->remaining_job_time == 0){
				total_wait_time+=(time-(temp->process.time_created)-(temp->process.job_time));
				free(temp);
				free_segment(memory, cpu);
				cpu = NULL;
				process_count++;
			} else if(quantum == 0){
				if(algorithm == FCFS){
					fcfs_enqueue(queue, temp);
				} else if(algorithm == MULTI){
					multi_enqueue(queues, temp);
				}
				cpu = NULL;
			}
		}	
	}
	
	printf("time %d, simulation finished.\n", time);
	/* Prints out the average waiting time of the processes. */
	printf("average waiting time: %.2f\n",total_wait_time*1.0/num_processes);
	free(processes);
	return 0;
}
