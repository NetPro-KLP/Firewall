#include "bst.h"

#include <linux/slab.h>

#include "kstring.h"

int InsertBST(bstNode **root, klp_flow data)
{
	// data key value and current key value
	klp_key *dkey = &data.key;
	klp_key *ckey = 0;

	// node will be inserted
	bstNode *_new = 0;

	// current point and parent point
	bstNode *cur = 0;
	bstNode *pre = 0;

	// compare factor
	int cmp = 0;

	// search location of node
	cur = *root;
	while(cur)
	{
		ckey = &(cur->data.key);
		cmp = kstrcmp( (char*)ckey, (char*)dkey);

		pre = cur;
		if( cmp > 0)
			cur = cur->left;
		else if( cmp < 0)
			cur = cur->right;
		else 
			return -1;
	}

	// set data
	_new = (bstNode*)kmalloc(sizeof(bstNode), GFP_KERNEL);
	_new->data = data;
	_new->left = 0;
	_new->right = 0;

	ckey = &(pre->data.key);

	// insert node
	if(*root == 0x00)
		*root = cur;
	else if( kstrcmp( (char*)ckey, (char*)dkey) > 0)
		pre->right = _new;
	else
		pre->left = _new;

	return 0;
}

bstNode *SearchBST(bstNode **root, klp_flow data)
{
	klp_key *dkey = &(data.key);
	klp_key *ckey = 0;
	int cmp = 0;

	bstNode *cur = *root;

	while(cur)
	{
		ckey = &(cur->data.key);
		cmp = kstrcmp( (char*)ckey, (char*)dkey);
		if(cmp > 0)
			cur = cur->right;
		else if(cmp < 0)
			cur = cur->left;
		else
			return cur;
	}
	return 0x00;
}

void PrintBST(bstNode *root)
{
	if(root)
    {
        /*Traverse down the left subtree*/
        if(root->left)
            PrintBST(root->left);
        /*Traverse down the right subtree*/
        if(root->right)
            PrintBST(root->right);
        
        printk("print source : %u \t %u dest : %u\n \n", root->data.key.src, root->data.key.dest );
    }
}
int DestroyBST(bstNode *root)
{
	if(root)
    {
        /*Traverse down the left subtree*/
        if(root->left)
            DestroyBST(root->left);
        /*Traverse down the right subtree*/
        if(root->right)
            DestroyBST(root->right);
        /*Free the memory*/
        kfree(root);
    }
    return 0;
}