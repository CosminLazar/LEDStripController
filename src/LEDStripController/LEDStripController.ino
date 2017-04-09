#include "MqttParameters.h"
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
#include "WString.h"
#include "MqttParameters.h"

MqttHelperClass mqtt;

const MqttParametersClass * p1;
const MqttParametersClass * p2;
const LedHeperClass * led1;
const LedHeperClass * led2;

void setup() {

	p1 = new MqttParametersClass(&mqtt, F("bedroom/light/1"));
	p1->configurePower(F("bedroom/light/1/get"), F("bedroom/light/1/set"), F("bedroom/light/1/status"));
	p1->configureBrigtness(F("bedroom/light/1/get/brightness"), F("bedroom/light/1/set/brightness"), F("bedroom/light/1/status/brightness"));
	p1->configureHue(F("bedroom/light/1/get/hue"), F("bedroom/light/1/set/hue"), F("bedroom/light/1/status/hue"));
	p1->configureSaturation(F("bedroom/light/1/get/saturation"), F("bedroom/light/1/set/saturation"), F("bedroom/light/1/status/saturation"));


	p2 = new MqttParametersClass(&mqtt, F("bedroom/light/2"));
	p2->configurePower(F("bedroom/light/2/get"), F("bedroom/light/2/set"), F("bedroom/light/2/status"));
	p2->configureBrigtness(F("bedroom/light/2/get/brightness"), F("bedroom/light/2/set/brightness"), F("bedroom/light/2/status/brightness"));
	p2->configureHue(F("bedroom/light/2/get/hue"), F("bedroom/light/2/set/hue"), F("bedroom/light/2/status/hue"));
	p2->configureSaturation(F("bedroom/light/2/get/saturation"), F("bedroom/light/2/set/saturation"), F("bedroom/light/2/status/saturation"));

	led1 = new LedHeperClass(60, 5, p1);
	led2 = new LedHeperClass(60, 6, p2);


	FP<void, void *> connectedCb;
	connectedCb.attach(&connected);
	mqtt.onConnect(connectedCb);

	FP<void, void *> dataCb;
	dataCb.attach(&data);
	mqtt.onData(dataCb);

	led1->init();
	led2->init();

	mqtt.init();
}

void loop() {
	mqtt.process();
}

void connected(void * data) {

}

void data(void * response) {

}

void statusRequested(const char * data) {
	char buff[10];
	char * freeMem = itoa(freeMemory(), buff, 10);
	mqtt.publish(F("str"), freeMem);
}
