#include <stdio.h>
#include <stdlib.h>
#include "commonlib.h"
#include "mythread.h"

_MyThread *currentThread_g;
MyQueue *readyQueue_g;
MyQueue *blkQueue_g;
ucontext_t initContext;

/*

References : http://www.cs.ucsb.edu/~rich/class/cs170/notes/Semaphores/

*/

MySemaphore MySemaphoreInit(int initialValue) {
	_MySemaphore *_mySemaphore = (_MySemaphore *)malloc(sizeof(_MySemaphore));
	_mySemaphore->val = initialValue;
	_mySemaphore->blkQueue = (MyQueue *)malloc(sizeof(MyQueue));
	initQueue(_mySemaphore->blkQueue);
	return (MySemaphore)_mySemaphore;
}

// Signal a semaphore
void MySemaphoreSignal(MySemaphore sem) {
	_MySemaphore *_mySemaphore = (_MySemaphore *)sem;
	(_mySemaphore->val)++;
	if (_mySemaphore->val <= 0) {
		_MyThread *_myThread = dequeue(_mySemaphore->blkQueue);
		enqueue(readyQueue_g, _myThread);
	}
}

// Wait on a semaphore
void MySemaphoreWait(MySemaphore sem) {
	_MySemaphore *_mySemaphore = (_MySemaphore *)sem;
	(_mySemaphore->val)--;

	if (_mySemaphore->val < 0) {
		_MyThread *temp = currentThread_g;
		enqueue(_mySemaphore->blkQueue, currentThread_g);
		if (!isEmpty(readyQueue_g)) {
		    currentThread_g = dequeue(readyQueue_g);
		}
		else {
			currentThread_g->ucontext = initContext;
		}
		swapcontext(&(temp->ucontext), &(currentThread_g->ucontext));
	}
}

// Destroy on a semaphore
int MySemaphoreDestroy(MySemaphore sem) {
	_MySemaphore *_mySemaphore = (_MySemaphore *)sem;
	if (_mySemaphore->val == 0) {
		free(_mySemaphore->blkQueue);
		free(_mySemaphore);
		return 0;
	}
	return  -1;
}
