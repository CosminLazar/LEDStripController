#include "MqttHelper.h"

MqttHelperClass::MqttHelperClass()
{
	this->esp = new ESP(&Serial, 4);
	this->mqtt = new MQTT(this->esp);
}

void MqttHelperClass::init()
{
	Serial.begin(19200);
	esp->enable();
	delay(500);

	esp->reset();
	delay(500);

	while (!esp->ready());

	char mqttClient[10];
	char mqttUser[10];
	char mqttPassword[10];
	SensitiveData::Read_MQTT_CLIENT(mqttClient);
	SensitiveData::Read_MQTT_USER(mqttUser);
	SensitiveData::Read_MQTT_PASSWORD(mqttPassword);

	if (!mqtt->begin(mqttClient, mqttUser, mqttPassword, 120, 1))
		while (1);

	mqtt->lwt("/lwt", "offline", 0, 0);

	mqtt->connectedCb.attach(this, &MqttHelperClass::mqttConnected);
	mqtt->disconnectedCb.attach(this, &MqttHelperClass::mqttDisconnected);
	mqtt->dataCb.attach(this, &MqttHelperClass::mqttDataCallback);

	esp->wifiCb.attach(this, &MqttHelperClass::wifiCallback);

	char ssid[5];
	SensitiveData::Read_WIFI_SSID(ssid);

	char password[30];
	SensitiveData::Read_WIFI_PASSWORD(password);

	esp->wifiConnect(ssid, password);
}

void MqttHelperClass::wifiCallback(void * response)
{
	uint32_t status;
	RESPONSE res(response);

	if (res.getArgc() == 1) {
		res.popArgs((uint8_t*)&status, 4);

		if (status == STATION_GOT_IP) {
			//WIFI connected			
			char mqttHost[10];
			SensitiveData::Read_MQTT_HOST(mqttHost);
			mqtt->connect(mqttHost, 1883);
			wifiConnected = true;
		}
		else {
			wifiConnected = false;
			mqtt->disconnect();
		}
	}
}

void MqttHelperClass::mqttConnected(void* response) {
	mqttIsConnected = true;
	publish(F("/topic/0"), "hello world!");

	for (size_t i = 0; i < subscriptionList.size(); i++)
	{
		const char * topic = subscriptionList.keyAt(i);
		mqtt->subscribe(topic);
	}
}

void MqttHelperClass::mqttDisconnected(void* response) {
	mqttIsConnected = false;
}

void MqttHelperClass::mqttDataCallback(void* response) {
	RESPONSE res(response);
	String topic = res.popString();
	String data = res.popString();
	const char * topicCStr = topic.c_str();
	const char * dataCStr = data.c_str();

	if (subscriptionList.contains(topicCStr)) {
		for (size_t i = 0; i < subscriptionList[topicCStr]->size(); i++)
		{
			subscriptionList[topicCStr]->get(i)(dataCStr);
		}
	}
}

bool MqttHelperClass::topicComparator(const char * a, const char * b)
{
	return strcmp(a, b) == 0;
}

void MqttHelperClass::process() {
	esp->process();
}

void MqttHelperClass::subscribe(const char * topic, FP<void, const char *> callback) {
	if (!subscriptionList.contains(topic)) {
		subscriptionList[topic] = new LinkedList<FP<void, const char *>>();
	}

	subscriptionList[topic]->add(callback);
}

void MqttHelperClass::publish(const char * topic, char * data) {
	mqtt->publish(topic, data);
}

void MqttHelperClass::publish(const __FlashStringHelper * topic, char * data)
{
	char buff[32];
	char * topicStr = strcpy_P(buff, (const char *)topic);
	publish(topicStr, data);
}


