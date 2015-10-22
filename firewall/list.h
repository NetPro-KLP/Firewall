//
//  list.h
//  hash
//
//  Created by jangsoo on 2015. 10. 13..
//  Copyright © 2015년 jangsoo. All rights reserved.
//

#ifndef __LIST_H__
#define __LIST_H__

#include "klp_protocol.h"

typedef struct listNode
{
    klp_flow data;
    struct listNode *next;
    
} listNode;
typedef struct list
{
    int c;
    listNode *head;
} list;

void InitList(list *head);
int InsertList(list *head, klp_flow *data);

listNode *SearchList(list *head, klp_flow *data);

int DeleteList(list *head, klp_flow *data);
void DestroyList(list *head);

#endif /* list_h */
