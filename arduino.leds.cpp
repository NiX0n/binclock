#include "FastLED.h"

CRGB leds[9];

void setup()
{
	FastLED.addLeds<NEOPIXEL, 6>(leds, 9);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	leds[0] = CRGB::White;
	leds[1] = CRGB::Red;
	FastLED.show();
	delay(1000);
}
