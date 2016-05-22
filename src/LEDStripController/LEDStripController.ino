#include "SensitiveData.h"
#include <FP.h>
#include "MqttHelper.h"
#include <MemoryFree.h>


MqttHelperClass mqtt;

void setup() {

	FP<void, const char *> s;
	s.attach(&statusRequested);
	mqtt.subscribe("st", s);

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
