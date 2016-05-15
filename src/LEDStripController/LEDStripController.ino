#include <HashMap.h>
#include <LinkedList.h>
#include "SensitiveData.h"
#include "SensitiveData.h"
#include <ringbuf.h>
#include <rest.h>
#include <mqtt.h>
#include <FP.h>
#include <espduino.h>
#include <crc16.h>
#include "MqttHelper.h"
#include <MemoryFree.h>


MqttHelperClass mqtt;

void setup() {

	FP<void, void*> s;
	s.attach(&statusRequested);
	mqtt.subscribe("statusreq", s);

	mqtt.init();
}

void loop() {
	freeMemory();
	mqtt.process();
}

void statusRequested(void* data) {
	char buff[10];
	char * freeMem = itoa(freeMemory(), buff, 10);
	mqtt.publish("status", freeMem);
}
