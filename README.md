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
