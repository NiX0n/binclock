#define N_LEDS 9
#define LEDS_PIN 6
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
	FastLED.addLeds<WS2812B, LEDS_PIN>(leds, N_LEDS);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	leds[0] = CRGB::White;
	leds[1] = CRGB::Red;
	FastLED.show();
	delay(1000);
}
