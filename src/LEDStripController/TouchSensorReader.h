// TouchSensorReader.h

#ifndef _TOUCHSENSORREADER_h
#define _TOUCHSENSORREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define TOUCHSTATE_IDLE 0
#define TOUCHSTATE_READINGINPUT 1
#define TOUCHSTATE_READINGLONGINPUT 2
#define LONG_INPUT_THRESHOLD_TIME 500
#define LONG_INPUT_STEP_TIME 250

enum TouchResult :uint8_t { nothing = 0, toggle = 1, long_press = 2 };

class TouchSensorReaderClass
{
private:
	uint8_t _sensorPin;
	uint8_t _touchState;
	unsigned long _startedReadingAt;
	unsigned long _previouslyIncreasedAt;

public:
	TouchSensorReaderClass(uint8_t sensorPin) :_sensorPin(sensorPin) {};
	void init();
	TouchResult process();
private:
	void EnterIdleState();
};

#endif

