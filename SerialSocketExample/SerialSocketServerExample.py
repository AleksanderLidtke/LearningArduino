#!/usr/bin/python
# -*- coding: utf-8 -*-
"""

A socket server that will redirect data from the Arduino to the client that
connect to it.

Created on Wed Apr 15 22:28:06 2015


@author: Alek & Artur
@version: 1.0.0
@since: 16 Apr 2015
"""
import sys, serial, socket

"""
    ---------------------------------------------------------------------------
    Setup.
    ---------------------------------------------------------------------------
"""
HOST = 'localhost'
PORT = 5431 # Arbitrary non-privileged port
MAX_CLIENTS = 5 # Maximum number of clients listening to this socket.
SIZE = 1024
ARDUINO_HAIL_MSG = "Arduino sending" # Arduino will send this when it starts to send meaningful data.
ARDUINO_SENDING = False # Whether Arduino is sending useful data.
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # In case this socket was used before.
print 'Socket created.'

arduinoSerial = serial.Serial('/dev/ttyACM0', 9600)
if not arduinoSerial.readable():
    raise IOError("Arduino isn't readable! Check the connection.")
    sys.exit()

"""
    ---------------------------------------------------------------------------
    Bind socket to local host and an unused port, and await clients.
    ---------------------------------------------------------------------------
"""
try:
    serverSocket.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()

print 'Socket bound to host {} on port {}.'.format(HOST, PORT)
serverSocket.listen(MAX_CLIENTS)

"""
    ---------------------------------------------------------------------------
    Wait to accept a connection - redirect data from Arduion to the client.
    ---------------------------------------------------------------------------
"""
client, address = serverSocket.accept()
print 'Connected with ' + address[0] + ':' + str(address[1])
client.sendall("You are: {}. I'll be sending you data from an Arduino.".format(address[0]+':'+str(address[1])))

data = client.recv(SIZE) # Receive stuff.
print 'Data received through socket port: {}'.format(data)

" Keep pushing Arduino data into the socket. "
try:
    currentMeasurement = "" # The most recent time step and the measured value formatted to be sent to the client.
    previousLine = "" # Previous line received from the Arduino. Will contain the time stamp or the measured value.
    while True: 
        line = arduinoSerial.readline()
        if ARDUINO_SENDING: # Only do this when Arduino is sending useful data. It will spit garbage before executing its setup method.
            if line.strip()=="T":
                currentMeasurement += previousLine # previousLine was the time stamp.
            elif line.strip()=="V":
                currentMeasurement += "-"+previousLine # This is the end of the measurement message, i.e. the value.
                client.sendall(currentMeasurement+',') # Add a comma to separate different values received from Arduino - protect against Net latency.
                previousLine="";currentMeasurement=""; # Reset to be ready for the next measurement.
                
        if line.strip()  == ARDUINO_HAIL_MSG: # Do this check here to avoid sending ARDUINO_HAIL_MSG when it comes in.
            ARDUINO_SENDING = True
        
        previousLine = line.strip()
        
except Exception as exc:
    print "Got an exception {}. Cleaning up the sockets and serial connection to the Arduino and quitting.".format(exc)    
    client.close() # Close this connection...
    serverSocket.close() #...the socket...
    arduinoSerial.close() ##...and the serial port.
    sys.exit()
except KeyboardInterrupt: # Quit quietly now, the user knows what they are doing.
    client.close() # Close this connection...
    serverSocket.close() #...the socket...
    arduinoSerial.close() ##...and the serial port.
    sys.exit()    