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
#include <FP.h>
#include <LinkedList.h>
#include <HashMap.h>

#define WIFI_WATCHDOG_INTERVAL 10000

class MqttHelperClass
{
private:
	unsigned long lastWifiDogTimestamp = 0;
	ESP *esp;
	MQTT *mqtt;
	LinkedList<FP<void, void*>> connectCb = LinkedList<FP<void, void*>>();
	LinkedList<FP<void, void*>> disconnectCb = LinkedList<FP<void, void*>>();
	LinkedList<FP<void, void*>> dataCb = LinkedList<FP<void, void*>>();
		
	void beginMqttConnection();
	void wifiCallback(void* response);
	void mqttConnected(void* response);
	void mqttDisconnected(void* response);
	void mqttDataCallback(void* response);
	void runWifiWatchDog();
public:

	MqttHelperClass();
	void init();
	void process();
	bool hasPendingData();
	void subscribe(const char * topic);
	void subscribe(const __FlashStringHelper*  topic);
	void publish(const char * topic, char * data);
	void publish(const char * topic, char * data, uint8_t qos, bool retain);
	void publish(const __FlashStringHelper* topic, char * data);
	void publish(const __FlashStringHelper* topic, char * data, uint8_t qos, bool retain);
	boolean lastWillAndTestament(const char* topic, const char* message);
	boolean lastWillAndTestament(const char* topic, const char* message, uint8_t qos, bool retain);
	void onConnect(FP<void, void*> callback);
	void onDisconnect(FP<void, void*> callback);
	void onData(FP<void, void*> callback);
};
#endif

