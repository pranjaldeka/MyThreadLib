objects = mythread.o mysemaphore.o myqueue.o
headers = mythread.h commonlib.h
lib = mythread.a

$(lib): $(objects)
	ar rcs $(lib) $(objects)

mythread.o: mythread.c mythread.h mysemaphore.c myqueue.c commonlib.h 
	gcc -c mythread.c mysemaphore.c myqueue.c 

mysemaphore.o: mysemaphore.c commonlib.h mythread.h
	gcc -c mysemaphore.c -o mysemaphore.o 

myqueue.o: myqueue.c commonlib.h
	gcc -c myqueue.c -o myqueue.o 

clean:
	rm -f $(lib) $(objects)
