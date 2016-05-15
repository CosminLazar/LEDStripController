// SensitiveData.h

#ifndef _SENSITIVEDATA_h
#define _SENSITIVEDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
class SensitiveData
{
public:
	static const char * WIFI_SSID;
	static const char * WIFI_PASSWORD;
	static const char * MQTT_HOST;
	static const char * MQTT_CLIENT;
	static const char * MQTT_USER;
	static const char * MQTT_PASSWORD;
};

#endif

