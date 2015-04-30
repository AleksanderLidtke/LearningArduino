// Arduino includes
#include <Servo.h>

// Standard c/C++ includes
#include <stdio.h>

// Input data buffer
#define DATABUFFERSIZE      80
char dataBuffer[DATABUFFERSIZE+1]; //Add 1 for NULL terminator

// data chunk delimiters
const char inputStartChar = '>';
const char inputEndChar = ';';
const char dataDelimiter[2] = ",";

// Pin and element definitions
int ledPin = 9; // Status indicator diode.
int servoPin = A0; // Control voltage.

Servo servoMotor; // Servo motor object.

// Servo control parameters
const char servoAngleLabel[] = "deltaServo"; // Used for parsing inputs
float servoAngle = 0; // Angle [deg] <0,180>

void setup(void)
{
  pinMode(ledPin,OUTPUT);
  
  servoMotor.attach(servoPin); // Connect servo with the pin.
  
  Serial.begin(19200); // Start serial comms
  
  Serial.println("Ready");
}

void loop(void)
{    
  // Light the LED to show the thing is working.
  digitalWrite(ledPin, HIGH);
  
  // Get control inputs
  if ( Serial.available())
  {
    if (getSerial())
    {
      parseInput();
    }
  }
  
  // Set all the pins to the desired level
  servoMotor.write(computeServoInput(servoAngle));
  
  // reduce rate at which stuff ha
  delay(1000);
}

int computeServoInput(float angle)
/* Rounds the float angle demand to an acceptable integer */
{
  int level = int(angle);
  if (level < 0)
    level = 0;
  else if (level > 180)
    level = 180;
  return level;
}

boolean getSerial()
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
            break; // Exit the while loop
        }
        
        if(incomingbyte==inputEndChar)
        {
            dataBuffer[dataBufferIndex] = 0; //null terminate the C string
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

void parseInput()
/* Extract numbers from the input buffer */
{
  // indicate which numbers are coming next
  boolean servoAngleNext = false;
  
  // get the first token
  char * token;
  token = strtok (dataBuffer, dataDelimiter);
  
  while (token != NULL)
  {
      // check available strings
      if (strcmp(token, servoAngleLabel) == 0)
      {
        servoAngleNext = true;
      }
      
      // check if any of the available numbers are coming
      else if (servoAngleNext)
      {
        servoAngle = atof(token);
        servoAngleNext = false;
      }
      
      // get a new token
      token = strtok (NULL, dataDelimiter);
  }
}
