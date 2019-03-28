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
