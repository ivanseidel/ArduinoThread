#include <Thread.h>
#include <ThreadController.h>

int ledPin = 13;

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//My Thread
Thread myThread = Thread();
//His Thread
Thread hisThread = Thread();
//Blink Led Thread
Thread blinkLedThread = Thread();
//ThreadController, that will be added to controll
ThreadController groupOfThreads = ThreadController();

// callback for myThread
void niceCallback(){
	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

// callback for hisThread
void boringCallback(){
	Serial.println("BORING...");
}

// callback for blinkLedThread
void blinkLed(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("blinking: ");
	Serial.println(ledStatus);
}

void setup(){
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);

	// Configure myThread
	myThread.onRun(niceCallback);
	myThread.setInterval(500);

	// Configure hisThread
	hisThread.onRun(boringCallback);
	hisThread.setInterval(250);
	
	// Configure blinkLedThread
	blinkLedThread.onRun(blinkLed);
	blinkLedThread.setInterval(100);

	// Adds myThread to the controll
	controll.add(&myThread);

	// Adds hisThread and blinkLedThread to groupOfThreads
	groupOfThreads.add(&hisThread);
	groupOfThreads.add(&blinkLedThread);

	// Add groupOfThreads to controll
	controll.add(&groupOfThreads);
	
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

	// Rest of code
	float h = 3.1415;
	h/=2;
}