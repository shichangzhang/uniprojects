/* bst_ll.h
 *
 * Created by: Shi Chang Zhang
 * Username: shiz2 SID:695434 
 * 
 * Last modified: 6/9/2015
 *
 * Description:
 * A header file containing struct/type 
 * definitions for bst_ll.c and main2.c.
 */

#include <stdlib.h>
#include <string.h>

typedef struct node{
	/* Used for storing information on duplicate keys */
	char *info;
	
	struct node *more_info;
}node_t;

typedef struct tree{
	struct tree* parent;
	struct tree* left;
	struct tree* right;
	
	struct node* more_info;
	
	char *key;
	char *info;
	
	int height;  /* height of a tree, with a leaf having 0 height */
	int counter; /* difference of height of sub-trees */
}tree_t;



typedef tree_t* tree_ptr;

typedef node_t* node_ptr;
