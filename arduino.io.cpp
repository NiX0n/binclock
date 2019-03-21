String date;

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
	cmd = "date set "; if(in.startsWith(cmd))
	{
		setDate(in.substring(cmd.length()));
		return;
	}
	
	cmd = "date get"; if(in.startsWith(cmd))
	{
		getDate();
		return;
	}
	
	Serial.println("ERROR: Command not found");
}

void setDate(String __date)
{
	Serial.println("setDate():" + __date);
	date = __date;
}

void getDate()
{
	Serial.println("getDate():" + date);
	//return date;	
}
