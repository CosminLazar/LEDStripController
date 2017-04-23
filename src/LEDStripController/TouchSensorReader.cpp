// 
// 
// 

#include "TouchSensorReader.h"

void TouchSensorReaderClass::init()
{
	pinMode(_sensorPin, INPUT);

	_touchState = 0;
	_startedReadingAt = 0;
	_previouslyIncreasedAt = 0;
}

TouchResult TouchSensorReaderClass::process()
{
	int isPressed = digitalRead(_sensorPin);

	if (_touchState == TOUCHSTATE_INACTIVE) {
		if (isPressed == 1)
		{
			_startedReadingAt = millis();
			_touchState = TOUCHSTATE_READINGINPUT;
		}
	}
	else {
		if (_touchState == TOUCHSTATE_READINGINPUT) {
			if (isPressed == 1) {
				if (millis() - _startedReadingAt > 500) {
					_touchState = TOUCHSTATE_READINGLONGINPUT;
				}
			}
			else {
				//was on, now off -> toggle
				_touchState = 0;
				_startedReadingAt = 0;
				_previouslyIncreasedAt = 0;

				return TouchResult::toggle;
				
				/*if (led1->isOn())
				{
					led1->powerOff();
				}
				else {
					led1->powerOn();
					if (led1->getBrightness() == 0)
						led1->setBrightness(100);
				}*/

				
			}
		}
		else {
			if (_touchState == TOUCHSTATE_READINGLONGINPUT) {
				//if (!led1->isOn())
				//{
				//	led1->powerOn();
				//	//led1->setBrightness(250);
				//}


				if (isPressed == 1)
				{

					if (millis() - _previouslyIncreasedAt > 250) {
						/*uint8_t v = led1->getBrightness();
						v = v + 25;
						led1->setBrightness(v);*/
						_previouslyIncreasedAt = millis();
						return TouchResult::long_press;
					}
				}
				else {
					//reset
					_touchState = 0;
					_startedReadingAt = 0;
					_previouslyIncreasedAt = 0;
				}
			}
		}
	}

	return TouchResult::nothing;
}

