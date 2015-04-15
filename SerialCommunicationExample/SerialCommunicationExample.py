# -*- coding: utf-8 -*-
"""
Created on Wed Apr 15 18:41:47 2015

@author: alek

Look at
http://stackoverflow.com/questions/27858041/oserror-errno-13-permission-denied-dev-ttyacm0-using-pyserial-from-pyth
for details and editing permissions to the serial ports.
"""

import sys, serial

"""
    ===========================================================================
    SIMPLE IO EXAMPLE.
    ===========================================================================
"""
ser=serial.Serial('/dev/ttyACM0', 9600)
if not ser.readable():
    raise IOError("Arduino isn't readable! Check the connection.")
    sys.exit()

" Read the message from Arduino. "
print ser.readline()

""" Write a simple message to Arduino. 
    This uses Arduino loop code:
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
	}
"""
MSG = b'5'
ser.write(MSG) # Convert the string to bytes. Optional in Python 2.X, mandatory in 3.X.
print "We sent {}. Arduino responded with {}.".format(MSG,ser.readline()) # Arduino should sent the same stuff back.

#""" Write a bit more complicated message to Arduino.
#    This uses Arduino loop code:
#    	char array[4] = { '0', '0', '0', '0'}; // Array to which the incoming chars will be written.
#	if (Serial.available() > 3) // Got more than 3 bytes/characters in the buffer - fill the array with them.
#	{
#		for (int i=0; i < 4 ; i++)
#		{
#		    array[i] = Serial.read();
#		}
#		Serial.flush();
#		
#		for(int i=0; i<4; i++) // Acknowledge the reception.
#		{
#			Serial.println(array[i]);
#		}
#	}
#"""
#LONGMSG = b'1234'
#ser.write(LONGMSG)
#print "An array in Arduino is now [{},{},{},{}].".format(ser.readline(), ser.readline(), ser.readline(), ser.readline())