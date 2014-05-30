/*
	This is an example from ArduinoThread. You can find more information
	in https://github.com/ivanseidel/ArduinoThread.

	Coded by Ivan Seidel, Jun/2014 - ivanseidel@gmail.com

	Dont be afraid. 90% is commented lines. READ them, they will teach you.
*/

#include <Thread.h>
#include <ThreadController.h>

/*
	This example provides an object-oriented approach to
	develop a custom Thread that overrides the 'shouldRun'
	method, to only run the thread after a button was pushed.

	After the push, it should 'keep' running for a desired time.

	It should also provide us, a way to easily implement this
	controll multiple times, without trouble.

	We are giving this Custom Thread the name 'ButtonThread'.

	Exemplifying what it does:
		+ ButtonThread added to our mainController ThreadList
			=> Instantiated with a custom Pin #,
			=> and a time duration (in miliseconds)
		
		+ ButtonThread is not running.

		+ When the button is pressed:
			+ Thread will start and keep running.
			+ If the thread runned for our defined period,
			  we stop it.

	================ HOW TO SETUP HARDWARE ==================
	In order to make this example work with any arduino, hook up
	the pins on the board to 3 buttons. You can change the inputs
	if you need below here.

	The Buttons are being SOFTWARE pulled UP (to VCC), and when
	pushed, should go LOW. Connect like this:
	(Arduino Input) <----> (Btn) <----> GND (-)

	We are using digital pins 9, 10 and 11 as input.
	It also uses a LED, but we are using the default one in the board.

	=============== WHAT YO LEARN WITH THIS =================
	1) Threads are actually running in 'parallel'.
	   
	   Synce each thread process time is very tiny, they appear
	   as being runned in parallel.

	   Because of that, clicking multiple buttons at any time,
	   will looks like there is a program for each one of them.

	2) If you keep the button 'pressed', it will continue to run.

	   Since we are 'enabling' the thread, and reseting the timer
	   flag (_lastButtonPushed) every time the button is pressed,
	   we should notice that in btn1Callback, where we print this
	   flag, it will never go beyond 0 if we keep pressing it.

	3) The LED turns off, only because the Thread runs a last time
	   with the flag 'enabled' as false. This way, we can turn the
	   LED off and remain OFF until we press it egain.

	I hope you enjoy, and learn some advanced-cool stuf with this tutorial.
	Any feedback is apreciated!
*/
#define BTN1	9
#define BTN2	10
#define BTN3	11

#define LED 	13

// ThreadController that will controll all button threads
ThreadController controll = ThreadController();

// Here we implement our custom ButtonThread, that Inherits from Thread
class ButtonThread: public Thread{
public:
	// Our custom thread attributes
	int pin;
	long duration;
	long _lastButtonPushed;

	/*
		Our Constructor. This will initialize the thread
		with it's corresponding pin and duration after clicked.
	*/
	ButtonThread(int _pin, long _duration): Thread(){
		// Set our attributes on construct
		pin = _pin;
		duration = _duration;
		_lastButtonPushed = 0;

		// Thread will start disabled
		enabled = false;

		// Configure the pin as INPUT and enable pull-up
		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);
	}

	/*
		Override the method responsible for
		checking if the thread should run.

		It will first check if the button is pressed.
		If so, we enable the thread, and then let the
		"Old" default Thread method 'shouldRun' return if
		it should run.
	*/
	bool shouldRun(long time){
		// Override enabled on thread when pin goes LOW.
		if(digitalRead(pin) == LOW){
			enabled = true;
			/*
				Here, we save the current time in this object,
				to compare it later.

				the 'time' parameter in this method, is an override for the
				'millis()' method. It allows who is checking the thread, to
				pass a custom time.

				This is sintax for writing an 'inline' if is very usefull, 
				it's the same as:
					if(time > 0){
						_lastButtonPushed = time;
					}else{
						_lastButtonPushed = millis();
					}
			*/
			_lastButtonPushed = (time ? time : millis());
		}

		// Let default method check for it.
		return Thread::shouldRun(time);
	}

	/*
		We 'disable' the thread after the duration on the
		'run' method.

		What we should do here, is check if the time saved
		in the _lastButtonPushed variable plus the duration,
		is greater than our current time. If that's true, it
		means we exceeded the thread time, and that we must
		disable it and prevent from running.
	*/
	void run(){
		// Check if time elapsed since last button push
		if(millis() > _lastButtonPushed + duration){
			// It exceeded time. We should disable it.
			enabled = false;
		}

		/*
			Run the thread.

			Note that this method will only get called
			from the ThreadList, IF the 'shouldRun' returns true.

			If the thread is not enabled anymore, it will run a 'last'
			time with the flag 'enabled' as false, meaning it's the last
			run in the period. You can use it for doing something only
			before it stops running.
		*/
		Thread::run();
	}
};

/*
	ButtonThreads objects instantiation
	(we are instantiating 2 as a member, and one
	as pointer in the setup, just to show you
	different ways of doing it)
*/

// Thread 1 will be reading BTN1 pin, and will run for 3 secs
ButtonThread btn1Thread(BTN1, 3000);

// Thread 2 will be reading BTN1 pin, and will run for 5 secs
ButtonThread btn2Thread = ButtonThread(BTN2, 5000);

// Thread 3 will be instantiated in the setup()
ButtonThread *btn3Thread;


/*
	Callback for ButtonThreads
*/
void btn1Callback(){
	// When it's running, this thread will write to the serial.
	/*
		This math will print 'how long' the thread has been running,
		since the button was/is pressed.

		After pressing it, it should print as 0, and goes up untill
		the thread duration (in this case, +-5000ms).
	*/
	Serial.print("BTN 1 Thread: ");
	Serial.println(millis() - btn1Thread._lastButtonPushed);
}

void btn2Callback(){
	/*
		This thread will remain with the LED on pin 13 turned on
		while it is running.

		We detect that this method is called for the LAST time, if
		the flag 'enabled' is FALSE on the btn2Thread object.

		So, basically: If it's TRUE, we should turn ON the led, if not
		we should turn OFF. We can simplify that into one line.
		(Same 'inline' sintax as above)
	*/
	digitalWrite(LED, btn2Thread.enabled ? HIGH : LOW);
}

void btn3Callback(){
	// When it's running, this thread will also write to the serial
	Serial.println("BTN 3 Thread");
}

void setup(){
	// Configure serial and output pins
	Serial.begin(9600);
	pinMode(LED, OUTPUT);

	// Configure btn1Thread callback
	// (During the 'enabled' time, it will run every 100ms, aka Interval)
	btn1Thread.onRun(btn1Callback);
	btn1Thread.setInterval(100); 

	// Configure btn2Thread callback and interval
	btn2Thread.onRun(btn2Callback);
	btn2Thread.setInterval(200);

	// Instantiate btn3Thread
	btn3Thread = new ButtonThread(BTN3, 4000);
	// Configure btn3Thread callback and interval
	btn3Thread->onRun(btn3Callback);
	btn3Thread->setInterval(100);

	// Adds all threads to the controller
	controll.add(&btn1Thread); // & to pass the pointer to it
	controll.add(&btn2Thread);
	controll.add(btn3Thread);  // Its already a pointer, no need for &
}

void loop(){
	// Here we just run the main thread controller
	controll.run();
}