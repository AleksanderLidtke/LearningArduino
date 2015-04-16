#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Serial client that will connect to a socket server and listen to the data it sends.
These data will come from an Arduino and will be a mock-up saw-tooth wave. We will
plot them live on a matplotlib figure.

Created on Thu Apr 16 18:44:17 2015

@author: Alek & Artur
@version: 1.0.0
@since: 16 Apr 2015
"""

import socket, sys
import realTimePlot

HOST = 'localhost' # To keep it simple.
PORT = 5431 # A normally unused port.

clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.connect((HOST, PORT))
print "Client says hi with {}".format( clientSocket.recv(1024) ) # Print the received welcome hand-shaking message.
clientSocket.send("Hello") # Send a hand-shaking message ourselves to be polite.

with realTimePlot.realTimePlot(10,r'Voltage [$\mu$V]',nPtAv = 5, figName = 'Sensor 1 voltage') as rtp: # This will plot the data as it comes in.
	try:
		while True: # Keep listening to the data the server redirects from the Arduino.
			point = clientSocket.recv(1024) # Receive data which may contain a number of data points.
			point = [[float(v) for v in x.split('-')] for x in point.split(',') if x] # Separate the numbers received from the server ignoring empty strings and splitting time stamps from values.
			# Plot each of the received values.
			for t,v in point:
				rtp.update(t/1000.0,v) # Update the plot (remeber that t is in milliseconds).

	except Exception as exc:
		print "Got an exception {}. Cleaning up the sockets and serial connection to the Arduino and quitting.".format(exc)
		clientSocket.close() # Close this connection...
		sys.exit() #...and exit this program.

	except KeyboardInterrupt: # Quit quietly now, the user knows what they are doing.
		clientSocket.close() # Close this connection...
		sys.exit() #...and exit this program.
