#define N_LEDS 9
#define LEDS_DATA_PIN 6
#define LEDS_CLOCK_PIN 7
#define LED_BRIGHTNESS	64
#define LED_REFRESH_RATE 1024

#define SERIAL_BAUD 115200

#include <SPI.h>
#include "SparkFunDS3234RTC.h"
#include "FastLED.h"

CRGB leds[N_LEDS];

int ledMap[][3] = {
	// hours
	{6,7,8},
	// min
	{5,4,3},
	// sec
	{0,1,2}
};

// colors described as RGB
unsigned long colorMap[4] = {
	// 00 = BLACK
	0x000000,
	// 01 = RED
	0xFF0000,
	// 10 = GREEN
	0x00FF00,
	// 11 = BLUE
	0x0000FF
};

unsigned long millisOffset = 0;
unsigned long millisFactors[4] = {
	1000L * 60L * 60L,
	1000L * 60L,
	1000L,
	1L
};


unsigned long btParts[4] = {0,0,0,0};
unsigned long btFactors[4] = {
	1000L * 60L * 60L,
	(1000L * 60L * 60L) / 64L,
	round(((1000L * 60L * 60L) / 64L) / 64L),
	1L
};


void setup()
{
	setupSerial();
	setupRtc();
	setupLeds();
	setupAlarm();
}

void setupSerial()
{
	Serial.begin(SERIAL_BAUD);
}

void setupRtc()
{
	rtc.begin(PIN_SPI_SS);
	rtc.set24Hour();
	updateMillisOffset();
}

void setupAlarm()
{
	if (rtc.alarm1())
	{
		Serial.println("In setupAlarm().  Alarm was set ");
	}
	
	int min = rtc.minute() + 1;
	int hr = rtc.hour();
	rtc.setAlarm1(min, hr);
	if (rtc.alarm1(false))
	{
		Serial.println("In setupAlarm().  Alarm was actually set ");
	}
	Serial.println("In setupAlarm().  Alarm is set " + String(hr) + ":" + String(min));

	
	//void writeToRegister(uint8_t address, uint8_t data);
	//uint8_t readFromRegister(uint8_t address);
	uint8_t regval = rtc.readFromRegister(0x0E);
	Serial.println("In setupAlarm(). Before regval=" + String(regval));
	// set 00XXX;
	//uint8_t regval = 29;// original value
	regval = regval & ~(11 << 3);
	Serial.println("In setupAlarm(). After regval=" + String(regval));
	rtc.writeToRegister(0x8E, regval);
	//rtc.writeSQW(SQW_SQUARE_1);
	//printHex(regval);
	
}

void printHex(uint8_t val)
{
	char buffer[64];
	sprintf(buffer, "%d", val);
	Serial.println("Hex: ");
	Serial.println(buffer);
}

void setupLeds()
{
	FastLED.addLeds<APA102, LEDS_DATA_PIN, LEDS_CLOCK_PIN, BGR>(leds, N_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FastLED.setMaxRefreshRate(LED_REFRESH_RATE);
}

void updateMillisOffset()
{
	rtc.update();
	millisOffset = 
		(rtc.hour() * millisFactors[0])
		+ (rtc.minute() * millisFactors[1])
		+ (rtc.second() * millisFactors[2])
	;
}

unsigned long getTime()
{
	// keep time under 24 hours
	return (millisOffset + millis()) % (24L * millisFactors[0]);
}

void loop()
{
	renderTime();
	delay(100);
}

void renderTime()
{
	updateBTParts();
	// operating little-endian first to right-justify digits
	for(int row = 2; row >= 0; row--)
	{
		unsigned long bits = btParts[row];
		for(int col = 2; col >= 0; col--)
		{
			// twits=twin+bits
			int twits = bits & 3L; // 3D=11B
			bits = bits >> 2;
			leds[ledMap[row][col]] = colorMap[twits];
		}
	}
	FastLED.show();
}

void updateBTParts()
{
	unsigned long rem = getTime();
	for(int i = 0; i < 4; i++)
	{
		btParts[i] = rem / btFactors[i];
		rem -= btParts[i] * btFactors[i];
		// special handling of hour row
		if(i == 0)
		{
			// day quadrant
			unsigned long quadiem = btParts[i] / 6L;
			// use 12 hour clock, and set left-most bits to quadiem
			btParts[i] = (btParts[i] % 12) + (quadiem << 4);
		}
	}
}
