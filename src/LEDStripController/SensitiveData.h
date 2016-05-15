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
	static void Read_WIFI_SSID(char * buffer);
	static void Read_WIFI_PASSWORD(char * buffer);
	static void Read_MQTT_HOST(char * buffer);
	static void Read_MQTT_CLIENT(char * buffer);
	static void Read_MQTT_USER(char * buffer);
	static void Read_MQTT_PASSWORD(char * buffer);
};

#endif

