// 
// 
// 

#include "MqttParameters.h"

void MqttParametersClass::configureVerbs(const String & get, const String & set, const String & status, const String & connected)
{
	_verb_get = get;
	_verb_set = set;
	_verb_status = status;
	_verb_connected = connected;
}

void MqttParametersClass::configureComponents(const String & power, const String & brightness, const String & hue, const String & saturation)
{
	_power = power;
	_brightness = brightness;
	_hue = hue;
	_saturation = saturation;
}

void MqttParametersClass::subscribe()
{
	subscribeTo(_verb_get, _power);
	subscribeTo(_verb_set, _power);

	subscribeTo(_verb_get, _brightness);
	subscribeTo(_verb_set, _brightness);

	subscribeTo(_verb_get, _hue);
	subscribeTo(_verb_set, _hue);

	subscribeTo(_verb_get, _saturation);
	subscribeTo(_verb_set, _saturation);
}

void MqttParametersClass::reportConnected()
{
	String connectedTopic = topLevelAddress + _verb_connected;
	uint8_t deliverAtMostOnce = 0;
	bool retainMessage = true;

	//when connected, report fully operational (2)	
	mqtt->publish(connectedTopic.c_str(), String("2").c_str(), deliverAtMostOnce, retainMessage);

	//when disconnected, report disconnected (0)	
	mqtt->lastWillAndTestament(connectedTopic.c_str(), "0", deliverAtMostOnce, retainMessage);
}

bool MqttParametersClass::isGet(const String & topic)
{
	return isVerb(topic, _verb_get);

}

bool MqttParametersClass::isSet(const String & topic)
{
	return isVerb(topic, _verb_set);
}

bool MqttParametersClass::isPower(const String & topic)
{
	return isResource(topic, _power);
}

bool MqttParametersClass::isBrightness(const String & topic)
{
	return isResource(topic, _brightness);
}

bool MqttParametersClass::isHue(const String & topic)
{
	return isResource(topic, _hue);
}

bool MqttParametersClass::isSaturation(const String & topic)
{
	return isResource(topic, _saturation);
}

void MqttParametersClass::reportPower(const String & value)
{
	reportResourceValue(_power, value);
}

void MqttParametersClass::reportBrightness(const String & value)
{
	reportResourceValue(_brightness, value);
}

void MqttParametersClass::reportHue(const String & value)
{
	reportResourceValue(_hue, value);
}

void MqttParametersClass::reportSaturation(const String & value)
{
	reportResourceValue(_saturation, value);
}

void MqttParametersClass::subscribeTo(const String & verb, const String & resource)
{
	String fullTopic = topLevelAddress + verb + resource;

	mqtt->subscribe(fullTopic.c_str());
}

bool MqttParametersClass::isVerb(const String & topic, const String & verb)
{
	bool isVerb = topic.indexOf(verb) > 0;
	return isVerb;
}

bool MqttParametersClass::isResource(const String & topic, const String & resource)
{
	bool isResource = topic.indexOf(resource) > 0;
	return isResource;
}

void MqttParametersClass::reportResourceValue(const String & resource, const String & value)
{
	String fullTopic = topLevelAddress + _verb_status + resource;

	mqtt->publish(fullTopic.c_str(), value.c_str(), 0, true);
}
