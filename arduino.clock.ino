
// References:
// Clock
// https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library
// https://learn.sparkfun.com/tutorials/deadon-rtc-breakout-hookup-guide
// LEDs
// https://learn.sparkfun.com/tutorials/lumenati-hookup-guide

#define DS13074_SS_PIN 11
#define DS13074_MOSI_PIN 10
#define DS13074_MISO_PIN 9
#define DS13074_CLK_PIN 8

#define N_LEDS 9
#define LEDS_DATA_PIN 6
#define LEDS_CLOCK_PIN 7
#define LED_BRIGHTNESS  64
#define LED_REFRESH_RATE 1

#define SERIAL_BAUD 115200

#include <SPI.h>
#include "SparkFunDS3234RTC.h"
#include "FastLED.h"

CRGB leds[N_LEDS];

int ledMap[][3] = {
	// hours
	{0,1,2},
	// min
	{3,4,5},
	// sec
	{6,7,8}
};

// colors described as RGB
int colorMap[][3] = {
	// 00 = BLACK
	0x000000,
	// 01 = RED
	0xFF0000,
	// 10 = GREEN
	0x00FF00,
	// 11 = BLUE
	0x0000FF
};

unsigned long btParts[3] = {0,0,0};

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
	rtc.begin(DS13074_SS_PIN);
}

void setupLeds()
{
	FastLED.addLeds<APA102, LEDS_DATA_PIN, LEDS_CLOCK_PIN, RGB>(leds, N_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FastLED.setMaxRefreshRate(LED_REFRESH_RATE);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	renderTime();
	delay(250);
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
	unsigned long rem = readTime();
	for(int i = 0; i < 4; i++)
	{
		binTimeParts[i] = rem / partFactors[i];
		rem -= binTimeParts[i] * partFactors[i];
		// special handling of hour row
		if(i == 0)
		{
			// day quadrant
			unsigned long quadiem = binTimeParts[i] / 6L;
			// use 12 hour clock, and set left-most bits to quadiem
			binTimeParts[i] = (binTimeParts[i] % 12) + (quadiem << 4);
		}
	}
}

