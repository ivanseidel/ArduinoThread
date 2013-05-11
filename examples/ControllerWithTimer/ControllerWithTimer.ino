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

//My Thread
Thread myThread = Thread();
//His Thread
Thread hisThread = Thread();

// callback for myThread
void myThreadCallback(){
	Serial.println("myThread\t\tcallback");
}

// callback for hisThread
void hisThreadCallback(){
	Serial.println("\thisThread\tcallback");
}
	
// This is the callback for the Timer
void timerCallback(){
	controll.run();
}

void setup(){
	Serial.begin(9600);

	// Configure myThread
	myThread.onRun(myThreadCallback);
	myThread.setInterval(500);

	// Configure myThread
	hisThread.onRun(hisThreadCallback);
	hisThread.setInterval(200);

	// Adds both threads to the controller
	controll.add(&myThread); // & to pass the pointer to it
	controll.add(&hisThread);

	/*
		If using DueTimer...
	*/
	// Timer1.attachInterrupt(timerCallback).start(20000);

	/*
		If using TimerOne...
	*/
	// Timer1.initialize(20000);
	// Timer1.attachInterrupt(timerCallback);
	// Timer1.start();
}

void waitSerial(){
	while (!Serial.available());
	delay(10);
    while (Serial.available() && Serial.read());	
}

void loop(){
	while(1){
		noInterrupts();	// Call to disable interrupts
		Serial.println("Type anyting to stop myThread!");
		interrupts();	// Call to enable interrupts
		waitSerial();
		myThread.enabled = false;

		noInterrupts();
		Serial.println("Type anyting to stop hisThread!");
		interrupts();
		waitSerial();
		hisThread.enabled = false;

		noInterrupts();
		Serial.println("Type anyting to enable myThread!");
		interrupts();
		waitSerial();
		myThread.enabled = true;

		noInterrupts();
		Serial.println("Type anyting to enable hisThread!");
		interrupts();
		waitSerial();
		hisThread.enabled = true;

	}
}