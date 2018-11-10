/*
  ThreadLite.h - An runnable object

  Thread is responsable for holding the "action" for something,
  also, it responds if it "should" or "should not" run, based on
  the current time;

  ThreadLite is based on the class Thread, but takes less memory
  and maximum period is only 32,767 seconds.
  WARNING! It is not recommended to use if you do not have a memory deficit.

  Copyright © 2013 Ivan Seidel Gomes.
  Copyright © 2018 Stanislav Hnatiuk.
  Released into the public domain.
*/

#ifndef THREADLITE_H
#define THREADLITE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#endif

#define TIME_INT uint16_t
#define TIME_OVERFLOW 0x8000 // uint16_t = 0x8000 or uint32_t = 0x80000000

class ThreadLite {
  protected:
    // Desired interval between runs
    TIME_INT interval;

    // Last runned time in Ms
    TIME_INT last_run;

    // Scheduled run in Ms (MUST BE CACHED)
    TIME_INT _cached_next_run;

    /*
    	IMPORTANT! Run after all calls to run()
    	Updates last_run and cache next run.
    	NOTE: This MUST be called if extending
    	this class and implementing run() method
    */

    // Default is to mark it runned "now"
    void runned();

    // Callback for run() if not implemented
    void (*_onRun)(void);

  public:
    ThreadLite(void (*callback)(void), TIME_INT _interval);

    // Set the desired interval for calls, and update _cached_next_run
    void setInterval(TIME_INT _interval);

    // Default is to check whether it should run "now"
    bool shouldRun();

    // Runs ThreadLite
    void run();
};

#endif // THREADLITE_H
