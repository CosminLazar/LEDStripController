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
LedHeperClass led(60, 5);

void setup() {

	FP<void, const char *> s;
	s.attach(&statusRequested);
	mqtt.subscribe("st", s);

	FP<void, const char *> sl;
	sl.attach(&setLights);
	mqtt.subscribe("sl", sl);

	led.init();
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

void setLights(const char * data) {
	LedStatusClass st = LedStatusConverterClass::FromStr(data);
	led.set(st);
}