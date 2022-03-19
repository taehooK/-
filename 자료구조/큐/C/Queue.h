//Queue.h

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stddef.h>

typedef signed long int Long;
typedef enum boolean{TRUE=1, FALSE=0}Boolean;
typedef struct _queueNode QueueNode; // 전방선언
typedef struct _queueNode {
	QueueNode *next;
}QueueNode;
typedef struct _queue {
	QueueNode *front;
	QueueNode *rear;
	Long length;
}Queue;

void Queue_Create(Queue *queue);
QueueNode *Put(Queue *queue, void *object, size_t size);
QueueNode *Get(Queue *queue);
void Front(Queue *queue, void *object, size_t size);
void Rear(Queue *queue, void *object, size_t size);
void Queue_Empty(Queue *queue);
Boolean Queue_IsEmpty(Queue *queue);
void Queue_Destroy(Queue *queue);
#endif // _QUEUE_H


