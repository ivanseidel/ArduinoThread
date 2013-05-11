#include <Thread.h>
#include <ThreadController.h>

/*
	This example, requires a Timer Interrupt Library.
	If you are using Arduino NANO, UNO... (with ATmega168/328)
		Please go to: http://playground.arduino.cc/code/timer1
	If you are using Arduino DUE,
		Please go to: https://github.com/ivanseidel/DueTimer

	Include the library corresponding to your Arduino.
*/
// #include <DueTimer.h>
// #include <TimerOne.h>

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

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
	
// This is the callback for the Timer
void timerCallback(){
	controll.run();
}

void setup(){
	Serial.begin(9600);

	// Configure myThread
	myThread->onRun(niceCallback);
	myThread->setInterval(500);

	// Configure myThread
	hisThread.onRun(boringCallback);
	hisThread.setInterval(250);

	// Adds both threads to the controller
	controll.add(myThread);
	controll.add(&hisThread); // & to pass the pointer to it

	/*
		If using DueTimer...
	*/
	// Timer1.attachInterrupt(timerCallback).start(10000);

	/*
		If using TimerOne...
	*/
	// Timer1.initialize(20000);
	// Timer1.attachInterrupt(timerCallback);
	// Timer1.start();
}

void loop(){
	while(1){
		float h = 3.1415;
		h/=2;
		Serial.println("Help me! I'm stuck here...");
		delay(1000);
	}
}