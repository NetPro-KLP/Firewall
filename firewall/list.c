//
//  list.c
//  hash
//
//  Created by jangsoo on 2015. 10. 13..
//  Copyright © 2015년 jangsoo. All rights reserved.
//

#include "list.h"

#include <linux/slab.h>
#include <linux/string.h>

void InitList(list *head)
{
    head->head = 0;
    head->c = 0;
}

int InsertList(list *head, klp_flow *data)
{
    listNode *_new = (listNode *)kmalloc(sizeof(listNode), GFP_KERNEL);
    
    if(_new == 0)
        return -1;
    
    _new->data = *data;
    _new->next = head->head;
    
    head->head = _new;
    
    return 0;
}

listNode *SearchList(list *head, klp_flow *data)
{
    listNode *cur = head->head;
    
    while(cur && strcmp((char*)&cur->data.key, (char*)&data->key))
    {
        cur = cur->next;
    }
    return cur;
}

int DeleteList(list *head, klp_flow *data)
{
    listNode *del = head->head;
    listNode *pre = 0;
    
    while(del && strcmp((char*)&del->data.key, (char*)&data->key))
    {
        pre = del;
        del = del->next;
    }
    
    if(del == 0)
        return -1;
    
    if(del == head->head)
        head->head = head->head->next;
    else if(del->next)
        pre->next = 0;
    else
        pre->next = del->next;
    
    kfree(del);
    
    return 0;
}

void DestroyList(list *head)
{
    listNode *del;
    
    while(head->head)
    {
        del = head->head;
        head->head = head->head->next;
        kfree(del);
    }
}