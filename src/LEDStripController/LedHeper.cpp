#include "LedHeper.h"

LedHeperClass::LedHeperClass(uint8_t noOfLeds, uint8_t pin)
{
	this->strip = new Adafruit_NeoPixel(noOfLeds, pin, NEO_GRB + NEO_KHZ800);
}

void LedHeperClass::init()
{
	strip->begin();
	strip->show();//all off
}

void LedHeperClass::allWhite()
{
	for (uint16_t i = 0; i < 60; i++)
	{
		strip->setPixelColor(i, strip->Color(255, 255, 255));
	}

	strip->show();
}


