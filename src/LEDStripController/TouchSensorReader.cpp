// Three states: Idle, ReadingInput, ReadingLongInput
// Transitions
//		Idle -> ReadingInput				(when: touch sensor activated)
//		ReadingInput -> Idle				(when: touch sensor activated and deactivated => toggle)
//		ReadingInput -> ReadingLongInput	(when: touch sensor activated for a long period => long_press)
//		ReadingLongInput -> Idle			(when: touch sensor deactivated)

#include "TouchSensorReader.h"

void TouchSensorReaderClass::init()
{
	pinMode(_sensorPin, INPUT);

	EnterIdleState();
}

TouchResult TouchSensorReaderClass::process()
{
	int isPressed = digitalRead(_sensorPin);

	if (_touchState == TOUCHSTATE_IDLE) {
		if (isPressed == 1)
		{
			if (_startedReadingAt == 0)
			{
				_startedReadingAt = millis();
			}
			else {
				if (millis() - _startedReadingAt > READING_INPUT_THRESHOLD_TIME)
					_touchState = TOUCHSTATE_READINGINPUT;
			}
		}
		else {
			EnterIdleState();
		}
	}
	else {
		if (_touchState == TOUCHSTATE_READINGINPUT) {
			if (isPressed == 1) {
				if (millis() - _startedReadingAt > LONG_INPUT_THRESHOLD_TIME) {
					_touchState = TOUCHSTATE_READINGLONGINPUT;
				}
			}
			else {
				EnterIdleState();

				//was on, now off -> toggle
				return TouchResult::toggle;
			}
		}
		else {
			if (_touchState == TOUCHSTATE_READINGLONGINPUT) {
				if (isPressed == 1)
				{
					if (millis() - _previouslyIncreasedAt > LONG_INPUT_STEP_TIME) {
						_previouslyIncreasedAt = millis();
						return TouchResult::long_press;
					}
				}
				else {
					//reset
					EnterIdleState();
				}
			}
		}
	}

	return TouchResult::nothing;
}

void TouchSensorReaderClass::EnterIdleState()
{
	_touchState = TOUCHSTATE_IDLE;
	_startedReadingAt = 0;
	_previouslyIncreasedAt = 0;
}

