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
	 const __FlashStringHelper * topLevelAddress;	 

	 const __FlashStringHelper * power_get;
	 const __FlashStringHelper * power_set;
	 const __FlashStringHelper * power_status;

	 const __FlashStringHelper * brightness_get;
	 const __FlashStringHelper * brightness_set;
	 const __FlashStringHelper * brightness_status;

	 const __FlashStringHelper * hue_get;
	 const __FlashStringHelper * hue_set;
	 const __FlashStringHelper * hue_status;

	 const __FlashStringHelper * saturation_get;
	 const __FlashStringHelper * saturation_set;
	 const __FlashStringHelper * saturation_status;
 public:
	MqttParametersClass(const MqttHelperClass * mqttBridge, const __FlashStringHelper * topLevelAddress) :mqtt(mqttBridge), topLevelAddress(topLevelAddress) {};
	
	void configureBrigtness(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic);
	void configurePower(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic);
	void configureHue(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic);
	void configureSaturation(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic);
};

#endif

