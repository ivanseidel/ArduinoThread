![ArduinoThread Logo](https://raw.githubusercontent.com/ivanseidel/ArduinoThread/master/extras/ArduinoThread.png)

## ArduinoThreads Motivation
Arduino does not support isolated parallel tasks ([Threads](https://en.wikipedia.org/wiki/Thread_(computing))), 
but we can make the main `loop` switch function execution conditionally and 
thus simulate threading with [Protothread](https://en.wikipedia.org/wiki/Protothread) mechanism. 
This library implements it and helps you to:

- schedule, manage and simplify parallel, periodic tasks 
- define fixed or variable time between runs
- organize the code in any type of project
    - put all sensor readings in a thread
    - keep the main loop clean
- hide the complexity of thread management
- run "pseudo-background" tasks using Timer interrupts

Blinking an LED is often the very first thing an Arduino user learns. 
And this demonstrates that periodically performing one single task, like toggling the LED state, is really easy. 
However, one may quickly discover that managing multiple periodic tasks is not so simple 
if the tasks have different schedule.

The user defines a Thread object for each of those tasks, then lets the library manage their scheduled execution.

It should be noted that these are not “threads” in the real computer-science meaning of the term: 
tasks are implemented as functions that are run periodically. 
On the one hand, this means that the only way a task can *yield* the CPU is by returning to the caller, 
and it is thus inadvisable to `delay()` or do long waits inside any task. 
On the other hand, this makes ArduinoThreads memory friendly, as no stack need to be allocated per task.

## Installation

1. Download [the Master branch](https://github.com/ivanseidel/ArduinoThread/archive/master.zip) from gitHub.
2. Unzip and modify the Folder name to "ArduinoThread" (Remove the '-master' suffix)
3. Paste the modified folder on your Library folder (On your `Libraries` folder inside Sketchbooks or Arduino software).
4. Restart the Arduino IDE

**If you are here just because another library requires a class from ArduinoThread, then you are done now
.**


## Getting Started

There are many examples showing many ways to use it. We will explain Class itself,
what it does and how it does.

There are three main classes included in the library:
`Thread`, `ThreadController` and `StaticThreadController` (both controllers inherit from `Thread`).

- `Thread`: Basic class, witch contains methods to set and run callbacks,
  check if the Thread should be run, and also creates a unique ThreadID on the instantiation.

- `ThreadController`: Responsible for managing multiple Threads. Can also be thought of 
  as "a group of Threads", and is used to perform `run` in every Thread ONLY when needed.

- `StaticThreadController`: Slightly faster and smaller version of the `ThreadController`.
  It works similar to `ThreadController`, but once constructed it can't add or remove threads to run.

#### Create Thread instance:

```c++
Thread myThread = Thread();
// or, if initializing a pointer
Thread* myThread = new Thread();
```

#### Setup thread behaviour
You can configure many things:

```c++
myThread.enabled = true; // Default enabled value is true
myThread.setInterval(10); // Setts the wanted interval to be 10ms
/*
	This is useful for debugging
	(Thread Name is disabled by default, to use less memory)
	(Enable it by definint USE_THREAD_NAMES on 'Thread.h')
*/
myThread.ThreadName = "myThread tag";
// This will set the callback of the Thread: "What should I run"?
myThread.onRun(callback_function); // callback_function is the name of the function
```

#### Running threads manually
Ok, creating threads isn't too hard, but what do we do with them?

```c++
// First check if our Thread should be run
if(myThread.shouldRun()){
  // Yes, the Thread should run, let's run it
  myThread.run();
}
```

#### Running threads via a controller
If you had 3, 5 or 100 threads, managing them manually could become tedious. 
That's when `ThreadController` or `StaticThreadController` comes into play and saves you the repetitive thread management parts of code.

```c++
// Instantiate new ThreadController
ThreadController controller = ThreadController();
// Now, put bunch of Threads inside it, FEED it!
controller.add(&myThread); // Notice the '&' sign before the thread, IF it's not instantied as a pointer.
controller.add(&hisThread);
controller.add(&sensorReadings);
...
```
or
```c++
// Instantiate a new StaticThreadController with the number of threads to be supplied as template parameter
StaticThreadController<3> controller (&myThread, &hisThread, &sensorReadings);
// You don't need to do anything else, controller now contains all the threads.
...
```

You have created, configured, grouped it. What is missing? Yes, whe should RUN it! 
The following will run all the threads that NEED to run.

```c++
// call run on a Thread, a ThreadController or a StaticThreadController to run it
controller.run();
```

Congratulations, you have learned the basics of the `ArduinoThread` library. If you want to learn more, see bellow.

### Tips and Warnings

* `ThreadController` is not of a dynamic size (like a `LinkedList`). The maximum number of threads that it can manage
  is defined in `ThreadController.h` (default is 15)

* ☢ When extending the `Thread` class and overriding the `run()` function, 
    remember to always call `runned();` at the end, otherwise the thread will hang forever.

* It's a good idea, to create a `Timer` interrupt and call a `ThreadController.run()` there.
That way, you don't need to worry about reading sensors and doing time-sensitive stuff
in your main code (`loop`). Check `ControllerWithTimer` example.

* Inheriting from `Thread` or even `ThreadController` is always a good idea.
For example, I always create base classes of sensors that extends `Thread`,
so that I can "register" the sensors inside a `ThreadController`, and forget
about reading sensors, just having the values available in my main code.
Check the `SensorThread` example.

* Remember that `ThreadController` is in fact, a `Thread` itself. If you want to group threads and 
manage them together (enable or disable), think about putting all of them inside a `ThreadController`,
and adding this `ThreadController` to another `ThreadController` (YES! One inside another). 
Check `ControllerInController` example.

* `StaticThreadController` is optimal when you know the exact number of
threads to run. You cannot add or remove threads at runtime, but it
doesn't require additional memory to keep all the treads together, doesn't limit the number of thread 
(except for available memory) and the code may be slightly
better optimized because all the threads always exist and no need to do any runtime checks.

* Check the full example `CustomTimedThread` for a cool application of threads that run
for a period, after a button is pressed.

* Running tasks on the `Timer` interrupts must be thought though REALLY carefully
  
  - You mustn't use `sleep()` inside an interrupt, because it would cause an infinite loop.
  
  - Things execute quickly. Waiting too loooong on a interrupt, means waiting too
  loooong on the main code (`loop`)

  - Things might get "scrambled". Since Timers interrupts actually "BREAK" your code in half
  and start running the interrupt, you might want to call `noInterrupts` and `interrupts`
  on places where cannot be interrupted:

```c++
noInterrupts();
// Put the code that CANNOT be interrupted...
interrupts(); // This will enable the interrupts egain. DO NOT FORGET!
```
## Library Reference

### Configuration options
#### Thread
- `bool Thread::enabled` - Enables or disables the thread. (doesn't prevent it from running, but will
  return `false` when `shouldRun()` is called)
- `void Thread::setInterval()` - Schedules the thread run interval in milliseconds
- `bool Thread::shouldRun()` - Returns true, if the thread should be run.
  (Basically,the logic is: (reached time AND is enabled?).
- `void Thread::onRun(<function>)` - The target callback function to be called.
- `void Thread::run()` - Runs the thread (executes the callback function).
- `int Thread::ThreadID` - Theoretically, it's the memory address. It's unique, and can
  be used to compare if two threads are identical.
- `int Thread::ThreadName` - A human-readable thread name. 
    Default is "Thread ThreadID", eg.: "Thread 141515". 
    Note that to enable this attribute, you must uncomment the line that disables it on `Thread.h`
- protected: `void Thread::runned()` - Used to reset internal timer of the thread.
  This is automatically called AFTER a call to `run()`.

#### ThreadController  
- `void ThreadController::run()` - Runs the all threads grouped by the controller, 
    but only if needed (if `shouldRun()`  returns true);
- `bool ThreadController::add(Thread* _thread)` - Adds a the thread to the controller,
  and returns `true` if succeeded (returns false if the array is full).
- `void ThreadController::remove(Thread* _thread)` - Removes the thread from the controller
- `void ThreadController::remove(int index)` - Removes the thread at the `index` position
- `void ThreadController::clear()` - Removes ALL threads from the controller
- `int ThreadController::size(bool cached = true)` - Returns number of threads allocated 
  in the ThreadController. Re-calculates thread count if `cached` is `false`
- `Thread* ThreadController::get(int index)` - Returns the thread at the `index` position

#### StaticThreadController
- `void StaticThreadController::run()` - Runs all the threads within the controller,
    but only if needed (if `shouldRun()`  returns true);
- `int StaticThreadController::size()` - Returns how many Threads are allocated inside the controller.
- `Thread* ThreadController::get(int index)` - Returns the thread at the `index` position - or `nullptr` if `index`
  is out of bounds.
