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
	uint8_t _isOn;
	uint8_t _r;
	uint8_t _g;
	uint8_t _b;
public:
	LedStatusClass(uint8_t isOn, uint8_t r, uint8_t g, uint8_t b) :_isOn(isOn), _r(r), _g(g), _b(b) {};
	bool isOn();
	uint8_t get_R();
	uint8_t get_G();
	uint8_t get_B();
};

extern LedStatusClass LedStatus;

#endif

