//All transient strings should be moved into PROGMEM as there is no reason for them to live in SRAM if only used once.
//This makes the code a bit uglier due to the buffer being passed around, however it is better this way because :
//	1. the byte eating strings stay on FLASH and are only copied to SRAM when they are on their way to ESPP8266
//	2. since the buffer is declared by the calling method we get free deallocation and more important no memory fragmentation.

#include "SensitiveData.h"

void SensitiveData::Read_WIFI_SSID(char * buffer)
{
	strcpy_P(buffer, PSTR("ssid"));
}

void SensitiveData::Read_WIFI_PASSWORD(char * buffer) {
	strcpy_P(buffer, PSTR("wifi password"));
}

void SensitiveData::Read_MQTT_HOST(char * buffer) {
	strcpy_P(buffer, PSTR("mqtt host"));
}

void SensitiveData::Read_MQTT_CLIENT(char * buffer)
{
	strcpy_P(buffer, PSTR("mqtt client name"));
}

void SensitiveData::Read_MQTT_USER(char * buffer)
{
	strcpy_P(buffer, PSTR("mqtt user"));
}

void SensitiveData::Read_MQTT_PASSWORD(char * buffer)
{
	strcpy_P(buffer, PSTR("mqtt password"));
}
