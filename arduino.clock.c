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
// MAX_INT =  65535
unsigned long offsetTime = 26400000L;
//unsigned long offsetTime = 26400L;

// offsetTime + millis()
unsigned long time;

unsigned long binTimeParts[4] = {0,0,0,0};
unsigned long partFactors[4] = {
  1000L * 60L * 60L,
  (1000L * 60L * 60L) / 64L,
  round(((1000L * 60L * 60L) / 64L) / 64L),
  //(1000.0 * 60.0) / 64.0,
  1L//float(1)
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
  unsigned long rem = readTime();
  for(int i = 0; i < 4; i++)
  {
    binTimeParts[i] = rem / partFactors[i];
    rem -= binTimeParts[i] * partFactors[i];
  }
}

unsigned long readTime()
{
  time = offsetTime + millis();
  return time;
}

void printReport()
{
  char buffer[32];
  char buffer2[32];
  char buffer3[32];
  //Serial.println(
  //  "Factors Hours="+String(partFactors[0])
  //   +", Mins="+String(partFactors[1])
  //   +", Secs="+String(partFactors[2])
  //   +", MS="+String(partFactors[3])
  //);
  sprintf(buffer2, "Factors Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
    partFactors[0], partFactors[1],
    partFactors[2], partFactors[3]
  );
  Serial.println(buffer2);
  memset(buffer2, 0, sizeof(buffer));
  
  sprintf(buffer, "time=%lu, math=%lu", time, 26402092L/3600000L);
  Serial.println(buffer);
  memset(buffer, 0, sizeof(buffer));
  
  sprintf(buffer3, "Parts Hours=%lu, Mins=%lu, Secs=%lu, MS=%lu",
    binTimeParts[0], binTimeParts[1],
    binTimeParts[2], binTimeParts[3]
  );
  Serial.println(buffer3);
  memset(buffer3, 0, sizeof(buffer));
}
