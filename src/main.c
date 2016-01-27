#include<stdio.h>
#include "commonlib.h"

int main() {
    MyQueue *myQueue = malloc(sizeof(MyQueue));
    initQueue(myQueue);
    printf("Test\n");
    return 0;
}
