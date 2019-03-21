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

void parseIn(String in)
{
	String cmd;
	cmd = "date set "; if(in.startsWith(cmd))
	{
		setDate(in.substring(cmd.length()));
		return;
	}
}

void setDate(String __date)
{
	Serial.println("setDate():" + __date);
}
