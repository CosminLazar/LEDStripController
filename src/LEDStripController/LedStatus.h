// LedStatus.h

#ifndef _LEDSTATUS_h
#define _LEDSTATUS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class LedStatusClass
{
private:
	bool _isOn;
	uint8_t _r;
	uint8_t _g;
	uint8_t _b;
	uint8_t _brightness;
public:
	///<summary>Holds the LED state</summary>
	///<param name="isOn">Boolean indicating the power state</param>
	///<param name="r">Color: Red component (range 0-255)</param>
	///<param name="g">Color: Green component (range 0-255)</param>
	///<param name="b">Color: Blue component (range 0-255)</param>
	///<param name="brightness">Brightness (range 0-100)</param>
	LedStatusClass(bool isOn, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) 
		:_isOn(isOn), _r(r), _g(g), _b(b), _brightness(brightness) {};

	bool isOn();
	uint8_t get_R();
	uint8_t get_G();
	uint8_t get_B();	
	uint8_t get_Brigthness();
};

extern LedStatusClass LedStatus;

#endif

