/* bst.c
 *
 * Created by: Shi Chang Zhang
 * Username: shiz2 SID:695434
 *
 * Last modified: 6/9/2015
 *
 * Description:
 * Functions for a balanced Binary Search Tree.
 */
 
#include <stdio.h>
#include "bst.h"

int height(tree_ptr tree){
	/* Returns the height of the tree with NULL returning -1. 
	   It is used to help balance the binary tree. */
	if(tree != NULL){
		return tree->height;
	}
	return -1;
}

int heightDiff(tree_ptr tree){
	/* A function that calculates the heigh difference
	   between subtrees.
	   It is used to help balance the binary tree. */
	if(tree == NULL){
		return 0;
	}
	int right_subtree_height = height(tree->right);
	int left_subtree_height = height(tree->left);
		
	return left_subtree_height - right_subtree_height;
}

void updateHeight(tree_ptr tree){
	/* After balancing part of the tree with rotation
	   the heights of some parent nodes/trees need to be
	   updated. This function updates them. */
	if(tree!=NULL){
		if(((tree->counter)=heightDiff(tree))>=1){
			tree->height = height(tree->left)+1;
		}
		else{
			tree->height = height(tree->right)+1;
		}
		updateHeight(tree->parent);	
	}
}

void rotateR(tree_ptr tree){
	/* Rotates the node/tree provided in the 
	   argument to the right. This function
	   assumes that the input tree is not NULL
	   and tree->left is not NULL. 
	   
	   If there is no left sub-tree rotating right 
	   would not be possible. */
	tree_ptr left = tree->left;
	tree_ptr leftRight = left->right;
	tree_ptr parent = tree->parent;
	
	left->parent = parent;
	left->right = tree;
	tree->parent = left;
	tree->left = leftRight;
	if(leftRight!=NULL){
		leftRight->parent = tree;
	}
	if(parent!=NULL){
		if(parent->left == tree){
			parent->left = left;	
		}
		if(parent->right == tree){
			parent->right = left;	
		}
	}	
	updateHeight(tree);
}

void rotateL(tree_ptr tree){
	/* Similar to function rotateR but instead 
	   of rotate right, it rotates left. */
	tree_ptr right = tree->right;
	tree_ptr rightLeft = right->left;
	tree_ptr parent = tree->parent;
	
	right->parent = parent;
	right->left = tree;
	tree->parent = right;
	tree->right = rightLeft;
	if(rightLeft!=NULL){
		rightLeft->parent = tree;
	}
	if(parent!=NULL){
		if(parent->left == tree){
			parent->left = right;	
		}
		if(parent->right == tree){
			parent->right = right;	
		}
	}
	updateHeight(tree);
}

void rotateRL(tree_ptr tree){
	/* Rotate right subtree right,
	   then the tree itself left. */
	rotateR(tree->right);
	rotateL(tree);
}

void rotateLR(tree_ptr tree){
	/* Rotate right subtree right,
	   then the tree itself left. */
	rotateL(tree->left);
	rotateR(tree);
}

void balanceTree(tree_ptr tree, tree_ptr *root){
	/* Implements AVL trees. If counter is greater than
	   1 or less than -1, balance the tree with a rotation. 
	   Use the tree's counter and its subtree's counter to
	   determine what rotation needs to be done. */
	if(abs(tree->counter)>1){
		if(tree->counter > 0){
			if((tree->left)->counter > 0){
				rotateR(tree);
			}
			else{
				rotateLR(tree);
			}
		}
		else{
			if((tree->right)->counter < 0){
				rotateL(tree);
			}
			else{
				rotateRL(tree);
			}
		}
		*root = tree->parent;
		/* a rotation has been done, so we need to
		   change the root of the tree to the new root */
	}
}
void createLeaf(tree_ptr *new_leaf){
	/* Allocates memory and initializes the leaf */
	*new_leaf = (tree_ptr)malloc(sizeof(tree_t));
	
	(*new_leaf)->left = NULL;
	(*new_leaf)->right = NULL;
	(*new_leaf)->parent = NULL;	
	(*new_leaf)->height = 0;
	(*new_leaf)->counter = 0;
}

void insert(tree_ptr *tree, tree_ptr new_leaf, tree_ptr parent){
	/* Inserts a new leaf into the tree and balances
	   the tree if necessary. */
	if ( *tree == NULL ){
		*tree = new_leaf;
		(*tree)->parent = parent;
		
		updateHeight(*tree);
	}
	else if ( strcmp((*tree)->key, new_leaf->key) > 0 ) {
		insert( &((*tree)->left), new_leaf, *tree);
	}
	else {
		insert( &((*tree)->right), new_leaf, *tree);
	}
	balanceTree(*tree, tree);
}

int searchTree(tree_ptr tree, char* key, int *is_found, FILE *file_ptr){
	/* Searchs the tree/dict for a certain key and its duplicates, 
	   then returns the number of key comparisons made. */
	int check;
	if(tree == NULL){
		return 0;
		/* comparisons with NULL are not counted */
	}
	check = strcmp(tree->key, key);
	if(check == 0){
		*is_found = 1;
		fprintf(file_ptr, "%s ",tree->info);
		return 1+searchTree(tree->left, key, is_found, file_ptr)
			+searchTree(tree->right, key, is_found, file_ptr);
		/* Since I have chosen to implement an AVL tree, duplicates
		   won't be guaranteed to be on a specific side, so both need
		   to be checked.*/
	}
	else if(check > 0){
		return 1+searchTree(tree->left, key, is_found, file_ptr);
	}
	else{
		return 1+searchTree(tree->right, key, is_found, file_ptr);	
	}
}