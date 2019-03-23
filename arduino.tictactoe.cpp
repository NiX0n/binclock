
// References:
// https://learn.sparkfun.com/tutorials/lumenati-hookup-guide

#define N_LEDS 9
#define LEDS_DATA_PIN 6
#define LEDS_CLOCK_PIN 7
#define LED_BRIGHTNESS  64

#define SERIAL_BAUD 115200

#include "FastLED.h"

CRGB leds[N_LEDS];

bool player1 = true;

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
	FastLED.setBrightness(LED_BRIGHTNESS);
	FastLED.setMaxRefreshRate(1);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loopOld()
{
	for(int i = 0; i < N_LEDS; i++)
	{
		leds[i] = random(0xFFFFFF);
	}
	FastLED.show();
	delay(1000);
	//FastLED.delay(2000 / UPDATES_PER_SECOND);
}


void loop()
{
	String in = readSerial();
	if(in.length() > 0)
	{
		processIn(in);
	}
	delay(1000);
}

String readSerial()
{
	String ret = "";
	if(Serial.available() > 0)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		do 
		{
			ret += (char)Serial.read();
		} while(Serial.available() > 0);
		digitalWrite(LED_BUILTIN, LOW);
	}
	return ret;
}

void processIn(String in)
{
	Serial.println("> " + in);

	String cmd;
	cmd = "help"; if(in.startsWith(cmd))
	{
		// printHelp();
		return;
	}
	
	cmd = "play "; if(in.startsWith(cmd))
	{
		play(in.substring(cmd.length()));
		return;
	}
	
	cmd = "clear"; if(in.startsWith(cmd))
	{
		clear();
		return;
	}
	
	Serial.println("ERROR: Command not found");
}

void play(String coords)
{
  leds[i] = random(0xFFFFFF);
	
	FastLED.show()
}

void clear()
{
  
}
