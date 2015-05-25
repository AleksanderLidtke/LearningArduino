#include <Wire.h>

int x = 0; // Variable that will be transmitted.

// connect GND to GND, A4 to A4 (SDA), and A5 to A5 (SCL)

void setup(void)
{
	/* Called when Arduino starts up. */
	Wire.begin(); // Join the I2C bus (address optional for master).
}

void loop(void)
{
	/* Called repeatedly whilst Arduino is active. */
	Wire.beginTransmission(4); // Transmit a message to device #4.
	Wire.write( "\nsx" ); // Practice sending char arrays - will need these to distinguish telecommands.
	Wire.write( (byte)x ); // Sends one byte with an integer.
	Wire.write( "\n" );
	Wire.endTransmission(); // Stop transmitting the message.
	x++; // Change the message.
	
	delay(5000);
}
  
