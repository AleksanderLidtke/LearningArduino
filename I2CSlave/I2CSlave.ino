/* Receive a value to be set for three variables, X, Y and Z, or requests to get
the current value of those variables. Do all the communications over I2C.

General telecommand format has one byte for getting and two for setting. If we're
setting then the message format is:
[const char* VariableAlias, int ValueToSet],
where ValueToSet is an int (0,255) i.e. 1-byte integer.
If we're just getting the value the format is:
[const char* VariableAlias].
The VariableAlias has to have the alias that corresponds to a given variable,
i.e. simply 'X', 'Y' or 'Z'.
We will know whether we're setting or getting by looking at the length of
the message we get.

The test circuit is simple: connect GND to GND, A4 to A4 (SDA), and A5 to A5 (SCL)
on both Arduinos, then connect both Arduinos to a PC/laptop with USB cables to have
data and power connections.

@author: Alek
@version: 1.0.0
@since: 17 May 2015

CHANGELOG:
17 May 2015 - 1.0.0 - First release with all the functionality of getting and setting three variables X, Y, and Z.
*/
#include <Wire.h>

int SLAVE_I2C_ADDRESS = 4; // Address of this Arduino on the I2C bus.
int X=-1; int Y=-1; int Z=-1; // The variables that will be set or got by the Master Arduino.
const char XAlias='X'; const char YAlias='Y'; const char ZAlias='Z'; // These are the names that refer to the actual variables stored here.
char RetVar='N'; // When we try to get a variable from this Arduino we first need to tell it which variable we want. This information is stored here.

// Prototypes.
void receiveEvent(int howMany);
void requestEvent(void);

void setup(void)
/* Prepare to receive and transmit over I2C. Called when Arduino starts up. */
{
	Wire.begin(SLAVE_I2C_ADDRESS); // Join the I2C bus with a given address.
	Wire.onReceive(receiveEvent); // Register event listener that will handle reception of commands on I2C.
	Wire.onRequest(requestEvent); // This listener handles requests for data from the Master.
}

void loop(void)
/* Called repeatedly whilst Arduino is active. Do nothing and wait to get stuff over I2C. */
{
	delay(100); // Just do nothing and wait for transmissions over I2C.
}

void receiveEvent(int howMany)
/* Function that executes whenever data is received from master over the I2C bus.

This function is registered as an event, @see setup().

When this kicks in the main loop will pause. Then this will execute and, when
this method is finished, the main loop will resume where it left off.

@param howMany - how many bytes are sent in the I2C message.
*/
{
	char VarAlias = Wire.read(); // A variable alias that tells us which variable to set.
	if (howMany==2) // We're setting a variable - the message has 3 bytes [VariableAlias, ValueToSet].
	{
		if (VarAlias==XAlias) // The Master has set X.
		{
			X = Wire.read();
		}
		else if (VarAlias==YAlias)
		{
			Y = Wire.read();
		}
		else if (VarAlias==ZAlias)
		{
			Z = Wire.read();
		}
	}
	else if (howMany==1) // Get the alias of the variable we are supposed to return now.
	{
		RetVar = VarAlias;
	}
}

void requestEvent(void)
/* Function that executes whenever data is requested by the master over the I2C bus.
Will send the value stored in the desired variable

This function is registered as an event, @see setup().
*/
{
	if (RetVar==XAlias) // The Master has requested X.
	{
		Wire.write(X);
	}
	else if (RetVar==YAlias)
	{
		Wire.write(Y);
	}
	else if (RetVar==ZAlias)
	{
		Wire.write(Z);
	}
	else // No idea what the Master wants.
	{
		Wire.write(-1);
	}
}
