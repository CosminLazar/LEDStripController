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

void MqttHelperClass::beginMqttConnection()
{
	char mqttHost[10];
	SensitiveData::Read_MQTT_HOST(mqttHost);
	mqtt->connect(mqttHost, 1883);
}

void MqttHelperClass::wifiCallback(void * response)
{
	uint32_t status;
	RESPONSE res(response);

	if (res.getArgc() == 1) {
		res.popArgs((uint8_t*)&status, 4);

		if (status == STATION_GOT_IP) {
			//WIFI connected						
			beginMqttConnection();
		}
		else {
			mqtt->disconnect();
		}
	}
}

void MqttHelperClass::mqttConnected(void* response) {
	for (uint8_t i = 0; i < connectCb.size(); i++)
	{
		connectCb.get(i)(response);
	}
}

void MqttHelperClass::mqttDisconnected(void* response) {
	for (uint8_t i = 0; i < disconnectCb.size(); i++)
	{
		disconnectCb.get(i)(response);
	}

	//only the mqtt connection needs to be re-established
	//the sepparate esp chip handles wifi reconnection auto-magically
	beginMqttConnection();
}

void MqttHelperClass::mqttDataCallback(void* response) {
	for (uint8_t i = 0; i < dataCb.size(); i++)
	{
		dataCb.get(i)(response);
	}
}

void MqttHelperClass::process() {
	esp->process();
}

bool MqttHelperClass::hasPendingData()
{
	return (Serial.available() > 0);
}

void MqttHelperClass::subscribe(const char * topic) {
	mqtt->subscribe(topic);
}

void MqttHelperClass::subscribe(const __FlashStringHelper * topic)
{
	char buff[32];
	char * topicStr = strcpy_P(buff, (const char *)topic);

	subscribe(topicStr);
}

void MqttHelperClass::publish(const char * topic, char * data) {
	publish(topic, data, 0, false);
}

void MqttHelperClass::publish(const char * topic, char * data, uint8_t qos, bool retain)
{
	mqtt->publish(topic, data, qos, retain);
}

void MqttHelperClass::publish(const __FlashStringHelper * topic, char * data)
{
	publish(topic, data, 0, false);
}

void MqttHelperClass::publish(const __FlashStringHelper * topic, char * data, uint8_t qos, bool retain)
{
	char buff[32];
	char * topicStr = strcpy_P(buff, (const char *)topic);
	publish(topicStr, data, qos, retain);
}

boolean MqttHelperClass::lastWillAndTestament(const char * topic, const char * message)
{
	return lastWillAndTestament(topic, message, 0, 0);
}

boolean MqttHelperClass::lastWillAndTestament(const char * topic, const char * message, uint8_t qos, bool retain)
{
	return mqtt->lwt(topic, message, qos, retain);
}

void MqttHelperClass::onData(FP<void, void*> callback)
{
	this->dataCb.add(callback);
}

void MqttHelperClass::onConnect(FP<void, void*> callback)
{
	this->connectCb.add(callback);
}

void MqttHelperClass::onDisconnect(FP<void, void*> callback)
{
	this->disconnectCb.add(callback);
}