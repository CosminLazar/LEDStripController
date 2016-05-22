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

void LedHeperClass::set(LedStatusClass status)
{
	strip->clear();
	strip->setBrightness(status.get_Brigthness());

	if (status.isOn()) {
		for (uint16_t i = 0; i < strip->numPixels(); i++)
		{
			strip->setPixelColor(i, status.get_R(), status.get_G(), status.get_B());
		}
	}
	
	strip->show();
}


