// Arduino includes
#include <Servo.h>

#define DATABUFFERSIZE      80
char dataBuffer[DATABUFFERSIZE+1]; //Add 1 for NULL terminator

int ledPin = 9; // Status indicator diode.
int servoPin = A0; // Control voltage.

Servo servoMotor; // Servo motor object.

void setup(void)
{
  pinMode(ledPin,OUTPUT);
  
  servoMotor.attach(servoPin); // Connect servo with the pin.
  
  Serial.begin(9200); // Print status.
  Serial.println("Ready");
}

void loop(void)
{
  digitalWrite(ledPin, HIGH); // Light the LED to show the thing is working.
  
  //long serial = 0;
  /*if ( Serial.available())
  {*/
  if (getSerial())
  {
    Serial.print("Got stuff: ");
    Serial.println(dataBuffer);
  }
    
  //}
  
  // Every second move the servo in a cyclic pattern.
  /*servoMotor.write(0);
  delay(1000);
  servoMotor.write(100);
  delay(1000);
  servoMotor.write(200);
  delay(1000);
  servoMotor.write(100);
  delay(1000);*/
  
  // Receive a message from serial port and send it back.

  /*if ( Serial.available())
  {
    String content = "";
    char ch = Serial.read();
    content.concat(ch);
    
    if (content != "")
    {
      Serial.println(content); // Re-send the received information to see if it works.
    }
  }*/
  
  // Other way of receiving messages.
  /*if (Serial.available())
  {
     //make sure we have all the data
     delay(5);
     
     inbyte = Serial.read();
      
     if (inbyte > 0)
     {
       serialData = serialData * 10 + inbyte - '0';
     
       Serial.println(serialData);
     }
   }*/
  
  /*static int v = 0;

  if ( Serial.available()) {
    char ch = Serial.read();

    switch(ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 's':
        servo1.write(v);
        v = 0;
        break;
      case 'w':
        servo2.write(v);
        v = 0;
        break;
      case 'd':
        servo2.detach();
        break;
      case 'a':
        servo2.attach(15);
        break;
    }
  }*/
}

/*unsigned long serialdata;
int inbyte;*/

boolean getSerial()
/* Reads an int from the serial port. */
{
  /*serialdata = 0;
  while (inbyte != '/')
  {
    inbyte = Serial.read();
    if (inbyte > 0 && inbyte != '/')
    {
      serialdata = serialdata * 10 + inbyte - '0';
    }
    delay(5);
  }
  inbyte = 0;
  Serial.println(serialdata);
  return serialdata;*/
  
  char startChar = '$'; // data chunk delimiters
  char endChar = '!';
  
  static boolean storeString = false; // Only write data when working on a recognisable data chunk
  static byte dataBufferIndex = 0;
  
  while(Serial.available()>0)
  {
    Serial.println(Serial.available());
    delay(15);
    
    char incomingbyte = Serial.read();
    
    Serial.println(incomingbyte);
    
    if(incomingbyte==startChar)
    {
        dataBufferIndex = 0;  //Initialize our index variable
        storeString = true;
    }
    
    if(storeString)
    {
        if(dataBufferIndex==DATABUFFERSIZE)
        {
            //Oops, our index is pointing to an array element outside our buffer.
            dataBufferIndex = 0;
            break; // Exit the while loop
        }
        
        if(incomingbyte==endChar)
        {
            dataBuffer[dataBufferIndex] = 0; //null terminate the C string
            //Our data string is complete
            storeString = false;
            dataBufferIndex = 0;
            return true;
        }
        else
        {
            dataBuffer[dataBufferIndex++] = incomingbyte;
        }
    }
  }
  
  return false;
}
