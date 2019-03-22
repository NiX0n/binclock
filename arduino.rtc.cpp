// References:
// https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library

#define DS13074_CS_PIN 11
#define DS13074_MOSI_PIN 10
#define DS13074_MISO_PIN 9
#define DS13074_CLK_PIN 8

#include <SPI.h>
#include "SparkFunDS3234RTC.h"

void setup()
{
	
}

void setupRtc()
{
	rtc.begin(DS13074_CS_PIN);
}

void loop()
{
	
}
