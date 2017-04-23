// LedHeper.h

#ifndef _LEDHEPER_h
#define _LEDHEPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_NeoPixel.h>
#include "MqttParameters.h"
#include "TouchSensorReader.h"

#define PENDING_POWER_REPORT 0
#define PENDING_BRIGHTNESS_REPORT 1
#define PENDING_HUE_REPORT 2
#define PENDING_SATURATION_REPORT 3

#define PENDING_REPORT_DEBOUNCE 300
#define HARDWARE_UPDATE_DEBOUNCE 250

class LedHeperClass
{
protected:
	const MqttParametersClass * mqttParameters;
	Adafruit_NeoPixel* strip;
	TouchSensorReaderClass * touchSensorReader;
	uint8_t pendingStatusReport[4] = { 0,0,0,0 };
	bool _hardwareUpdateRequested = true;
	unsigned long _hardwareUpdateRequestTimestamp = 0;
	unsigned long _statusReportRequestTimestamp = 0;
	bool _isOn = false;
	uint8_t _brightness = 0;
	float _hue = 0;
	float _saturation = 0;	
public:
	LedHeperClass(uint8_t noOfLeds, uint8_t pin, const MqttParametersClass * mqttParameters, uint8_t touchSensorPin);
	void init();
	void process();
	bool isOn();
	void powerOn();
	void powerOff();
	void setBrightness(uint8_t brigtness);
	void setHue(float hue);
	void setSaturation(float saturation);
private:
	void onMqttConnected(void * data);
	void onMqttMessage(void * data);

	void requestPowerStateReport();
	void requestBrightnessStateReport();
	void requestHueStateReport();
	void requestSaturationStateReport();

	void requestUpdateHardwareState();
	void processHardwareUpdates();
	void updateHardwareState();

	void processTouchSensor();
	void processPendingStatusReports();

	static void HSV_to_RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
};

#endif

