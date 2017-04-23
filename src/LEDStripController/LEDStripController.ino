#include "MqttParameters.h"
#include "LedHeper.h"
#include <FP.h>
#include "MqttHelper.h"
#include <MemoryFree.h>
#include "WString.h"
#include "MqttParameters.h"

MqttHelperClass mqtt;

const MqttParametersClass * p1;
const MqttParametersClass * p2;
const LedHeperClass * led1;
const LedHeperClass * led2;

const String get("/get");
const String set("/set");
const String status("/status");
const String connected("/connected");

const String power("/power");
const String brightness("/brightness");
const String hue("/hue");
const String saturation("/saturation"); 
const String animation("/anm");
 
void setup() {	
	p1 = new MqttParametersClass(&mqtt, String("b/l1"));
	p1->configureVerbs(get, set, status, connected);
	p1->configureComponents(power, brightness, hue, saturation, animation);
	

	p2 = new MqttParametersClass(&mqtt, String("b/l2"));
	p2->configureVerbs(get, set, status, connected);
	p2->configureComponents(power, brightness, hue, saturation, animation);

	led1 = new LedHeperClass(60, 5, p1, 8);
	led2 = new LedHeperClass(60, 6, p2, 9);


	FP<void, void *> connectedCb;
	connectedCb.attach(&onconnected);
	mqtt.onConnect(connectedCb);

	FP<void, void *> dataCb;
	dataCb.attach(&data);
	mqtt.onData(dataCb);

	led1->init();
	led2->init();

	mqtt.init();	
}

void loop() {
	led1->process();
	led2->process();
}

void onconnected(void * data) {

}

void data(void * response) {
	//statusRequested(NULL);
}

void statusRequested(const char * data) {
	char buff[10];
	char * freeMem = itoa(freeMemory(), buff, 10);
	mqtt.publish(F("str"), freeMem);
}
