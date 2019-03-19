#define LEDS_PIN 6
#define N_LEDS 12
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel leds = Adafruit_NeoPixel(N_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);


int hourLeds[3] = {1,2,3};
int minLeds[3] = {5,6,7};
int secLeds[3] = {9,10,11};


// Mon 18 March 2019
// 12:41:54
//unsigned long  startTime = 1552927314170;

// Offset from midnight
//unsigned long offsetTime = 0;
unsigned long offsetTime = 26400000L;

// offsetTime + millis()
unsigned long time = 26400000L;

unsigned long binTimeParts[4] = {0,0,0,0};
float partFactors[4] = {
//  float(1000) * float(60) * float(60),
//  float(1000) * (float(60) / float(64)) * (float(60) / float(64)),
//  float(1000) * (float(60) / float(64)),
  1000.0 * 60.0 * 60.0,
  (1000.0 * 60.0 * 60.0) / 64.0,
  //1000.0 * (60.0 * (60.0 / 64.0)) * (60.0 * (60.0 / 64.0)),
  (1000.0 * 60.0) / 64.0,
  float(1)
};

void setup()
{
  setupSerial();
  setupLeds();
  
  Serial.println("PFs: " + String(partFactors[0]));
  Serial.println("PFs: " + String(partFactors[1]));
  Serial.println("PFs: " + String(partFactors[2]));
}

void setupSerial()
{
  Serial.begin(9600);
}

void setupLeds()
{
  leds.begin();
  leds.setPixelColor(8, 255,0,0);
  leds.show();
  //pinMode(13, OUTPUT);
}

void loop()
{
  refreshBinTimeParts();
  //Serial.println("Hours: " + String(binTimeParts[0]));
  //Serial.println("Mins: " + String(binTimeParts[1]));
  //Serial.println("Secs: " + String(binTimeParts[2]));
  //Serial.println("MS: " + String(binTimeParts[3]));
  
  //unsigned long now = readTime();
  //Serial.println("Start Time: " + String(startTime, DEC));
  //Serial.println("Time: " + String(now));
  //Serial.println("MS: " + String(millis(), DEC));//String(now));
  //digitalWrite(13, HIGH);
  //delay(1000); // Wait for 1000 millisecond(s)
  //digitalWrite(13, LOW);
  //delay(1000); // Wait for 1000 millisecond(s)
  delay(1000);
}

void refreshBinTimeParts()
{
  unsigned long now = readTime();
  unsigned long rem = now;
  Serial.println("int(millis())="+String(millis())+", now=" + String(now) +", rem=" + String(rem));
  for(int i = 0; i < 4; i++)
  {
    binTimeParts[i] = (unsigned long)(float(rem) / partFactors[i]);//floor(rem / partFactors[i]);
    rem -= binTimeParts[i];
    Serial.println("now=" + String(now, DEC) + ", i="+i+", part=" + String(binTimeParts[i]) + ", rem=" + String(rem));
    //= fmod(rem, partFactors[i]);
    //binTimeParts[i] = now;
  }
}

unsigned int readTime()
{
  //time = ((unsigned long)(offsetTime)) + ((unsigned long)(millis()));
  //time = offsetTime;
  Serial.println(
    "cast(millis())="+String(((unsigned long)(millis())))
    +", offsetTime=" + String(((unsigned long)(offsetTime))) 
    +", time=" + String(time));
  return time;
}
