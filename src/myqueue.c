#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include "commonlib.h"

/*
Queue implementation referenced http://geeksquiz.com/queue-set-2-linked-list-implementation/
*/

void initQueue(MyQueue *myQueue) {
	myQueue->size = 0;
	myQueue->front = NULL;
	myQueue->rear = NULL;
}

void enqueue(MyQueue *myQueue, _MyThread* _myThread) {
	MyQueueNode *newnode = createNewQueueNode(_myThread);

    myQueue->size++;

	if (myQueue->rear == NULL) {
		myQueue->front = myQueue->rear = newnode;
		return;
	}

	myQueue->rear->next = newnode;
	myQueue->rear = newnode;
}

_MyThread *dequeue(MyQueue *myQueue) {
	if (myQueue->front == NULL) {
		return NULL;
	}

    myQueue->size--;

	_MyThread *temp = myQueue->front->_myThread;
	myQueue->front = myQueue->front->next;

	if (myQueue->front == NULL) {
		myQueue->rear = NULL;
	}

	return temp;
}

int isEmpty(MyQueue *myQueue) {
	return myQueue->size == 0;
}

int removeFromQueue(MyQueue *myQueue, _MyThread *_myThread) {
	if (!isEmpty(myQueue)) {
		MyQueueNode *node1 = myQueue->front;
		MyQueueNode *node2 = myQueue->front;

		while (node1 != NULL) {
			if (node1->_myThread == _myThread) {
				if (myQueue->size == 1) {
					myQueue->front = myQueue->rear = NULL;
				}
				else if (node1 == myQueue->rear) {
					myQueue->rear = node2;
					myQueue->rear->next = NULL;
				}
				else if (node1 == myQueue->front) {
					myQueue->front = myQueue->front->next;
				}
				else {
					node2->next = node1->next;
				}
				//free(node1);
				myQueue->size--;
				return 1;
			}
			node2 = node1;
			node1 = node1->next;
		}
	}

	return 0;
}

int isPresentInQueue(MyQueue *myQueue, _MyThread *_myThread) {
	if (!isEmpty(myQueue)) {
		MyQueueNode *node1 = myQueue->front;
		while (node1 != NULL) {
			if (node1->_myThread == _myThread) {
				return 1;
			}
			node1 = node1->next;
		}
	}
	return 0;
}

MyQueueNode *createNewQueueNode(_MyThread* _myThread) {
	MyQueueNode *myQueueNode = (MyQueueNode *)malloc(sizeof(MyQueueNode));
	myQueueNode->_myThread = _myThread;
	myQueueNode->next = NULL;
	return myQueueNode;
}
