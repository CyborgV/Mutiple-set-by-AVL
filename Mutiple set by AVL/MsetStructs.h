#ifndef MSET_STRUCTS_H
#define MSET_STRUCTS_H

// IMPORTANT: Only structs should be placed in this file.
//            All other code should be placed in Mset.c.

// DO NOT MODIFY THE NAME OF THIS STRUCT
struct mset {
	struct node *tree;  
	int size;
	int totalCount;
};

// DO NOT MODIFY THE NAME OF THIS STRUCT
struct node {
	int elem;           
	int count;          
	struct node *left;  
	struct node *right; 
	int height;
};



