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

#define MAX_SUBSCRIPTIONS 5

class MqttHelperClass
{
private:
	ESP *esp;
	MQTT *mqtt;
	HashMap<const char *, LinkedList<FP<void, void*>>*, MAX_SUBSCRIPTIONS> subscriptionList = HashMap<const char*, LinkedList<FP<void, void*>>*, MAX_SUBSCRIPTIONS>(&topicComparator);

	void wifiCallback(void* response);
	void mqttConnected(void* response);
	void mqttDisconnected(void* response);
	void mqttDataCallback(void* response);
	static bool topicComparator(const char * a, const char * b);
public:
	bool wifiConnected;
	bool mqttIsConnected;
	MqttHelperClass();
	void init();
	void process();
	void subscribe(const char * topic, FP<void, void*> callback);
	void publish(const char * topic, char * data);
	void publish(const __FlashStringHelper* topic, char * data);
};

extern MqttHelperClass MqttHelper;

#endif

