# binclock
Binary Clock

This project attempts to diplay time using a binary-like (power of two) representation.  It was designed for entertainment purposes, and not some attempt to overthrow the status quo of time-keeping.

In this implementation, we're using a 3x3 grid of RBG LEDs.  We could in theroy use a vast number of colors to represent different numbers.  However, I've chosen to use 4 colors to represent two bits.

| Color | Dec | Bin |
| ----- | --- | --- |
| Black |  0  |  00 |
| Red   |  1  |  01 |
| Green |  2  |  10 |
| Blue  |  3  |  11 |


## Hours ##
Each of the three rows in the LED grid represents hours, minutes, and seconds respectively.  I have chosen to maintain 24 hours in a day.

The first row does have enough bits to represent 24; however, that only requires 5 bits, and that ends up wasting the left-most bit.  Instead, I've chosen to use a 12 (0-11) hour clock, which only requires 4 bits.  This leaves the left 2 bits available for other uses.  In this impelmentation, I've parodied an AM/PM bit; except with 2 bits instead.  Something I've called a "quadiem" (or day quadrant).

Unlike a traditional clock, there is no "12 o'clock"; only "0 o'clock".

## Minutes / Seconds ##
In this implementation, I have chosen to divide real hours by 64 "minutes", and each of those minutes by 64 "seconds."  These minutes and seconds can each be represented using 6 bits (3 LEDs), counting from 0-63.

## Parts List ##
In the final implmentation, these are the parts used to build the clock.
* [Arduino Uno R3](https://www.sparkfun.com/products/13975)
* [SparkFun DS3234 DeadOn Real-Time Clock (RTC) Breakout](https://learn.sparkfun.com/tutorials/deadon-rtc-breakout-hookup-guide)
  * An RTC is required for persisting time across power cycles; however, a version of the clock can run using only the Arduino function millis() and an initial offsetTime.
* [SparkFun Lumenati 3x3](https://learn.sparkfun.com/tutorials/lumenati-hookup-guide)
  * The tutorial recommends using a 3v-5v logic translator, but the Arduino board chosen already supplies 5v.
  * The tutorial also recommends using an external 5v power supply (not for clock/data), but this is really only applicable when using a long series of LEDs.  For only 9 LEDs, I was able to get away without it.

During prototyping, a virtual environment was used on TinkerCAD, and a series of virtual Adafruit NeoPixel LED strips were used.  There is a branch to this repository dedicated to supporting this virtual implementation.

## Library Dependencies ##

### [FastLED](https://github.com/FastLED/FastLED) ###
This library is very simple to use.  Particularly compared to the ones used by Adafruit NeoPixel.

One gotcha is I had to call FastLED.setMaxRefreshRate(LED_REFRESH_RATE), where LED_REFRESH_RATE = some sane value.  Without this line, the LEDs were stuck in a very unusable glitchy state.

### [SparkFun DS3234 RTC](https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library) ###
I have no complaints about this library on its own; however it does depend on SPI which has its own issues.

Because this unit only time resolution down to the second, and our "binary second" requires higher precision; the RTC is only used to initialize a time offset during setup().  In loop(), the clock relies on millis() + offset to drive time.  While this isn't idal in terma of keeping in sync with RTC; it does maintain well timed and consistent counting.

Implementations utilizing only the RTC has resulted in skipped seconds.

### SPI ###
This library is mostly transparent to the application.  One gotcha however, is that defined in some arduino_pins.h file, is some hard coded definition of what pins should be used.

## Source Files ##  
There are multiple separate "arduino.\*" files.  Each has a unique purpose, and most are for the purposes of isolated diagnostics.

### arduino.clock.ino ###
This is the primary source file for driving the LED clock.

### arduino.rtc.cpp ###
This file is used for getting and setting time.  It leverages the serial interface as a means of efficiently setting the time.

From a terminal you can run
```
$ stty -F /dev/ttyUSB0 -hupcl
$ echo "date set $(date -Iseconds)" > /dev/ttyUSB0
```

From Serial Monitor there are two commands you can run

| Command             | Description |
| ------------------- | ----------- |
| date get            | Get the current datetime.  If RTC isn't connected or set, this will return zeros for all date/time parts. |
| date set {datetime} | Set the current date/time.  {datetime} is expected to be in ISO-8601 format |

### arduino.leds.cpp ###
This file is for doing diagnostics on the LEDs.

### arduino.io.cpp ###
This file is for doing diagnostics on Serial input/output processing.
