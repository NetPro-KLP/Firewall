
#ifndef __BST_H__
#define __BST_H__

#include "klp_protocol.h"

typedef struct bstNode
{
	klp_flow data;

	struct bstNode *left;
	struct bstNode *right;
} bstNode;

int InsertBST(bstNode **root, klp_flow data);
bstNode *SearchBST(bstNode **root, klp_flow data);

void PrintBST(bstNode *root);
int DestroyBST(bstNode *root);

#endif