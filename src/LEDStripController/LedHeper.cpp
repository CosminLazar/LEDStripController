#include "LedHeper.h"

LedHeperClass::LedHeperClass(uint8_t noOfLeds, uint8_t pin, const MqttParametersClass * mqttParameters, uint8_t touchSensorPin)
{
	this->mqttParameters = mqttParameters;
	this->strip = new Adafruit_NeoPixel(noOfLeds, pin, NEO_GRB + NEO_KHZ800);
	this->touchSensorReader = new TouchSensorReaderClass(touchSensorPin);

	FP<void, void *> connectedCb;
	connectedCb.attach(this, &onMqttConnected);
	this->mqttParameters->mqtt->onConnect(connectedCb);

	FP<void, void *> dataReceivedCb;
	dataReceivedCb.attach(this, &onMqttMessage);
	this->mqttParameters->mqtt->onData(dataReceivedCb);
}

void LedHeperClass::init()
{
	touchSensorReader->init();
	strip->begin();
	strip->show();//all off
}

void LedHeperClass::process()
{
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	
	mqtt->process();
	
	processTouchSensor();

	if (!mqtt->hasPendingData()) {
		processHardwareUpdates();
	}

	if (!mqtt->hasPendingData()) {
		processPendingStatusReports();
	}
}

void LedHeperClass::onMqttConnected(void * data)
{
	this->mqttParameters->subscribe();
	this->mqttParameters->reportConnected();

	requestPowerStateReport();
	requestBrightnessStateReport();
	requestHueStateReport();
	requestSaturationStateReport();
}

void LedHeperClass::onMqttMessage(void * message) {
	const MqttHelperClass * mqtt = mqttParameters->mqtt;
	RESPONSE res(message);
	String topic = res.popString();
		
	if (!this->mqttParameters->isTarget(topic)) {
		//will not handle message
		return;
	}	

	if (this->mqttParameters->isSet(topic)) {
		String data = res.popString();

		if (this->mqttParameters->isBrightness(topic)) {
			int brightness = atoi(data.c_str());
			//todo: boundary check
			setBrightness(brightness);
			return;
		}
		
		if (this->mqttParameters->isHue(topic)) {
			float hue = data.toFloat();
			setHue(hue);
			return;
		}

		if (this->mqttParameters->isSaturation(topic)) {
			float saturation = data.toFloat();
			setSaturation(saturation);
			return;
		}

		if (this->mqttParameters->isPower(topic)) {
			bool isOn = atoi(data.c_str()) != 0;

			if (isOn) {
				powerOn();
			}
			else {
				powerOff();
			}
			return;
		}
	}
	else {
		if (this->mqttParameters->isGet(topic)) {

			if (this->mqttParameters->isBrightness(topic)) {
				requestBrightnessStateReport();
				return;
			}

			if (this->mqttParameters->isHue(topic)) {
				requestHueStateReport();
				return;
			}

			if (this->mqttParameters->isSaturation(topic)) {
				requestSaturationStateReport();
				return;
			}

			if (this->mqttParameters->isPower(topic)) {
				requestPowerStateReport();
				return;
			}
		}
	}	
}



bool LedHeperClass::isOn()
{
	return _isOn;
}

void LedHeperClass::powerOn()
{
	if (_isOn)
		return;

	_isOn = true;

	requestUpdateHardwareState();

	requestPowerStateReport();
}

void LedHeperClass::powerOff()
{
	if (!_isOn)
		return;

	_isOn = false;

	requestUpdateHardwareState();

	requestPowerStateReport();
}

void LedHeperClass::setBrightness(uint8_t brigtness)
{
	_brightness = brigtness;

	if (brigtness == 0)
	{
		powerOff();
	}
	else
	{
		powerOn();
	}

	requestUpdateHardwareState();

	requestBrightnessStateReport();	
}

void LedHeperClass::setHue(float hue)
{
	_hue = hue;

	requestUpdateHardwareState();

	requestHueStateReport();
}

void LedHeperClass::setSaturation(float saturation)
{
	_saturation = saturation;

	requestUpdateHardwareState();	

	requestSaturationStateReport();
}

void LedHeperClass::requestPowerStateReport()
{
	pendingStatusReport[PENDING_POWER_REPORT] = 1;
	_statusReportRequestTimestamp = millis();
}

void LedHeperClass::requestBrightnessStateReport()
{
	pendingStatusReport[PENDING_BRIGHTNESS_REPORT] = 1;
	_statusReportRequestTimestamp = millis();
}

void LedHeperClass::requestHueStateReport()
{
	pendingStatusReport[PENDING_HUE_REPORT] = 1;
	_statusReportRequestTimestamp = millis();
}

void LedHeperClass::requestSaturationStateReport()
{
	pendingStatusReport[PENDING_SATURATION_REPORT] = 1;
	_statusReportRequestTimestamp = millis();
}

void LedHeperClass::requestUpdateHardwareState()
{
	_hardwareUpdateRequestTimestamp = millis();
	_hardwareUpdateRequested = true;
}

void LedHeperClass::processHardwareUpdates()
{
	//called in a loop
	if (!_hardwareUpdateRequested)
		return;

	if (millis() - _hardwareUpdateRequestTimestamp < HARDWARE_UPDATE_DEBOUNCE)
		return;	

	updateHardwareState();

	_hardwareUpdateRequested = false;
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

void LedHeperClass::processTouchSensor()
{
	TouchResult result = touchSensorReader->process();

	switch (result)
	{
	case TouchResult::toggle: {

		if (_isOn)
		{
			powerOff();
		}
		else {
			powerOn();
			if (_brightness == 0)
				setBrightness(100);
		}

		break;
	}
	case TouchResult::long_press: {
		if (!_isOn) {
			powerOn();
		}

		uint8_t step = 25;
		uint8_t newValue = ((_brightness / step) + 1) * step;
		setBrightness(newValue);

		//cannot work with the debounced hardware update here
		//need to show "real-time" updates
		updateHardwareState();
		break;
	}

	default:
		break;
	}
}

void LedHeperClass::processPendingStatusReports()
{
	//called in a loop

	if ((millis() - _statusReportRequestTimestamp < PENDING_REPORT_DEBOUNCE))
		return;

	if (pendingStatusReport[PENDING_POWER_REPORT])
	{
		mqttParameters->reportPower(String(_isOn ? 1 : 0));

		pendingStatusReport[PENDING_POWER_REPORT] = 0;		
	}

	if (pendingStatusReport[PENDING_BRIGHTNESS_REPORT])
	{
		mqttParameters->reportBrightness(String(_brightness));

		pendingStatusReport[PENDING_BRIGHTNESS_REPORT] = 0;		
	}

	if (pendingStatusReport[PENDING_HUE_REPORT])
	{
		mqttParameters->reportHue(String(_hue));

		pendingStatusReport[PENDING_HUE_REPORT] = 0;			
	}

	if (pendingStatusReport[PENDING_SATURATION_REPORT])
	{		
		mqttParameters->reportSaturation(String(_saturation));

		pendingStatusReport[PENDING_SATURATION_REPORT] = 0;		
	}
}
