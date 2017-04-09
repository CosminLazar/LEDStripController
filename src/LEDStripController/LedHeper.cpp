#include "LedHeper.h"

LedHeperClass::LedHeperClass(uint8_t noOfLeds, uint8_t pin, const MqttParametersClass * mqttParameters)
{
	this->mqttParameters = mqttParameters;
	this->strip = new Adafruit_NeoPixel(noOfLeds, pin, NEO_GRB + NEO_KHZ800);


	FP<void, void *> connectedCb;
	connectedCb.attach(this, &onMqttConnected);
	this->mqttParameters->mqtt->onConnect(connectedCb);

	FP<void, void *> dataReceivedCb;
	dataReceivedCb.attach(this, &onMqttMessage);
	this->mqttParameters->mqtt->onData(dataReceivedCb);
}

void LedHeperClass::onMqttConnected(void * data)
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;

	mqtt->subscribe(mqttParameters->power_get);
	mqtt->subscribe(mqttParameters->power_set);
	mqtt->subscribe(mqttParameters->brightness_get);
	mqtt->subscribe(mqttParameters->brightness_set);
	mqtt->subscribe(mqttParameters->hue_get);
	mqtt->subscribe(mqttParameters->hue_set);
	mqtt->subscribe(mqttParameters->saturation_get);
	mqtt->subscribe(mqttParameters->saturation_set);

	String connectedTopic = mqttParameters->topLevelAddress;// +F("/connected");
	connectedTopic.concat(F("/connected"));

	//when connected, report fully operational (2)	
	mqtt->publish(connectedTopic.c_str(), String("2").c_str());

	//when disconnected, report disconnected (0)
	uint8_t deliverAtLeastOnce = 1;
	uint8_t retainMessage = 1;
	mqtt->lastWillAndTestament(connectedTopic.c_str(), "0", deliverAtLeastOnce, retainMessage);

	reportPowerState();
	reportBrightnessState();	
	reportHueState();
	reportSaturationState();
}

void LedHeperClass::onMqttMessage(void * message) {
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	RESPONSE res(message);
	String topic = res.popString();


	if (topic.indexOf(mqttParameters->topLevelAddress) != 0) {
		//will not handle message
		return;
	}

	String data = res.popString();

	if (topic.indexOf(mqttParameters->brightness_set) == 0) {
		
		int brightness = atoi(data.c_str());
		//todo: boundary check
		setBrightness(brightness);

		return;
	}

	if (topic.indexOf(mqttParameters->hue_set) == 0) {
		float hue = data.toFloat();
		setHue(hue);
		return;
	}

	if (topic.indexOf(mqttParameters->saturation_set) == 0) {
		float saturation = data.toFloat();
		setSaturation(saturation);
		return;
	}

	if (topic.indexOf(mqttParameters->power_set) == 0) {
		bool isOn = atoi(data.c_str()) != 0;

		if (isOn) {
			powerOn();
		}
		else {
			powerOff();
		}
		return;
	}

	if (topic.indexOf(mqttParameters->brightness_get) == 0) {
		reportBrightnessState();
		return;
	}

	if (topic.indexOf(mqttParameters->hue_get) == 0) {
		reportHueState();
		return;
	}

	if (topic.indexOf(mqttParameters->saturation_get) == 0) {
		reportSaturationState();
		return;
	}

	if (topic.indexOf(mqttParameters->power_get) == 0) {
		reportPowerState();
		return;
	}	
}

void LedHeperClass::init()
{
	strip->begin();
	strip->show();//all off
}

void LedHeperClass::powerOn()
{
	_isOn = true;

	updateHardwareState();

	reportPowerState();
}

void LedHeperClass::powerOff()
{
	_isOn = false;
	
	updateHardwareState();

	reportPowerState();
}

void LedHeperClass::setBrightness(uint8_t brigtness)
{
	_brightness = brigtness;

	updateHardwareState();
	
	reportBrightnessState();
}

void LedHeperClass::setHue(float hue)
{
	_hue = hue;

	updateHardwareState();

	reportHueState();
}

void LedHeperClass::setSaturation(float saturation)
{
	_saturation = saturation;

	updateHardwareState();

	reportSaturationState();
}

void LedHeperClass::reportPowerState()
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	mqtt->publish(mqttParameters->power_status, String(_isOn ? 1 : 0).c_str());
}

void LedHeperClass::reportBrightnessState()
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	mqtt->publish(mqttParameters->brightness_status, String(_brightness).c_str());	
}

void LedHeperClass::reportHueState()
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	mqtt->publish(mqttParameters->hue_status, String(_hue).c_str());
}

void LedHeperClass::reportSaturationState()
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	mqtt->publish(mqttParameters->saturation_status, String(_saturation).c_str());
}

void LedHeperClass::updateHardwareState()
{
	if (_isOn) {
		strip->setBrightness(_brightness);
		uint8_t r, g, b;
		HSV_to_RGB(_hue, _saturation, _brightness, &r, &g, &b);

		for (uint16_t i = 0; i < strip->numPixels(); i++)
		{
			strip->setPixelColor(i, r, g, b);
		}
	}
	else {
		strip->clear();
	}

	strip->show();
}

void LedHeperClass::HSV_to_RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
	//HSV_to_RGB function is elegantly copied from https://gist.github.com/hdznrrd/656996
	
	int i;
	float f, p, q, t;

	h = max(0.0, min(360.0, h));
	s = max(0.0, min(100.0, s));
	v = max(0.0, min(100.0, v));

	s /= 100;
	v /= 100;

	if (s == 0) {
		// Achromatic (grey)
		*r = *g = *b = round(v * 255);
		return;
	}

	h /= 60; // sector 0 to 5
	i = floor(h);
	f = h - i; // factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		*r = round(255 * v);
		*g = round(255 * t);
		*b = round(255 * p);
		break;
	case 1:
		*r = round(255 * q);
		*g = round(255 * v);
		*b = round(255 * p);
		break;
	case 2:
		*r = round(255 * p);
		*g = round(255 * v);
		*b = round(255 * t);
		break;
	case 3:
		*r = round(255 * p);
		*g = round(255 * q);
		*b = round(255 * v);
		break;
	case 4:
		*r = round(255 * t);
		*g = round(255 * p);
		*b = round(255 * v);
		break;
	default: // case 5:
		*r = round(255 * v);
		*g = round(255 * p);
		*b = round(255 * q);
	}
}
