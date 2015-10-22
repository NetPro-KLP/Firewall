#ifndef __QUEUE_H__
#define __QUEUE_H__

struct node;
typedef struct node node;

typedef node DATA;

typedef struct queueNode
{
	DATA *data;

	struct queueNode *next;
} queueNode_t;

typedef struct queue
{
	queueNode_t *head;
	queueNode_t *tail;

	int size;
} queue_t;

int enqueue(queue_t *queue, DATA *data);
DATA *dequeue(queue_t *queue);

#endif