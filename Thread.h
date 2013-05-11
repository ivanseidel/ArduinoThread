/*
 	Thread.h - An runnable object

	Contains information about the running Thread, such as:
		Delay between calls,
		If it should be runned,

	ThreadController is an extended class of Thread, since you can include

	Created by Ivan Seidel Gomes, March, 2013.
	Released into the public domain.
*/

#ifndef Thread_h
#define Thread_h

#include <Arduino.h>
#include <inttypes.h>
#define MAX_THREADS		15;

class Thread{
protected:
	long 	interval,			// Desired interval between runs
			last_run,			// Last runned time in Ms
			_cached_next_run;	// Scheduled run in Ms (MUST BE CACHED)

	void runned(long time=-1);	// IMPORTANT! Run after all calls to run()
								// Updates last_run and cache next run.
								// NOTE: This MUST be called if extending
								// this class and implementing run() method

	void (*_onRun)(void);		// Callback for run() if not implemented

public:
	bool 	enabled;			// If the current Thread is enabled or not

	int ThreadID;				// ID of the Thread (initialized from memory adr.)
	String ThreadName;			// Thread Name (used for better UI).

	Thread(void (*callback)(void) = NULL, long _interval = 0);

	// Set the desired interval for calls, and update _cached_next_run
	Thread setInterval(long _interval);

	// Return if the Thread should be runned or not
	virtual bool shouldRun(long time = -1);

	// Callback set
	void onRun(void (*callback)(void));

	// Runs Thread
	virtual void run();
};

#endif