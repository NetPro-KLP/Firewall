#ifndef __NODE_H__
#define __NODE_H__

#define SYMBOL_SIZE	256

typedef unsigned char uchar;

//// trie node
typedef struct node
{
	uchar symbol;
	int isEnd;
    
    struct node *failure;

	struct node *child_vector[SYMBOL_SIZE];

} node;

#endif