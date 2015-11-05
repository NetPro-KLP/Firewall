//
//  hash.c
//  hash
//
//  Created by jangsoo on 2015. 10. 13..
//  Copyright © 2015년 jangsoo. All rights reserved.
//

#include "hash.h"

#include <linux/slab.h>

unsigned long strhash(const char *str)
{
    unsigned long hvalue = 0;
    int i = 0;
    for(i=0; *(str + i); i++)
    {
        hvalue += (unsigned long)*(str+i);
    }
    
    return hvalue % HASH_SIZE;
    
}

void InitHash(hash *table)
{
    unsigned long i;

    table->count = 0;
    table->item = (list *)kmalloc(sizeof(list)*HASH_SIZE, GFP_KERNEL);
    for(i=0; i<HASH_SIZE; i++)
        InitList(&table->item[i]);
}

int InsertHash(hash *table, klp_flow *data)
{
    unsigned long i = strhash((char *)&data->key);
    if( 0 <= i && i < HASH_SIZE)
    {
        table->count++;
        InsertList (&table->item[i], data);
    }
    
    return 0;
}

listNode *SearchHash(hash *table, klp_flow *data)
{
    unsigned long i = strhash((char *)&data->key);
    
    if( 0 <= i && i < HASH_SIZE)
    {
        return SearchList(&table->item[i], data);
    }
    else
    {
        return 0;
    }
}

int DeleteHash(hash *table, klp_flow *data)
{
    unsigned long i = strhash((char *)&data->key);
    table->count--;
    return DeleteList(&table->item[i], data);
}

void DestroyHash(hash *table)
{
    int i=0;
    for(i=0; i<HASH_SIZE; i++)
    {
        DestroyList(&table->item[i]);
    }
    
    table->count = 0;
    kfree(table->item);
    table->item = 0;
}

void PrintkHash(hash *table)
{
    int i;
    for(i=0; i<HASH_SIZE; i++)
    {
        listNode *cur = table->item[i].head;
        
        while(cur)
        {
            printk("[klp_firewall]%u %u %d\n", cur->data.key.saddr, cur->data.key.daddr, cur->data.packet_count);
            cur = cur->next;
        }
    }
}