#include<stdlib.h>
#include "mythread.h"
#include "commonlib.h"


_MyThread *currentThread_g;
MyQueue *readyQueue_g;
MyQueue *blkQueue_g;
ucontext_t initContext;

// Create a new thread.
MyThread MyThreadCreate(void(*start_funct)(void *), void *args) {

}

// Yield invoking thread
void MyThreadYield(void) {

}

// Join with a child thread
int MyThreadJoin(MyThread thread) {

}

// Join with all children
void MyThreadJoinAll(void) {

}

// Terminate invoking thread
void MyThreadExit(void) {

}

// Create and run the "main" thread
void MyThreadInit(void(*start_funct)(void *), void *args) {

}