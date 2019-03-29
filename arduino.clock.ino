
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
#define LED_REFRESH_RATE 64

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

unsigned long btParts[4] = {0,0,0,0};

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
}

void setupLeds()
{
	FastLED.addLeds<APA102, LEDS_DATA_PIN, LEDS_CLOCK_PIN, BGR>(leds, N_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FastLED.setMaxRefreshRate(LED_REFRESH_RATE);
	//pinMode(LED_BUILTIN, OUTPUT);
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
			//Serial.println(String(colorMap[twits], HEX));
		}
	}
	FastLED.show();
}

void updateBTParts()
{
	rtc.update();
	unsigned long precision = 10000L;
	uint8_t hour = rtc.hour();
	unsigned long second = (
		(((unsigned long)rtc.minute()) * 60L) 
		+ ((unsigned long)rtc.second())
	) * precision;

	unsigned long partFactors[3] = {
		0,
		(precision * 60L * 60L) / 64L,
		((precision * 60L * 60L) / 64L) / 64L,
		10
	};
	
	for(int i = 0; i < 4; i++)
	{
		btParts[i] = second / partFactors[i];
		second -= btParts[i] * partFactors[i];
	}

	// day quadrant
	uint8_t quadiem = hour / 6;
	btParts[0] = (hour % 12) + (quadiem << 4);
}


void printReport()
{
	char buffer[32];
	sprintf(buffer, "Parts Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
		btParts[0] & 15, btParts[1],
		btParts[2], btParts[3]
	);
	Serial.println(buffer);
	memset(buffer, 0, sizeof(buffer));
}
