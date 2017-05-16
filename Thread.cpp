#include "Thread.h"

Thread::Thread(void (*callback)(void), unsigned long _interval){
	enabled = true;
	onRun(callback);
	_cached_next_run = 0;
	last_run = millis();

	ThreadID = (int)this;
	#ifdef USE_THREAD_NAMES
		ThreadName = "Thread ";
		ThreadName = ThreadName + ThreadID;
	#endif

	setInterval(_interval);
};

void Thread::runned(unsigned long time){
	// Saves last_run
	last_run = time;

	// Cache next run
	_cached_next_run = last_run + interval;
}

void Thread::setInterval(unsigned long _interval){
	// Save interval
	interval = _interval;

	// Cache the next run based on the last_run
	_cached_next_run = last_run + interval;
}

bool Thread::shouldRun(unsigned long time){
	// If the "sign" bit is set the signed difference would be negative
	bool time_remaining = (time - _cached_next_run) & 0x80000000;

	// Exceeded the time limit, AND is enabled? Then should run...
	return !time_remaining && enabled;
}

void Thread::onRun(void (*callback)(void)){
	_onRun = callback;
}

void Thread::run(){
	if(_onRun != NULL)
		_onRun();

	// Update last_run and _cached_next_run
	runned();
}
