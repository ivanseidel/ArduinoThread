#include "Thread.h"
#include "ThreadController.h"
/*
	This is a more "complex" for of using Threads.
	You can also inherit from Thread, and do your entire code on the class.

	This allows you, to create for example:
		Sensor Readings (aquire, filter, and save localy values)
		Custom Blinks, Beeps...
		Anything you can imagine.

	Threads are more "usefull" when used within Timer interrupts

	This way of coding is more "reusable", and "correct" (Object Oriented)
*/


/*
	This example, requires a Timer Interrupt Library.
	If you are using Arduino NANO, UNO... (with ATmega168/328)
		Please go to: http://playground.arduino.cc/code/timer1
	If you are using Arduino DUE,
		Please go to: https://github.com/ivanseidel/DueTimer

	Include the library corresponding to your Arduino.
*/
#include <DueTimer.h>
// #include <TimerOne.h>

// Create a new Class, called SensorThread, that inherits from Thread
class SensorThread: public Thread
{
public:
	int value;
	int pin;

	// No, "run" cannot be anything...
	// Because Thread uses the method "run" to run threads,
	// we MUST overload this method here. using anything other
	// than "run" will not work properly...
	void run(){
		// Reads the analog pin, and saves it localy
		value = map(analogRead(pin), 0,1023,0,255);
		runned();
	}
};

// Now, let's use our new class of Thread
SensorThread analog1 = SensorThread();
SensorThread analog2 = SensorThread();

// Instantiate a new ThreadController
ThreadController controller = ThreadController();

// This is the callback for the Timer
void timerCallback(){
	controller.run();
}

void setup(){

	Serial.begin(9600);

	// Configures Thread analog1
	analog1.pin = A1;
	analog1.setInterval(100);

	// Configures Thread analog2
	analog2.pin = A2;
	analog2.setInterval(100);

	// Add the Threads to our ThreadController
	controller.add(&analog1);
	controller.add(&analog2);

	/*
		If using DueTimer...
	*/
	Timer1.attachInterrupt(timerCallback).start(10000);

	/*
		If using TimerOne...
	*/
	// Timer1.initialize(20000);
	// Timer1.attachInterrupt(timerCallback);
	// Timer1.start();

}

void loop(){
	// Do complex-crazy-timeconsuming-tasks here
	delay(1000);
	
	// Get the fresh readings
	Serial.print("Analog1 Thread: ");
	Serial.println(analog1.value);

	Serial.print("Analog2 Thread: ");
	Serial.println(analog2.value);

	// Do more complex-crazy-timeconsuming-tasks here
	delay(1000);

}

