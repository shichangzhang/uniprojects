/* processGen.c
 *
 * by Shi Chang Zhang, shiz2, 695434
 *
 * Description: This program generates a bunch
 * of process depending on the arguments provided.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_ARGS 8

int main(int argc, char **argv){

	srand(time(NULL));
	int r = rand();

	if(argc != NUM_ARGS){
		fprintf(stderr,"Usage: ./processGen [num_processes] "
			"[max_process_size] [fixed or random] "
			"[max_run_time] [fixed or random] "
			"[max_time_gap] [fixed or random]\n");
		exit(EXIT_FAILURE);
	}

	int i, time=0, size;
	int num_proc = atoi(argv[1]);
	int max_size = atoi(argv[2]);
	int max_time = atoi(argv[4]);
	int max_gap = atoi(argv[6]);	
	int rand_size = 1;
	int rand_time = 1;
	int rand_gap = 1;

	if(strcmp("fixed",argv[3])==0){
		rand_size = 0;
	}
	if(strcmp("fixed",argv[5])==0){
		rand_time = 0;
	}
	if(strcmp("fixed",argv[7])==0){
		rand_gap = 0;
	}

	int proc_size=max_size;
	int proc_time=max_time;;

	for(i=0;i<num_proc;i++){
		if(rand_size){
			proc_size = (rand()%max_size)+1;
		}
		if(rand_time){
			proc_time = (rand()%max_time)+1;
		}
		printf("%d %d %d %d\n",time,i,proc_size,proc_time);
		if(rand_gap){
			time+=(rand()%max_gap);
		} else {
			time+=max_gap;
		}
	}	

	return 0;
}
