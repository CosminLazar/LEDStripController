// 
// 
// 

#include "MqttParameters.h"

void MqttParametersClass::configureBrigtness(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic)
{
	brightness_get = getTopic;
	brightness_set = setTopic;
	brightness_status = statusTopic;
}

void MqttParametersClass::configurePower(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic)
{
	power_get = getTopic;
	power_set = setTopic;
	power_status = statusTopic;
}

void MqttParametersClass::configureHue(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic)
{
	hue_get = getTopic;
	hue_set = setTopic;
	hue_status = statusTopic;
}

void MqttParametersClass::configureSaturation(const __FlashStringHelper * getTopic, const __FlashStringHelper * setTopic, const __FlashStringHelper * statusTopic)
{
	saturation_get = getTopic;
	saturation_set = setTopic;
	saturation_status = statusTopic;
}
