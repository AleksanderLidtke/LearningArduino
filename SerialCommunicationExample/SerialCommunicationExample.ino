void setup()
{
	Serial.begin(9600); // Sets the velocity of dates in bits per second (baud).
	Serial.println("Arduino active."); // Let the server know we're up.
	pinMode(13, OUTPUT); // Use the in-built diode to test casting from chars to usable ints.
} 

void loop()
{
	/* READ A SINGLE CHARACTER. */
	String content = "";
	char character;

	while(Serial.available()) // Read data incoming from the serial port one byte/char at a time.
	{
	  character = Serial.read();
	  content.concat(character);
	}

	if (content != "")
	{
		Serial.println(content); // Re-send the received information to see if it works.
		  digitalWrite(13, HIGH); // Shine the diode for the desired number of seconds.
		  delay(1000*content.toInt()); // ms
		  digitalWrite(13,LOW);
	}

	/* READ SEVERAL CHARACTERS. */
/*	char array[4] = { '0', '0', '0', '0'}; // Array to which the incoming chars will be written.
	if (Serial.available() > 3) // Got more than 3 bytes/characters in the buffer - fill the array with them.
	{
		for (int i=0; i < 4 ; i++)
		{
		    array[i] = Serial.read();
		}
		Serial.flush();
		
		for(int i=0; i<4; i++) // Acknowledge the reception.
		{
			Serial.println(array[i]);
		}
	}*/
}
