// Copyright © 2013 Ivan Seidel Gomes.
// Copyright © 2018 Stanislav Hnatiuk.

#include "ThreadLite.h"

ThreadLite::ThreadLite(void (*callback)(void), TIME_INT _interval) {
  _onRun = callback;
  _cached_next_run = 0;
  last_run = 0;
  setInterval(_interval);
};

void ThreadLite::runned() {
  // Saves last_run
  last_run = millis();

  // Cache next run
  _cached_next_run = last_run + interval;
}

void ThreadLite::setInterval(TIME_INT _interval) {
  // Save interval
  interval = _interval;

  // Cache the next run based on the last_run
  _cached_next_run = last_run + interval;
}

bool ThreadLite::shouldRun() {
  TIME_INT time = millis();
  // If the "sign" bit is set the signed difference would be negative
  bool time_remaining = (time - _cached_next_run) & TIME_OVERFLOW;

  // Exceeded the time limit, AND is enabled? Then should run...
  return !time_remaining;
}

void ThreadLite::run() {
  _onRun();
  // Update last_run and _cached_next_run
  runned();
}
