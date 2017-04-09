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

class MqttHelperClass
{
private:
	ESP *esp;
	MQTT *mqtt;	
	LinkedList<FP<void, void*>> connectCb = LinkedList<FP<void, void*>>();
	LinkedList<FP<void, void*>> disconnectCb = LinkedList<FP<void, void*>>();
	LinkedList<FP<void, void*>> dataCb = LinkedList<FP<void, void*>>();

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
	void subscribe(const char * topic);
	void subscribe(const __FlashStringHelper*  topic);
	void publish(const char * topic, char * data);
	void publish(const __FlashStringHelper* topic, char * data);
	boolean lastWillAndTestament(const char* topic, const char* message);
	boolean lastWillAndTestament(const char* topic, const char* message, uint8_t qos, uint8_t retain);
	void onConnect(FP<void, void*> callback);
	void onDisconnect(FP<void, void*> callback);
	void onData(FP<void, void*> callback);
};

extern MqttHelperClass MqttHelper;

#endif

