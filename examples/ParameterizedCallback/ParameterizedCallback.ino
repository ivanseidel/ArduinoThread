/*
  This example shows how to use a single callback function for multiple threads.
  Each of 5 threads lights up one of 5 LEDs in randomly selected intervals.
  Single callback function is called with a pointer to a data structure, which
  can be used for identifying a pin it needs to light up and thread point to set up
  a new waiting interval.
  
  author:    Ivan Koryakovskiy <i.koryakovskiy@gmail.com>
  date:      2017-07-29
*/

#include <Thread.h>
#include <ThreadController.h>

typedef struct blinkParam
{
  Thread *th;
  int pin;
};

ThreadController g_controller = ThreadController();

// Creating 5 controllers, each will call same callback function, 
// but with a different thread pointer as a parameter, and therefore different led
const int g_th_num = 5;
Thread *g_th[g_th_num];
blinkParam *g_param[g_th_num];

int nextInterval()
{
  // next time call thread after 1-5 seconds
  return 1000 + random(4000);
}

void blinkLed(void *param)
{
  // cast parameters to data structure
  blinkParam *bp = static_cast<blinkParam*>(param);

  digitalWrite(bp->pin, HIGH);
  delayMicroseconds(10000);
  digitalWrite(bp->pin, LOW);

  bp->th->setInterval(nextInterval());  
}

void setup()
{
  randomSeed(analogRead(0));

  // create five threads to light up pins D9-D13 (Arduino Nano)
  int pin = 9;
  for (int i = 0; i < g_th_num; i++)
  {
    g_param[i] = new blinkParam;

    // note that parameter includes placeholders for thread pointer and pin number
    g_th[i] = new Thread(blinkLed, nextInterval(), static_cast<void*>(g_param[i]));

    g_param[i]->th = g_th[i];
    g_param[i]->pin = pin + i;
    
    g_controller.add(g_th[i]);

    pinMode(g_param[i]->pin, OUTPUT);
  }
}

void loop()
{
	g_controller.run();
}
