#include <Wire.h>

void receiveEvent(int howMany); // Prototype.

void setup(void)
{
	/* Called when Arduino starts up. */
	pinMode(13, OUTPUT); // Use this pin to show that something was received.
	Wire.begin(4); // Join the I2C bus with address #4.
	Wire.onReceive(receiveEvent); // Register event listener.
	Serial.begin(9600); // Start serial to show the stuff received over I2C there.
}

void loop(void)
{
	/* Called repeatedly whilst Arduino is active. */
	delay(100); // Just do nothing most of the time.
}

void receiveEvent(int howMany)
/* Function that executes whenever data is received from master over the I2C bus.
	This function is registered as an event, see setup().
*/
{
	while(1 < Wire.available()) // Loop through all but the last part of the message.
	{
		char c = Wire.read(); // Receive byte as a character...
		Serial.print(c); //...and print the character to the serial port.
	}
	int x = Wire.read(); // Receive bytes as an integer.
	Serial.println(x); // Print the int.
  
	// Light the LED to show that a message has been received.  
  	digitalWrite(13, HIGH);
	delay(1000); // ms
	digitalWrite(13,LOW);
}
  
