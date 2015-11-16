#ifndef __TRIE_H__
#define __TRIE_H__

#include "node.h"


//// trie function
node *CreateNode(char symbol, int isEnd);

void CraeteTrie(node** root);
int InsertTrie(node **root, char *key);

node *SearchTrie(node **root, const char *key);
void DestroyTrie(node *root);


//// aho corasick algorithm

void MakeFailure(node** root);

int PatternMatching(node **root, char *string);
#endif