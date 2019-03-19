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
// 24 hours = 86400000 ms
unsigned long offsetTime = 26400000L;

// offsetTime + millis()
unsigned long time;

unsigned long binTimeParts[4] = {0,0,0,0};
float partFactors[4] = {
  1000.0 * 60.0 * 60.0,
  (1000.0 * 60.0 * 60.0) / 64.0,
  ((1000.0 * 60.0 * 60.0) / 64.0) / 64.0,
  //(1000.0 * 60.0) / 64.0,
  float(1)
};

void setup()
{
  setupSerial();
  setupLeds();
  //printReport();
  //Serial.println("PFs: " + String(partFactors[0]));
  //Serial.println("PFs: " + String(partFactors[1]));
  //Serial.println("PFs: " + String(partFactors[2]));
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
  
  //readTime();
  printReport();
  
  delay(1000);
}

void refreshBinTimeParts()
{
  unsigned int now = readTime();
  unsigned int rem = now;
  for(int i = 0; i < 4; i++)
  {
    binTimeParts[i] = (unsigned int)(float(rem) / partFactors[i]);//floor(rem / partFactors[i]);
    rem -= binTimeParts[i];
  }
}

unsigned int readTime()
{
  time = offsetTime + millis();
  return time;
}

void printReport()
{
  char buffer[32];
  Serial.println("Factors "
     +  "Hours="+String(partFactors[0])
     +", Mins="+String(partFactors[1])
     +", Secs="+String(partFactors[2])
     +", MS="+String(partFactors[3])
  );
  
  sprintf(buffer, "time=%lu", time);
  Serial.println(buffer);
  
  sprintf(buffer, "Parts Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
    binTimeParts[0], binTimeParts[1],
    binTimeParts[2], binTimeParts[3]
  );
  Serial.println(buffer);
}
