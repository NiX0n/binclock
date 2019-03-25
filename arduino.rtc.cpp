// References:
// https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library
// https://learn.sparkfun.com/tutorials/deadon-rtc-breakout-hookup-guide

#define DS13074_SS_PIN 11
#define DS13074_MOSI_PIN 10
#define DS13074_MISO_PIN 9
#define DS13074_CLK_PIN 8

#define SERIAL_BAUD 115200

#include <SPI.h>
#include "SparkFunDS3234RTC.h"

// String position map of ISO8601-formatted date
int posMapISO8601[][2] = {
	// year,  month,   day
	{0,  4}, {5,  2}, {8,  2},
	// hour,  minute,  second
	{11, 2}, {14, 2}, {17, 2}
};

void setup()
{
	setupSerial();
	setupRtc();
}

void setupSerial()
{
	Serial.begin(SERIAL_BAUD);
}

void setupRtc()
{
	rtc.begin(DS13074_SS_PIN);
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
	
	cmd = "date set "; if(in.startsWith(cmd))
	{
		setDate(in.substring(cmd.length()));
		return;
	}
	
	cmd = "date get"; if(in.startsWith(cmd))
	{
		printDate();
		return;
	}
	
	Serial.println("ERROR: Command not found");
}

/**
 * @param String __date expected to be in ISO-8601 format
 * Example: 2019-03-21T09:37:59-04:00
 */
void setDate(String d)
{
	d.trim();
	int dt[6] = {0,0,0,0,0,0};
	for(int i = 0; i < 6; i++)//sizeof(datetime)
	{
		dt[i] =  d.substring(
			posMapISO8601[i][0], 
			posMapISO8601[i][0] + posMapISO8601[i][1]
		).toInt();
	}
	rtc.setTime(dt[5], dt[4], dt[3], dt[2], dt[1], dt[0]);
	Serial.println("SET!");
	printDate();
}

void printDate()
{
	char buffer[25];
	rtc.update();
	sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02d-04:00",
		rtc.year(), rtc.month(), rtc.date(),
		rtc.hour(), rtc.minute(), rtc.second()
 	);
	Serial.println(buffer);
}
