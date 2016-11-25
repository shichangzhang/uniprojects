/* COMP30023 Computer Systems
* Project 1
* 
* reader.c
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: Reads in the data for processes
* 
* Last modified: 9/4/16
*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "reader.h"

process_ptr read_input(char *file_name, int *n){
	FILE *fid = (FILE *)fopen(file_name,"r");
	int num_lines = count_lines(fid);
	int i;
	
	if(num_lines == -1){
		fprintf(stderr,"Failed to open file: %s\n", file_name);
		exit(EXIT_FAILURE);
	}
	
	/* printf("%d lines read\n",num_lines); */
	
	process_ptr input = (process_ptr)malloc(num_lines*sizeof(process_t));
	
	for(i=0;i<num_lines;i++){
		fscanf(fid,"%d%d%d%d",
			&(input[i].time_created),
			&(input[i].process_id),
			&(input[i].memory_size),
			&(input[i].job_time));
	}
	
	fclose(fid);
	
	/* Returns the size of the input array. */
	*n = num_lines; 
	
	return input;
}

int count_lines(FILE *fid){
	if(fid == NULL){
		return -1;
	}
	
	/* dummy variables */
	int a, b, c, d; 
	int lines = 0;
	
	while(fscanf(fid,"%d%d%d%d",&a,&b,&c,&d)==NUM_PROCESS_INFO){
		lines++;
	}
	
	rewind(fid);
	
	return lines;
}
