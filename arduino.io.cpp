// For piping date into Serial
// Example: echo "date set $(date -Iseconds)" > /dev/ttyUSB0
// @see http://playground.arduino.cc/Interfacing/LinuxTTY

// Simple storage of imput string for later recall
String date;

// String position map of ISO8601-formatted date
int posMapISO8601[][2] = {
	// year,  month,   day
	{0,  4}, {5,  2}, {8,  2},
	// hour,  minute,  second
	{11, 2}, {14, 2}, {17, 2}
};

// Breakdown of date time parts
// Using posMapISO8601
// This is representative of the arguments
// passed into RTC->setDate();
int datetime[6] = {0,0,0,0,0,0};

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
		getDate();
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
	for(int i = 0; i < 6; i++)//sizeof(datetime)
	{
		//String dp = 
		datetime[i] =  d.substring(
			posMapISO8601[i][0], 
			posMapISO8601[i][0] + posMapISO8601[i][1]
		).toInt();
		char buffer[32];
		sprintf(buffer, "datetime[%d]=%d", i, datetime[i]);
		Serial.println(buffer);
	}
	date = d;
	Serial.println("date=" + d);
}

void getDate()
{
	Serial.println(date);
	//return date;	
}
