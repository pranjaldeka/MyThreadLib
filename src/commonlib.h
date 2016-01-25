#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

typedef struct _MyThread
{
	ucontext_t ucontext;
	struct _MyThread *children;
	struct _MyThread *next;
}_MyThread;

typedef struct MyQueueNode
{
	_MyThread *_myThread;
	struct MyQueueNode *next;
}MyQueueNode;

typedef struct MyQueue
{
	int size;
	MyQueueNode *front, *rear;
}MyQueue;


typedef struct _MySemaphore
{
	int val;
	MyQueue *blkQueue;
}_MySemaphore;

void initQueue(MyQueue *);

void enqueue(MyQueue *, _MyThread *);

_MyThread *dequeue(MyQueue *);

int isEmpty(MyQueue *);

int removeFromQueue(MyQueue *, _MyThread *);

MyQueueNode *createNewQueueNode(_MyThread *);
