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

	if (!mqtt->begin(SensitiveData::MQTT_CLIENT, SensitiveData::MQTT_USER, SensitiveData::MQTT_PASSWORD, 120, 1))
		while (1);

	mqtt->lwt("/lwt", "offline", 0, 0);

	mqtt->connectedCb.attach(this, &MqttHelperClass::mqttConnected);
	mqtt->disconnectedCb.attach(this, &MqttHelperClass::mqttDisconnected);
	mqtt->dataCb.attach(this, &MqttHelperClass::mqttDataCallback);

	esp->wifiCb.attach(this, &MqttHelperClass::wifiCallback);
	esp->wifiConnect(SensitiveData::WIFI_SSID, SensitiveData::WIFI_PASSWORD);
}

void MqttHelperClass::wifiCallback(void * response)
{
	uint32_t status;
	RESPONSE res(response);

	if (res.getArgc() == 1) {
		res.popArgs((uint8_t*)&status, 4);

		if (status == STATION_GOT_IP) {
			//WIFI connected			
			mqtt->connect(SensitiveData::MQTT_HOST, 1883);
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
	mqtt->publish("/topic/0", "hello world!");
}

void MqttHelperClass::mqttDisconnected(void* response) {
	mqttIsConnected = false;
}

void MqttHelperClass::mqttDataCallback(void* response) {
	RESPONSE res(response);
	String topic = res.popString();
	String data = res.popString();
}

void MqttHelperClass::process() {
	esp->process();
}

MqttHelperClass MqttHelper;

