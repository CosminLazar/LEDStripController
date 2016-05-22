#include "LedStatus.h"

bool LedStatusClass::isOn()
{
	return _isOn;
}

uint8_t LedStatusClass::get_R()
{
	return _r;
}

uint8_t LedStatusClass::get_G()
{
	return _g;
}

uint8_t LedStatusClass::get_B()
{
	return _b;
}

uint8_t LedStatusClass::get_Brigthness()
{
	return _brigtness;
}
