// LedHeper.h

#ifndef _LEDHEPER_h
#define _LEDHEPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Adafruit_NeoPixel.h>
#include "LedStatus.h"

class LedHeperClass
{
 protected:
	 Adafruit_NeoPixel* strip;

 public:
	 LedHeperClass(uint8_t noOfLeds, uint8_t pin);
	void init();
	void allWhite();
	void set(LedStatusClass status);
private:
	///<summary>Maps brightness from [0,100] interval to [0,255]</summary>
	///<param name="brightness">Brightness value in [0,100] range</param>	
	uint8_t mapBrightness(uint8_t brightness);
};

extern LedHeperClass LedHeper;

#endif

