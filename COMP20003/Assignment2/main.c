/* COMP20003 Algorithms and Data Structures
* Semester 2, 2015
* Assignment 2
*
* Solving Puzzle Games Optimally: Heuristic Search
* 
* main.c
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TILES 16
#define HEIGHT 4
#define WIDTH 4
#define MAX_ACTIONS 4
#define UP 0
#define DOWN 3
#define LEFT 1
#define RIGHT 2

typedef struct node{
	struct node *parent;
	int previous;
	int state[NUM_TILES];
	int blank_pos;
	int path_cost;
	int evaluation;
	int actions[MAX_ACTIONS];
} node_t;

typedef node_t* node_ptr;

void apply_action(int action, int state[], node_ptr next);
void copy_state(int state1[], int state2[]);
void swap(int *a, int *b);
int h(int state[]);
int dist(int tile, int tile_expected);
int find_blank(int state[]);
void possible(int blank_pos, int A[], int previous);
node_ptr IDA_control_loop();
node_ptr IDA_star(node_ptr n, int *B, int *B1);
//int *A(s);
int min(int a, int b);
void print_state(int state[]);
void print_solution(node_ptr solution);
void print_solution2(node_ptr solution);

int generated = 0;
int expanded = 0;
char *action_name[] = {"UP", "LEFT", "RIGHT", "DOWN"};

int main(int argc, char **argv){
	node_ptr n;	
	int state0[] = { 14, 13, 15, 7, 11, 12, 9, 5, 6, 0, 2, 1, 4, 8, 10, 3};
	int state1[] = { 1, 2, 3, 7, 4, 5, 6, 0, 8, 9, 10, 11, 12, 13, 14, 15};
	int state2[] = {13, 5, 4 ,10 ,9 ,12 ,8 ,14, 2 ,3 ,7 ,1 ,0, 15 ,11 ,6 };
	int state88[] = {15, 2, 12, 11, 14, 13, 9, 5, 1, 3, 8, 7, 0, 10, 6, 4};
	printf("\n\nh(s0) = %d\n", h(state0));
	printf("\n\nh(s0) = %d\n", h(state1));
	printf("\n\nh(s0) = %d\n", h(state2));

	clock_t start = clock(), diff;
	n = IDA_control_loop(state88);
	diff = clock() - start;
	
	float time = (diff*1.0)/CLOCKS_PER_SEC;
	printf("time: %.3f\n", time);
	printf("generated: %d\n", generated);
	printf("expanded: %d\n", expanded);
	printf("solution: %d", n->path_cost);
	
	print_solution(n);
	print_solution2(n);
	return 0;
}

node_ptr IDA_control_loop(int state0[]){
	int B = h(state0);
	print_state(state0);
	int B1;
	node_t n;
	node_ptr r = NULL;
	while (r==NULL){
		B1 = INT_MAX;
		copy_state(n.state, state0);
		n.path_cost = 0;
		n.blank_pos = find_blank(state0);
		n.previous = -1;
		r = IDA_star(&n, &B, &B1);
		if(r==NULL){
			B = B1;	
		}
	}
	return r;
}

node_ptr IDA_star(node_ptr n, int *B, int *B1){
	int i;
	int a;
	node_ptr r;
	node_ptr solution;
	node_t next;

	possible(n->blank_pos, n->actions, n->previous);
	
	for(i=0;i<WIDTH;i++){
		if(n->actions[i]){
			generated++;
			next.blank_pos = n->blank_pos;
			apply_action(i, n->state, &next);
			//print_state(next.state);
			next.blank_pos = find_blank(next.state);
			next.path_cost = n->path_cost + 1;
			next.evaluation = next.path_cost + 1.5*h(next.state);
			next.previous = i;

			if(next.evaluation > *B){
				*B1 = min(next.evaluation, *B1);	
			}
			else{
				if(h(next.state) == 0){
					solution = (node_ptr)malloc(sizeof(node_t));
					*solution = next;
					solution->parent = (node_ptr)malloc(sizeof(node_t));
					*(solution->parent) = *n;
					(solution->parent)->parent = NULL;
					return solution;
				}
				r = IDA_star(&next, B, B1);
				expanded++;
				if(r!=NULL){
					node_ptr temp = r;
					while(temp->parent != NULL){
						temp = temp->parent;
					}
					temp->parent = (node_ptr)malloc(sizeof(node_t));
					*(temp->parent) = *n;
					(temp->parent)->parent = NULL;
					return r;	
				}
			}
		}
	}
	return NULL;
}

int h(int state[]){
	int i;
	int sum = 0;
	for(i=0;i<NUM_TILES;i++){
		if(state[i] != 0){
			sum += dist(i, state[i]);
		}
	}
	return sum;
}

int dist(int tile, int tile_expected){
	int x = tile % WIDTH, x_expected = tile_expected % WIDTH;
	int y = tile / HEIGHT, y_expected = tile_expected / HEIGHT;
	return abs(x - x_expected) + abs(y - y_expected);	 
}

void apply_action(int action, int state[], node_ptr next){
	copy_state(next->state, state);
	if(action == UP){
		swap(&(next->state[next->blank_pos]), &(next->state[next->blank_pos-4]));
	}
	else if(action == DOWN){
		swap(&(next->state[next->blank_pos]), &(next->state[next->blank_pos+4]));
	}
	else if(action == LEFT){
		swap(&(next->state[next->blank_pos]), &(next->state[next->blank_pos-1]));
	}
	else if(action == RIGHT){
		swap(&(next->state[next->blank_pos]), &(next->state[next->blank_pos+1]));
	}
}

int min(int a, int b){
	if(a>=b){
		return b;
	}
	return a;
}

int find_blank(int state[]){
	int i;
	for(i=0;i<NUM_TILES;i++){
		if(state[i] == 0){
			return i;
		}
	}
	return -1;
}

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void copy_state(int state1[], int state2[]){
	int i;
	for(i=0;i<NUM_TILES;i++){
		state1[i] = state2[i];
	}
}

void possible(int blank_pos, int A[], int previous){
	if(blank_pos % 4 == 0){
		A[LEFT] = 0;
	}
	else {
		A[LEFT] = 1;
	}
	if(blank_pos % 4 == 3){
		A[RIGHT] = 0;
	}
	else {
		A[RIGHT] = 1;
	}
	if(blank_pos / 4 == 0){
		A[UP] = 0;
	}
	else {
		A[UP] = 1;
	}
	if(blank_pos / 4 == 3){
		A[DOWN] = 0;
	}
	else {
		A[DOWN] = 1;
	}
	if(previous >= 0){
		A[3-previous] = 0;
	}
}

void print_state(int state[]){
	int i;
	printf("\n");
	for(i=0;i<NUM_TILES;i++){
		if(i % 4 == 0){
			printf("\n|----|----|----|----|\n|");
		}
		printf(" %2d |", state[i]);
	}
	printf("\n|----|----|----|----|\n");
}

void print_moves(int move){
	printf("%s ", action_name[move]);	
}

void print_solution(node_ptr solution){
	if(solution == NULL){
		return;	
	}
	print_solution(solution->parent);
	print_state(solution->state);
	
}

void print_solution2(node_ptr solution){
	if(solution->previous == -1){
		return;	
	}
	print_solution2(solution->parent);
	print_moves(solution->previous);
	
}