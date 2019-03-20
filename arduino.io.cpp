void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	String in = readSerial();
	if(in.length() > 0)
	{
		Serial.println(in);
	}
	delay(1000);
}

String readSerial()
{
	String ret = "";
	while(Serial.available() > 0)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		ret += (char)Serial.read();
	}
	digitalWrite(LED_BUILTIN, LOW);
	return ret;
}
