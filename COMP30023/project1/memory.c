/* COMP30023 Computer Systems
* Project 1
* 
* memory.c
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: This file contains all the methods that deal with memory.
* 
* Last modified: 17/4/16
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "queue.h"
#include "memory.h"

/* memory_init() initializes and returns mem_list with a segment
* which contains a HOLE */
mem_list memory_init(int mem_size){
	mem_list memory = (mem_list)malloc(sizeof(mem_t));
	seg_ptr segment = (seg_ptr)malloc(sizeof(seg_t));
	
	if(memory == NULL){
		fprintf(stderr,"Error: Failed to allocate memory to memory"
			"in memory_init()\n");
		exit(EXIT_FAILURE);	
	}
	if(segment == NULL){
		fprintf(stderr,"Error: Failed to allocate memory to segment"
			"in memory_init()\n");
		exit(EXIT_FAILURE);	
	}
	
	segment->next = NULL;
	segment->process = HOLE;
	segment->start = 0;
	segment->end = mem_size;
	segment->time_added = 0;
	segment->time_running = 0;
	
	memory->segments = segment;
	memory->num_processes = 0;
	memory->num_holes = 1;
	memory->usage = 0;
	memory->size = mem_size;
	
	return memory;
}

/* This method first checks if the process is memory. If it is return the
* memory segment of the process. If it isn't then add the process to memory, 
* and return the memory segment allocated to the process.
*/
seg_ptr run_process(mem_list memory, node_ptr process, int time){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(process == NULL){
		fprintf(stderr,"Error: process is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	seg_ptr cpu = find_segment(memory, process);
	
	if(cpu == NULL){
		cpu = add_to_memory(memory, process, time);
	}
	
	return cpu;	
}

/* This method looks for the process in memory. */
seg_ptr find_segment(mem_list memory, node_ptr process){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(process == NULL){
		fprintf(stderr,"Error: process is NULL\n");
		exit(EXIT_FAILURE);
	}
	seg_ptr segment = memory->segments;
	while(segment!=NULL){
		if(segment->process != HOLE){
			if((segment->process)->process.process_id 
				== process->process.process_id){
			return segment;	
				}
		}
		segment = segment->next;	
	}
	return NULL;
}

/* This method tries to add a process to memory */
seg_ptr add_to_memory(mem_list memory, node_ptr process, int time){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(process == NULL){
		fprintf(stderr,"Error: process is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(time < 0){
		fprintf(stderr,"Error: time less than 0\n");
		exit(EXIT_FAILURE);
	}
	/* check if there is free memory (i.e. a HOLE) large enough to fit
	the process */
	seg_ptr segment = find_free_memory(memory, 
		process->process.memory_size);
	
	seg_ptr hole = (seg_ptr)malloc(sizeof(seg_t));
	
	if(segment == NULL){
		fprintf(stderr,"Error: Failed to find free memory\n");
		exit(EXIT_FAILURE);
	}
	if(hole == NULL){
		fprintf(stderr,"Error: Failed to allocate memory to hole"
			"in add_to_memory()\n");
		exit(EXIT_FAILURE);
	}
	
	/* make a copy of the HOLE so that it can be split up */
	memcpy(hole, segment, sizeof(seg_t));
	segment->process = (node_ptr)malloc(sizeof(node_t));
	
	if(segment == NULL){
		fprintf(stderr,"Error: Failed to allocate memory to process"
			"in add_to_memory()\n");
		exit(EXIT_FAILURE);
	}
	
	/* make a copy of the process and put it in memory */
	memcpy(segment->process, process, sizeof(node_t));
	if(segment->end > (segment->start + process->process.memory_size)){
		segment->end = segment->start + process->process.memory_size;	
		hole->start = segment->end + 1;
		segment->next = hole;
	} else {
		/* if the HOLE and process have the same size it doesn't need 
		to split, so free up the previously allocated memory */
		free(hole);	
	}
	segment->time_added = time;
	
	memory->num_processes++;
	update_memory(memory);
	
	return segment;
}

/* Searches memory for a HOLE large enough to fit the process */
seg_ptr find_free_memory(mem_list memory, int process_size){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(process_size < 0){
		fprintf(stderr,"Error: process_size is less than 0n");
		exit(EXIT_FAILURE);
	}
	
	seg_ptr temp = memory->segments;
	seg_ptr first_fitting_hole = NULL;
	
	while(temp!=NULL){
		if(temp->process == HOLE){
			if(process_size <= ((temp->end)-(temp->start))){
				first_fitting_hole = temp;
				return first_fitting_hole;
			}
		}
		temp = temp->next;
	}
	
	while(first_fitting_hole == NULL){
		swap_out_process(memory);
		update_memory(memory);
		/* recursively find first fitting hole.
		assuming a process' size is never greater than 
		the memory size, this should always terminate */
		first_fitting_hole = find_free_memory(memory, process_size);
	}
	
	return first_fitting_hole;
}

/* Frees a given memory segment by setting it to a HOLE */
void free_segment(mem_list memory, seg_ptr segment){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	if(segment == NULL){
		fprintf(stderr,"Error: memory segment is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	free(segment->process);
	segment->process = HOLE;
	segment->time_added = 0;
	segment->time_running = 0;
		
	memory->num_processes--;
	update_memory(memory);
}

/* Swaps out the largest process in memory. If two memory
* segments are of equal size, choose the one which has been 
* memory the longest.*/
void swap_out_process(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	seg_ptr temp = memory->segments;
	seg_ptr max_process = NULL; /* process occupying most memory */
	int max = 0; /* max memory occupied by a process */
	
	while(temp!=NULL){
		if((temp->process)!=HOLE){
			if((temp->process)->process.memory_size > max){
				max_process = temp;
				max = (temp->process)->process.memory_size;
			} else if((temp->process)->process.memory_size == max){
				if(temp->time_added < max_process->time_added){
					max_process = temp;
					max = 
					(temp->process)->process.memory_size;	
				}
			}
		}
		temp = temp->next;
	}
	
	if(max_process == NULL){
		fprintf(stderr,"Error: can't swap out processes in memory\n");
		exit(EXIT_FAILURE);	
	}
	
	free_segment(memory, max_process);
}

/* Updates the memory by concatenating adjacent holes and updating
* the total memory used. */
void update_memory(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	concat_holes(memory);
	memory->num_holes = count_holes(memory);
	memory->usage = update_usage(memory);
}

/* Finds adjacent holes and concatenates them */
void concat_holes(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	seg_ptr temp = memory->segments;
	seg_ptr temp2;
	while(temp!=NULL){
		if(temp->next!=NULL){
			if(temp->process==HOLE && (temp->next)->process==HOLE){
				temp2 = (temp->next)->next;
				temp->end = (temp->next)->end;
				free(temp->next);
				temp->next = temp2;
			}
		}
		temp = temp->next;
	}
}

/* Prints the processes occupying memory */
void print_memory(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	seg_ptr temp = memory->segments;
	while(temp!=NULL){
		if(temp->process == HOLE){
			printf("HOLE ");	
		} else {
			printf("pid:%d ",temp->process->process.process_id);
		}
		printf("occupying memory addresses %d - %d\n", 
			temp->start, temp->end);
		temp = temp->next;
	}
}

/* Counts the number of holes in memory */
int count_holes(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	seg_ptr temp = memory->segments;
	int counter = 0;
	
	while(temp!=NULL){
		if(temp->process==HOLE){
			counter++;
		}
		temp = temp->next;
	}
	
	return counter;
}

/* Adds up the sizes of the processes in memory */
int update_usage(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	
	seg_ptr temp = memory->segments;
	int total_usage = 0;
	
	while(temp!=NULL){
		if(temp->process!=HOLE){
			total_usage+=(temp->process)->process.memory_size;
			if(total_usage > memory->size){
				fprintf(stderr,"Error: Memory is full "
					"%d MB used %d MB total\n",
					memory->usage,
					memory->size);
				exit(EXIT_FAILURE);	
			}
		}
		temp = temp->next;
	}
	
	return total_usage;
}

/* Calculates the percentage of memory used */
int get_usage(mem_list memory){
	if(memory == NULL){
		fprintf(stderr,"Error: memory is NULL\n");
		exit(EXIT_FAILURE);
	}
	int percentage;	
	int numerator = memory->usage;
	int denominator = memory->size;
	
	if(numerator == 0){
		return 0;	
	}
	
	percentage = numerator*100;
	if(percentage%denominator > denominator/2){
		return percentage/denominator+1;		
	}
	return percentage/denominator;
}
