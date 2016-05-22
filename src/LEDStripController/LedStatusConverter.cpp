#include "LedStatusConverter.h"

LedStatusClass LedStatusConverterClass::FromStr(const char * str)
{
	const uint8_t AcceptableParams = 5;

	uint8_t ledParams[AcceptableParams] = {};
	ParseStr(str, ledParams, AcceptableParams);

	return LedStatusClass(ledParams[0], ledParams[1], ledParams[2], ledParams[3], ledParams[4]);
}

void LedStatusConverterClass::ParseStr(const char * str, uint8_t * valueBuff, uint8_t len)
{
	uint8_t dataLen = strlen(str);
	uint8_t indexer = 0;
	uint8_t parameterIndexer = 0;
	char buff[10];

	for (uint8_t i = 0; i < dataLen && parameterIndexer < len; i++)
	{
		buff[indexer] = str[i];

		if (str[i] == ';') {
			buff[indexer] = '\0';
			valueBuff[parameterIndexer] = atoi(buff);
			parameterIndexer++;
			indexer = 0;
		}
		else {
			indexer++;
		}
	}

	if (indexer != 0 && parameterIndexer < len) {
		//str did not finish in a ';'
		buff[indexer + 1] = '\0';
		valueBuff[parameterIndexer] = atoi(buff);
	}
}
