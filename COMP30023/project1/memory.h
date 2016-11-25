/* COMP30023 Computer Systems
* Project 1
* 
* memory.h
*  
* by Shi Chang Zhang, shiz2, 695434
* 
* Description: Header file for memory.c
* 
* Last modified: 17/4/16
*/

#define HOLE NULL

typedef struct segment{
	struct segment *next;
	node_ptr process; /* if NULL then it is a HOLE */
	int start; /* start index of memory segment */
	int end; /* end index of memory segment */
	int time_added; /* time added to memory */
	int time_running; /* CPU time */
}seg_t;

typedef seg_t *seg_ptr;

typedef struct memory{
	seg_ptr segments; /* a list of memory segments */
	int num_processes; /* counter for processes */
	int num_holes; /* counter for holes */
	int usage; /* total memory used in MB */
	int size; /* memory size in MB */
}mem_t;

typedef mem_t *mem_list;

mem_list memory_init(int mem_size);
seg_ptr run_process(mem_list memory, node_ptr process, int time);
seg_ptr find_segment(mem_list memory, node_ptr process);
seg_ptr add_to_memory(mem_list memory, node_ptr process, int time);
seg_ptr find_free_memory(mem_list memory, int process_size);
void free_segment(mem_list memory, seg_ptr segment);
void swap_out_process(mem_list memory);
void update_memory(mem_list memory);
void concat_holes(mem_list memory); /* concatenates adjacent holes */
void print_memory(mem_list memory);
int count_holes(mem_list memory); /* counts the number of holes*/
int update_usage(mem_list memory); /* sums up the total memory in use */
int get_usage(mem_list memory);
