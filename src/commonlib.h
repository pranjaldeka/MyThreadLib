#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

struct MyQueue;

typedef struct _MyThread
{
	ucontext_t ucontext;
	struct MyQueue *childQueue;
	struct _MyThread *parent;
	struct _MyThread *waitFor;
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

int isPresentInQueue(MyQueue *, _MyThread *);

extern _MyThread *currentThread_g;
extern MyQueue *readyQueue_g;
extern MyQueue *blkQueue_g;
extern ucontext_t initContext;

#define STACK_SIZE 8192
