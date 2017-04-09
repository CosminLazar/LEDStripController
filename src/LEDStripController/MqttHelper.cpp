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

void MqttHelperClass::subscribe(const char * topic) {
	mqtt->subscribe(topic);
}

void MqttHelperClass::subscribe(const __FlashStringHelper * topic)
{
	char buff[64];
	char * topicStr = strcpy_P(buff, (const char *)topic);

	subscribe(topicStr);
}

void MqttHelperClass::publish(const char * topic, char * data) {
	mqtt->publish(topic, data);
}

void MqttHelperClass::publish(const __FlashStringHelper * topic, char * data)
{
	char buff[64];
	char * topicStr = strcpy_P(buff, (const char *)topic);
	publish(topicStr, data);
}

boolean MqttHelperClass::lastWillAndTestament(const char * topic, const char * message)
{
	return lastWillAndTestament(topic, message, 0, 0);
}

boolean MqttHelperClass::lastWillAndTestament(const char * topic, const char * message, uint8_t qos, uint8_t retain)
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