// LedHeper.h

#ifndef _LEDHEPER_h
#define _LEDHEPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Adafruit_NeoPixel.h>

class LedHeperClass
{
 protected:
	 Adafruit_NeoPixel* strip;

 public:
	 LedHeperClass(uint8_t noOfLeds, uint8_t pin);
	void init();
	void allWhite();
};

extern LedHeperClass LedHeper;

#endif

