// MqttParameters.h

#ifndef _MQTTPARAMETERS_h
#define _MQTTPARAMETERS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "MqttHelper.h"

class MqttParametersClass
{
public:
	const MqttHelperClass * mqtt;	

private:
	const String _topLevelAddress;

	String _verb_get;
	String _verb_set;
	String _verb_status;
	String _verb_connected;

	String _power;
	String _brightness;
	String _hue;
	String _saturation;
	String _animation;

public:
	MqttParametersClass(const MqttHelperClass * mqttBridge, const String & topLevelAddress)
		:mqtt(mqttBridge), _topLevelAddress(topLevelAddress) {};

	void configureVerbs(const String & get, const String & set, const String & status, const String & connected);
	void configureComponents(const String & power, const String & brightness, const String & hue, const String & saturation, const String & animation);
	void subscribe();
	void reportConnected();

	bool isTarget(const String & topic);
	bool isGet(const String & topic);
	bool isSet(const String & topic);

	bool isPower(const String & topic);
	bool isBrightness(const String & topic);
	bool isHue(const String & topic);
	bool isSaturation(const String & topic);
	bool isAnimation(const String & topic);

	void reportPower(const String & value);
	void reportBrightness(const String & value);
	void reportHue(const String & value);
	void reportSaturation(const String & value);
private:
	void subscribeTo(const String & verb, const String & resource);
	bool isVerb(const String & topic, const String & verb);
	bool isResource(const String & topic, const String & resource);
	void reportResourceValue(const String & resource, const String & value);
};

#endif

