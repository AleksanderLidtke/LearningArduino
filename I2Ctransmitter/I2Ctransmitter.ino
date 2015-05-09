#include <Wire.h>

int x = 0; // Variable that will be transmitted.

void setup(void)
{
	/* Called when Arduino starts up. */
	Wire.begin(); // Join the I2C bus (address optional for master).
}

void loop(void)
{
	/* Called repeatedly whilst Arduino is active. */
	Wire.beginTransmission(4); // Transmit a message to device #4.
	Wire.write( (byte)x ); // Sends one byte.
	Wire.endTransmission(); // Stop transmitting the message.
	x++; // Change the message.
	
	delay(10000);
}
  
