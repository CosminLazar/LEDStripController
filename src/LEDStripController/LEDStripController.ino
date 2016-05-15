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
	mqtt.init();
}

void loop() {
	freeMemory();
	mqtt.process();
}
