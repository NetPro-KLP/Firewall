#include "queue.h"

#include <linux/slab.h>
int enqueue(queue_t *queue, DATA *data)
{
	queueNode_t *_new = (queueNode_t*)kmalloc(sizeof(queueNode_t), GFP_KERNEL);

	if(_new == 0)
		return -1;

	_new->data = data;
	_new->next = 0;

	if(queue->head && queue->tail)
	{
		queue->tail->next = _new;
		queue->tail = _new;
	}
	else
	{
		queue->head = _new;
		queue->tail = _new;
	}
	queue->size++;

	return 0;
}
DATA *dequeue(queue_t *queue)
{
	queueNode_t *pop = queue->head;
	DATA *data = pop->data;

	queue->head = queue->head->next;

	if(queue->head == 0)
		queue->tail = 0;

	kfree(pop);
	queue->size--;
	return data;
}