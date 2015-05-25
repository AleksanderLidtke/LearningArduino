/* Receive a value to be set for three variables, X, Y and Z, over serial. Then
transmit these values to another Arduino over I2C. Can also transmit commands to
read the X, Y, Z values stored on the other Arduino and transmit these back over serial.

General telecommand format that both the Master and Slave expect (on the serial and
I2C buses, respectively) has one byte for getting and two for setting. If we're
setting then the message format is:
[const char* VariableAlias, int ValueToSet],
where ValueToSet is an int (0,255) i.e. 1-byte integer.
If we're just getting the value the format is:
[const char* VariableAlias].
For commands to the Master the VariableAlias has to have the variable name (i.e. X, Y or Z)
preceded by "g" or "s" so that we know whether to set or get a given variable, respectively.
The slave will know whether we're setting or getting by looking at the length of
the message it gets.

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
#include <stdio.h>

int LED_PIN = 13; // Use the in-built LED to show that all this is working.
int SLAVE_I2C_ADDRESS = 4; // Address of the Other Arduino on the I2C bus.

// Prototypes.
boolean getSerial(void);
void setVarOverI2C(char* var, int value, int I2CAddr);
int getVarOverI2C(char* var, int I2CAddr);
void parseInput(void);

// Input data buffer.
#define DATABUFFERSIZE 80
char dataBuffer[DATABUFFERSIZE+1]; //Add 1 for NULL terminator

// Data chunk delimiters.
const char inputStartChar = '>';
const char inputEndChar = ';';
const char dataDelimiter[2] = ",";

// Used to parse the telecommands to get or to set X, Y, Z variables on another Arduino.
const char setX[] = "sX"; const char getX[] = "gX";
const char setY[] = "sY"; const char getY[] = "gY";
const char setZ[] = "sZ"; const char getZ[] = "gZ";

void setup(void)
/* Prepare LED output, and join I2C and serial buses.
Called when Arduino starts up.
*/
{

	pinMode(LED_PIN,OUTPUT);
	Wire.begin(); // Join the I2C bus (address optional for master).
	Serial.begin(9600); // Start serial to show the stuff received over I2C there.
}

void loop(void)
/* Called repeatedly whilst Arduino is active. */
{
	digitalWrite(LED_PIN, HIGH); // Light the LED to show the thing is working.
	
	// Get control inputs.
	if ( Serial.available())
	{
		if (getSerial()) // Extract chars from the input stream that has come over serial.
		{
	  		parseInput(); // Will parse commands and execute them.
		}
	}
	
	delay(100);
}

boolean getSerial(void)
/* Reads an int from the serial port and keeps them in the buffer. */
{
  static boolean storeString = false; // Only write data when working on a recognisable data chunk
  static byte dataBufferIndex = 0;
  
  while(Serial.available()>0)
  {
    char incomingbyte = Serial.read();

    if(incomingbyte==inputStartChar)
    {
        dataBufferIndex = 0;  //Initialize index variable
        storeString = true;
    }
    
    if(storeString)
    {
        if(dataBufferIndex==DATABUFFERSIZE)
        {
            // index is pointing to an array element outside our buffer.
            dataBufferIndex = 0;
            break; // Exit the while loop.
        }
        
        if(incomingbyte==inputEndChar)
        {
            dataBuffer[dataBufferIndex] = 0; // Null terminate the C string.
            storeString = false;
            dataBufferIndex = 0;
            return true;
        }
        else if (incomingbyte!=inputStartChar)
        {
            dataBuffer[dataBufferIndex++] = incomingbyte;
        }
    }
  }
  
  return false;
}

void setVarOverI2C(char* var, int value, int I2CAddr)
/* Set the value of a variable stored on another Arduino via the I2C data bus.
@param var - name of the variable we're setting recognised by the other Arduino.
@param value - the value to be set.
@param I2CAddr - address of the other Arduino on the I2C bus.
*/
{
	Wire.beginTransmission(I2CAddr); // Transmit a message to the Other Arduino.
	Wire.write( var ); // Say which variable it is.
	Wire.write( byte(value) ); // Send the actual value to be set.
	Wire.endTransmission(); // Finish the message.
}

int getVarOverI2C(char* var, int I2CAddr)
/* Get the value of a variable stored on another Arduino via the I2C data bus.
It will pause the process for 50 ms.
@param var - name of the variable we're setting recognised by the other Arduino.
@param I2CAddr - address of the other Arduino on the I2C bus.
@return value - the value to be set.
*/
{
	Wire.beginTransmission(I2CAddr); // Transmit a message to the Other Arduino.
	Wire.write( var ); // Say which variable it is.
	Wire.endTransmission(); // Finish the message.
	delay(50); // According to StackExchange and the formus we need to wait here for a while to let the other Arduino process the message. 50 ms seems to work fine.
	
	Wire.requestFrom(I2CAddr, 1);
	//int returnMessageLength = Wire.available(); // How many bytes the slave sent as a reply. Can call Wire.read in a while loop to get all the bytes from the message here.
	int response = Wire.read(); // Only expect one byte here.
	return response;
}

void parseInput(void)
/* Extract numbers from the input buffer and send appropriate commands to the
Other Arduino over I2C. If we get a response transmit it over the serial bus.
*/
{
	// Indicate which numbers are coming next.
	boolean XValueNext = false; boolean YValueNext = false; boolean ZValueNext = false;

	// Get the first token.
	char * token;
	token = strtok (dataBuffer, dataDelimiter);

	// Keep getting all the tokes.
	while (token != NULL)
	{
		// Check available strings to figure out what's coming - see if we are setting a variable on the other Arduino and which one.
		if (strcmp(token, setX) == 0){XValueNext = true;}
		else if (strcmp(token, setY) == 0){YValueNext = true;}
		else if (strcmp(token, setZ) == 0){ZValueNext = true;}

		// Check if any of the numbers to be set are coming now. If so set the appropriate variable over I2C.
		else if (XValueNext)
		{
			int XValue = atof(token);
			XValueNext = false; // OK, we've got the value for this variable that we want to set.
			setVarOverI2C("X", XValue, SLAVE_I2C_ADDRESS); // Now we just have to send it over I2C and set on the other Arduino.
			delay(5); // Need to wait here to let the I2C message finish properly.
			}
		else if (YValueNext)
		{
			int YValue = atof(token);
			YValueNext = false;
			setVarOverI2C("Y", YValue, SLAVE_I2C_ADDRESS);
			delay(5);
		}      
		else if (ZValueNext)
		{
			int ZValue = atof(token);
			ZValueNext = false;
			setVarOverI2C("Z", ZValue, SLAVE_I2C_ADDRESS);
			delay(5);
		}

		// Check for commands to get specific values from the other Arduino and return them over serial.
		else if (strcmp(token, getX) == 0)
		{
			int XTemp = getVarOverI2C("X", SLAVE_I2C_ADDRESS); // See what value is stored under this alias on the other Arduino.
			Serial.println(XTemp); // And print it to the serial bus.
			delay(5);
		}
		else if (strcmp(token, getY) == 0)
		{
			int YTemp = getVarOverI2C("Y", SLAVE_I2C_ADDRESS);
			Serial.println(YTemp);
			delay(5);
		}
		else if (strcmp(token, getZ) == 0)
		{
			int ZTemp = getVarOverI2C("Z", SLAVE_I2C_ADDRESS);
			Serial.println(ZTemp);
			delay(5);
		}
		
		// Get a new token of the message.
		token = strtok (NULL, dataDelimiter);
	}
}
