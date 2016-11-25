/* COMP20003 Algorithms and Data Structures
 * Second (Spring) Semester 2015
 * [Assignment 1]
 * Internet Movie Database
 * as a Binary Search Tree
 * 
 * Stage 2
 * 
 * Created by: Shi Chang Zhang
 * Username: shiz2 SID:695434 
 * 
 * Last modified: 6/9/2015
 *
 * Description:
 * An implementation of a balanced linked-list tree
 * which reads data from a csv file and stores
 * info of duplicate keys in a linked-list.
 */

#include <stdio.h>
#include "bst_ll.h"
#include "readcsv.h"

void *readInputFrom(char *);
void searchTreeAndOutputTo(char *, tree_ptr);

int main(int argc, char *argv[]){
	
	if(argc != 3){
		printf("Usage: ./imdb2 [file.csv] [output.txt] \n");
		exit(EXIT_FAILURE);
	}
	
	tree_ptr *tree_root;
	
	tree_root = (tree_ptr *)readInputFrom(argv[1]);
	searchTreeAndOutputTo(argv[2], *tree_root);
	
	free(tree_root);
	
	return 0;	
}

void *readInputFrom(char *input_file){
	int i=0;
	tree_ptr *tree_root, new_leaf, target;
	FILE *file_ptr;
	
	tree_root = (tree_ptr *)emalloc(sizeof(tree_ptr));
	*tree_root = NULL;
	
	file_ptr = fopen(input_file, "r");
	
	if(file_ptr==NULL){
		printf("Failed to open file: '%s'\n",input_file);
		exit(EXIT_FAILURE);
	}
	
	printf("reading input file...\n");
	
	do{
		createLeaf(&new_leaf);
		readField(&(new_leaf->key), MAX_NAME_SIZE,
						MAX_NAME_SIZE, file_ptr);
		/* Sometimes an EOF comes straight after '\n' or '\r' 
		   that's why feof is called here. */
		if(feof(file_ptr)){
			free(new_leaf);
		}
		else{
			readField(&(new_leaf->info), AVG_MOVIE_SIZE,
						MAX_MOVIE_SIZE, file_ptr);
			insert(tree_root, new_leaf, *tree_root);
			i++;
		}
	}while(!(feof(file_ptr)));
	
	printf("Done reading. (%d lines read)\n", i);
	
	fclose(file_ptr);	
	
	return tree_root;
}

void searchTreeAndOutputTo(char *output_file, tree_ptr tree_root){
	int num_cmp = 0, total_cmp = 0, num_keys = 0;
	char *buffer;
	FILE *file_ptr;
	tree_ptr target;
	
	file_ptr = fopen(output_file,"w");
	
	if(file_ptr==NULL){
		exit(EXIT_FAILURE);
	}
	
	buffer = (char *)emalloc(MAX_NAME_SIZE*sizeof(char));
	
	while(readLine(&buffer)){
		num_keys++;
		num_cmp = searchTree(tree_root, buffer, &target);
		total_cmp += num_cmp;
		if(target == NULL){
			fprintf(file_ptr,"%s --> NOTFOUND\n", buffer);	
		}
		else{
			printList(target, file_ptr);
		}
		printf("%s --> %d\n", buffer, num_cmp);
	}

	if(num_keys!=0){
		printf("Average key comparisons = %f\n",(total_cmp*1.0)/num_keys);
	}
	
	free(buffer);
	
	fclose(file_ptr);
}
