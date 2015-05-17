#include <Wire.h>

void receiveEvent(int howMany); // Prototype.

const int ledPinReceived = 13; // lights up when we get a message via i2c
const int ledPinBlinking = 9; // keeps blinking all the time

// connect GND to GND, A4 to A4 (SDA), and A5 to A5 (SCL)

void setup(void)
{
	/* Called when Arduino starts up. */
	pinMode(ledPinReceived, OUTPUT); // Use this pin to show that something was received.
	Wire.begin(4); // Joit a message to device #4.
	Wire.onReceive(receiveEvent); // Register event listener.
	Serial.begin(9600); // Start serial to show the stuff received over I2C there.

    pinMode(ledPinBlinking, OUTPUT);
}

void loop(void)
{
	/* Called repeatedly whilst Arduino is active. */
	Serial.print("Started main loop ... ");
	delay(100); // Just keep shining
    /*digitalWrite(ledPinBlinking, HIGH);
    delay(20000);
    digitalWrite(ledPinBlinking, LOW);*/
    Serial.println("Finished main loop ...");
}

int status = 0;

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
	
//	(status == 0)? status = 1 : status = 0;	
//	(status == 0)? digitalWrite(ledPinReceived,LOW) : digitalWrite(ledPinReceived, HIGH);

	// Light the LED to show that a message has been received.
    for (int i = 0; i < 5; i++)
    {
        Serial.print(" ");
        Serial.print(i);
        digitalWrite(ledPinReceived, HIGH);
  	    delay(100);
  	    digitalWrite(ledPinReceived,LOW);
        delay(100);
    }
    Serial.println("");
}
  
