/* bst.h
 *
 * Created by: Shi Chang Zhang
 * Username: shiz2 SID:695434 
 * 
 * Last modified: 6/9/2015
 *
 * Description:
 * A header file containing the struct/type 
 * definitions for bst.c and main.c.
 */

#include <stdlib.h>

typedef struct tree{
	struct tree* parent;
	struct tree* left;
	struct tree* right;
	
	char *key;
	char *info;
	
	int height;  /* height of a tree, with a leaf having 0 height */
	int counter; /* difference of height of sub-trees */
}tree_t;

typedef tree_t* tree_ptr;