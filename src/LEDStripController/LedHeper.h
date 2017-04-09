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
#include "MqttParameters.h"

class LedHeperClass
{
protected:
	const MqttParametersClass * mqttParameters;
	Adafruit_NeoPixel* strip;
	LedStatusClass currentState = LedStatusClass(false, 0, 0, 0, 0);
	bool _isOn = false;
	uint8_t _brightness = 0;
	float _hue = 0;
	float _saturation = 0;
public:
	LedHeperClass(uint8_t noOfLeds, uint8_t pin, const MqttParametersClass * mqttParameters);
	void init();
	void powerOn();
	void powerOff();
	void setBrightness(uint8_t brigtness);
	void setHue(float hue);
	void setSaturation(float saturation);
private:
	void onMqttConnected(void * data);
	void onMqttMessage(void * data);
	void reportPowerState();
	void reportBrightnessState();
	void reportHueState();
	void reportSaturationState();
	void updateHardwareState();
	void HSV_to_RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
};

#endif

