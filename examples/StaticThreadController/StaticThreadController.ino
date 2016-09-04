#include <Thread.h>
#include <StaticThreadController.h>

//My Thread (as a pointer)
Thread* myThread = new Thread();
//His Thread (not pointer)
Thread hisThread = Thread();

// callback for myThread
void niceCallback(){
	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

// callback for hisThread
void boringCallback(){
	Serial.println("BORING...");
}

// callback for theThread
void justCallback(){
	Serial.println("executing...");
}

//The Thread (as a pointer) with justCallback initialized
Thread* theThread = new Thread(justCallback);

// StaticThreadController that will controll all threads
// All non-pointers go with '&', but pointers go without '&', 
StaticThreadController<3> controll (myThread, &hisThread, theThread);

void setup(){
	Serial.begin(9600);

	// Configure myThread
	myThread->onRun(niceCallback);
	myThread->setInterval(500);

	// Configure hisThread
	hisThread.onRun(boringCallback);
	hisThread.setInterval(250);

	// Set interval for theThread using StaticThreadController interface
	controll[3].setInterval(375);
}

void loop(){
	// run StaticThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

	// Rest of code
	float h = 3.1415;
	h/=2;
}
