// LedStatusConverter.h

#ifndef _LEDSTATUSCONVERTER_h
#define _LEDSTATUSCONVERTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LedStatus.h"

class LedStatusConverterClass
{
 public:
	 static LedStatusClass FromStr(const char * str);
	 static void ToStr(LedStatusClass data, char * buff);
private:
	static void ParseStr(const char * str, uint8_t * valueBuff, uint8_t len);
};

extern LedStatusConverterClass LedStatusConverter;

#endif

