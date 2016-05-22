#include "LedStatusConverter.h"
#include "LedStatus.h"
#include <Adafruit_NeoPixel.h>
#include "LedHeper.h"
#include "SensitiveData.h"
#include <FP.h>
#include "MqttHelper.h"
#include <MemoryFree.h>
#include "LedStatus.h"
#include "LedStatusConverter.h"

MqttHelperClass mqtt;
LedHeperClass led1(60, 5);
LedHeperClass led2(60, 6);

void setup() {

	FP<void, const char *> s;
	s.attach(&statusRequested);
	mqtt.subscribe("st", s);

	FP<void, const char *> sl1;
	sl1.attach(&setLights1);
	mqtt.subscribe("sl1", sl1);

	FP<void, const char *> sl2;
	sl2.attach(&setLights2);
	mqtt.subscribe("sl2", sl2);

	FP<void, const char *> stl1;
	stl1.attach(&statusLights1);
	mqtt.subscribe("stl1", stl1);

	FP<void, const char *> stl2;
	stl2.attach(&statusLights2);
	mqtt.subscribe("stl2", stl2);

	led1.init();
	led2.init();
	mqtt.init();
}

void loop() {
	freeMemory();
	mqtt.process();
}

void statusRequested(const char * data) {
	char buff[10];
	char * freeMem = itoa(freeMemory(), buff, 10);
	mqtt.publish(F("str"), freeMem);
}

void setLights1(const char * data) {
	LedStatusClass st = LedStatusConverterClass::FromStr(data);
	led1.set(st);
}

void setLights2(const char * data) {
	LedStatusClass st = LedStatusConverterClass::FromStr(data);
	led2.set(st);
}

void statusLights1(const char * data) {
	LedStatusClass emptyResponse(1, 2, 3, 4, 5);
	char statusStr[32];
	LedStatusConverterClass::ToStr(emptyResponse, statusStr);

	mqtt.publish("stl1r", statusStr);
}

void statusLights2(const char * data) {
	LedStatusClass emptyResponse(5, 4, 3, 2, 1);
	char statusStr[32];
	LedStatusConverterClass::ToStr(emptyResponse, statusStr);

	mqtt.publish("stl2r", statusStr);
}