double SAMPLING_F = 1.0; // Sampling frequency in Hz.
bool SENDING = false; // Whether we're sending data through the serial port.
unsigned long currentTime; // Time since Arduino started executing the current program until now.
double currentOutput = 0.0; // Dummy sensor value we'll be sending over serial.

void setup()
{
    currentTime = millis(); // Get the time since start (at this point probably 0).
    Serial.begin(9600); // Sets the velocity of dates in bits per second (baud).
    Serial.println( "Arduino active" ); // Let the server know we're up.
} 

void loop()
{
    currentTime = millis(); // Update current time stamp.
        
	if(!SENDING)
	{
		SENDING = true;
		Serial.println("Arduino sending");
	}
	else
	{
		Serial.println(currentTime); Serial.println("T"); // Let the server know this is time we just sent.
		Serial.println(currentOutput); Serial.println("V"); // This was the output.
		
		currentOutput = currentOutput + 1.0;
		if(currentOutput>=100.0)
		{
			currentOutput = 0.0; // Reset this periodically for good measure.
		}
		delay( (int)1000/SAMPLING_F );
	}
}
