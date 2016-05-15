// MqttHelper.h

#ifndef _MQTTHELPER_h
#define _MQTTHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <espduino.h>
#include <mqtt.h>
#include "SensitiveData.h"

class MqttHelperClass
{
private:
	ESP *esp;
	MQTT *mqtt;
	void wifiCallback(void* response);
	void mqttConnected(void* response);
	void mqttDisconnected(void* response);
	void mqttDataCallback(void* response);
public:
	bool wifiConnected;
	bool mqttIsConnected;
	MqttHelperClass();
	void init();
	void process();
};

extern MqttHelperClass MqttHelper;

#endif

