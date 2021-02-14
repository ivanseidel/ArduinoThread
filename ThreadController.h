/*
 	ThreadController.h - Controlls a list of Threads with different timings

	Basicaly, what it does is to keep track of current Threads and run when
	necessary.

	ThreadController is an extended class of Thread, because of that,
	it allows you to add a ThreadController inside another ThreadController...

	For instructions, go to https://github.com/ivanseidel/ArduinoThread

	Created by Ivan Seidel Gomes, March, 2013.
	Released into the public domain.
*/

#ifndef ThreadController_h
#define ThreadController_h

#include "Thread.h"
#include "inttypes.h"

#define MAX_THREADS		32

class ThreadController: public Thread{
protected:
	Thread* thread[MAX_THREADS];
	int cached_size;
public:
	ThreadController(unsigned long _interval = 0);

	// run() Method is overrided
	void run();

	// Try to run our threads, return how long we can sleep before next needed.  
	long runOrDelay();

	// Adds a thread in the first available slot (remove first)
	// Returns if the Thread could be added or not
	bool add(Thread* _thread);

	// remove the thread (given the Thread* or ThreadID)
	void remove(size_t _id);
	void remove(Thread* _thread);

	// Removes all threads
	void clear();

	// Return the quantity of Threads
	int size(bool cached = true);

	// Return the I Thread on the array
	// Returns NULL if none found
	Thread* get(int index);

	// For debugging it is useful to know the next child thread we want to execute
	Thread* nextThread = NULL;
};

#endif
