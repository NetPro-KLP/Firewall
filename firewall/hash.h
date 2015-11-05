//
//  hash.h
//  hash
//
//  Created by jangsoo on 2015. 10. 13..
//  Copyright © 2015년 jangsoo. All rights reserved.
//

#ifndef __HASH_H__
#define __HASH_H__

#include "list.h"
#include "klp_protocol.h"

#define HASH_SIZE 100

typedef struct hash
{
	int count;
    list *item;
} hash;

unsigned long strhash(const char *str);

void InitHash(hash *table);
int InsertHash(hash *table, klp_flow *data);

listNode *SearchHash(hash *table, klp_flow *data);

int DeleteHash(hash *table, klp_flow *data);
void DestroyHash(hash *table);

void PrintkHash(hash *table);

#endif /* hash_h */
