
// References:
// Clock
// https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library
// https://learn.sparkfun.com/tutorials/deadon-rtc-breakout-hookup-guide
// LEDs
// https://learn.sparkfun.com/tutorials/lumenati-hookup-guide

//#define DS13074_SS_PIN 11
//#define DS13074_MOSI_PIN 10
//#define DS13074_MISO_PIN 9
//#define DS13074_CLK_PIN 8

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

unsigned long btParts[4] = {0,0,0,0};
unsigned long btFactors[4] = {
	1000L * 60L * 60L,
	(1000L * 60L * 60L) / 64L,
	round(((1000L * 60L * 60L) / 64L) / 64L)//,
	//1L
};


void setup()
{
	setupSerial();
	setupRtc();
	setupLeds();
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

void setupLeds()
{
	FastLED.addLeds<APA102, LEDS_DATA_PIN, LEDS_CLOCK_PIN, BGR>(leds, N_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FastLED.setMaxRefreshRate(LED_REFRESH_RATE);
	//pinMode(LED_BUILTIN, OUTPUT);
}

void updateMillisOffset()
{
	rtc.update();
	millisOffset = 
		(rtc.hour() * btFactors[0])
		+ (rtc.minute() * btFactors[1])
		+ (rtc.second() * btFactors[2])
	;
}

unsigned long getTime()
{
	// keep time under 24 hours
	return (millisOffset + millis()) % (24L * btFactors[0]);
}

unsigned int getMillisOffset()
{
	return (millis()  % 1000L) - millisOffset;
}

void loop()
{
	renderTime();
	//printReport();
	//delay(20);
	delay(100);
	//delay(btParts[3] > 0 ? btParts[3] : 879);
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

String dec2bin(unsigned long dec)
{
	String ret = "B";
	unsigned long mask = 1L << 0x1F;
	for(int i = 0; i < 0x20; i++)
	{
		ret += (dec & mask) > 0 ? "1" : "0";
		dec = dec << 1;
	}
	return ret;
}

void printReport()
{
	char buffer[100];
	sprintf(buffer, "Parts Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
		btParts[0] & 15L, btParts[1],
		btParts[2], btParts[3]
	);
	Serial.println(buffer);
	//memset(buffer, 0, sizeof(buffer));*/
	rtc.update();
	char buffer2[50];
	sprintf(buffer2, "Real Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
		(unsigned long)rtc.hour(), (unsigned long)rtc.minute(),
		(unsigned long)rtc.second(), millis()
	);
	Serial.println(buffer2);
	memset(buffer2, 0, sizeof(buffer2));
}
