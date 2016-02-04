#include<stdlib.h>
#include "mythread.h"
#include "commonlib.h"


_MyThread *currentThread_g;
MyQueue *readyQueue_g;
MyQueue *blkQueue_g;
ucontext_t initContext;
_MyThread *initThread;

// Create a new thread.
MyThread MyThreadCreate(void(*start_funct)(void *), void *args) {
	_MyThread *newThread = (_MyThread *)malloc(sizeof(_MyThread));
	if (newThread == NULL) {
		printf("Memory allocation error for thread\n");
		exit(-1);
	}
	newThread->childQueue = (MyQueue *)malloc(sizeof(MyQueue));
	if (newThread->childQueue == NULL) {
		printf("Memory allocation error for childQueue\n");
		exit(-1);
	}
	initQueue(newThread->childQueue);
	newThread->parent = currentThread_g;
	newThread->waitFor = NULL;
	getcontext(&(newThread->ucontext));
	(newThread->ucontext).uc_link = NULL;
    (newThread->ucontext).uc_stack.ss_sp  = (char *)malloc(sizeof(char)*STACK_SIZE);
    (newThread->ucontext).uc_stack.ss_size = STACK_SIZE;
    makecontext(&(newThread->ucontext), (void(*)())start_funct,1,args);
	enqueue(readyQueue_g, newThread);
	enqueue(currentThread_g->childQueue, newThread);
	return (void *)newThread;
}

// Yield invoking thread
void MyThreadYield(void) {
	if (!isEmpty(readyQueue_g)) {
		_MyThread *runningThread = currentThread_g;
		enqueue(readyQueue_g, runningThread);
		currentThread_g = dequeue(readyQueue_g);
		swapcontext(&(runningThread->ucontext), &(currentThread_g->ucontext));
	}
}

// Join with a child thread
int MyThreadJoin(MyThread thread) {
	_MyThread *child = (_MyThread *)thread;

	if((child == NULL) || child->parent != currentThread_g) {
		return -1;
	}
	else if (isPresentInQueue(currentThread_g->childQueue, child)){
		currentThread_g->waitFor = child;
		enqueue(blkQueue_g, currentThread_g);
		_MyThread *runningThread = currentThread_g;
		if (!isEmpty(readyQueue_g)) {
			currentThread_g = dequeue(readyQueue_g);
			swapcontext(&(runningThread->ucontext), &(currentThread_g->ucontext));
		}
		else {
			setcontext(&initContext);
		}
	}
	return 0;
}

// Join with all children
void MyThreadJoinAll(void) {
	if(!isEmpty(currentThread_g->childQueue)) {
		enqueue(blkQueue_g, currentThread_g);
		_MyThread *runningThread = currentThread_g;
		if (!isEmpty(readyQueue_g)) {
			currentThread_g = dequeue(readyQueue_g);
			swapcontext(&(runningThread->ucontext), &(currentThread_g->ucontext));
		}
		else {
			setcontext(&initContext);
		}
	}
}


void updateParentOfChildren(){
	MyQueue *childQueue = currentThread_g->childQueue;
	MyQueueNode *node = childQueue->front;

	_MyThread *parent = NULL;
    if (currentThread_g != initThread) {
        parent = initThread;
     }
     while(node != NULL) {
         _MyThread *_myThread = node->_myThread;
         if (_myThread != NULL) {
         	_myThread->parent = parent;
         }    
         node = node->next;  
    }
}

void unblockParent() {
	if (currentThread_g->parent != NULL) {
		removeFromQueue(currentThread_g->parent->childQueue, currentThread_g);
		if (isPresentInQueue(blkQueue_g, currentThread_g->parent)) {
			if (currentThread_g->parent->waitFor == currentThread_g || isEmpty(currentThread_g->parent->childQueue)) {
		    	removeFromQueue(blkQueue_g, currentThread_g->parent);
			    currentThread_g->parent->waitFor = NULL;
			    enqueue(readyQueue_g, currentThread_g->parent);
		    }
		}
	}
}

// Terminate invoking thread
void MyThreadExit(void) {
	updateParentOfChildren();
	unblockParent();
	/*TODO*/
	free(currentThread_g->childQueue);
	free(currentThread_g);
	currentThread_g = dequeue(readyQueue_g);
	if (currentThread_g != NULL) {
		setcontext(&(currentThread_g->ucontext));
	}
	else {
		setcontext(&initContext);
	}
}

// Create and run the "main" thread
void MyThreadInit(void(*start_funct)(void *), void *args) {
	readyQueue_g = (MyQueue *)malloc(sizeof(MyQueue));
	if (readyQueue_g == NULL) {
		printf("Memory allocation error for readyQueue_g\n");
		exit(-1);
	}

	initQueue(readyQueue_g);

	blkQueue_g = (MyQueue *)malloc(sizeof(MyQueue));
	if (blkQueue_g == NULL) {
		printf("Memory allocation error for blkQueue_g\n");
		exit(-1);
	}
	initQueue(blkQueue_g);

    currentThread_g = (_MyThread *)malloc(sizeof(_MyThread));
    if (currentThread_g == NULL) {
		printf("Memory allocation error for currentThread_g\n");
		exit(-1);
	}
    currentThread_g->childQueue = (MyQueue *)malloc(sizeof(MyQueue));
    if (currentThread_g->childQueue == NULL) {
		printf("Memory allocation error for currentThread_g->childQueue\n");
		exit(-1);
	}
    initQueue(currentThread_g->childQueue);
    currentThread_g->parent = NULL;
    currentThread_g->waitFor = NULL;

    getcontext(&(currentThread_g->ucontext));

    (currentThread_g->ucontext).uc_link = NULL;
    (currentThread_g->ucontext).uc_stack.ss_sp  = (char *)malloc(sizeof(char)*STACK_SIZE);
    (currentThread_g->ucontext).uc_stack.ss_size = STACK_SIZE;

    initThread = currentThread_g;

    makecontext(&(currentThread_g->ucontext), (void (*)()) start_funct, 1, args);
    swapcontext(&initContext, &(currentThread_g->ucontext));
}
