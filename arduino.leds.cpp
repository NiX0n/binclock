// References:
// https://learn.sparkfun.com/tutorials/lumenati-hookup-guide

#define N_LEDS 9
#define LEDS_DATA_PIN 6
#define LEDS_CLOCK_PIN 7
#define LED_BRIGHTNESS  64

#define SERIAL_BAUD 115200

#include "FastLED.h"

CRGB leds[N_LEDS];

void setup()
{
	setupSerial();
	setupLeds();
}

void setupSerial()
{
	Serial.begin(SERIAL_BAUD);
}

void setupLeds()
{
	FastLED.addLeds<APA102, LEDS_DATA_PIN, LEDS_CLOCK_PIN, RGB>(leds, N_LEDS);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	for(int i = 0; i < N_LEDS; i++)
	{
		leds[i] = random(0xFFFFFF);
	}
	FastLED.show();
    FastLED.delay(2000 / UPDATES_PER_SECOND);
}
