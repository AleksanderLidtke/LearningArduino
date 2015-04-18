#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Serial client that will connect to a laptop to see if it works.

Created on Sat Apr 18 21:50:17 2015

@author: Alek & Artur
@version: 1.0.0
@since: 18 Apr 2015
"""

import socket, sys

HOST = '192.168.0.7' # To keep it simple.
PORT = 50007#8000#5431 # A normally unused port.

clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientSocket.connect((HOST, PORT))

clientSocket.send("Hello") # Send a hand-shaking message ourselves to be polite.

print "Client says hi with {}".format( clientSocket.recv(1024) ) # Print the received welcome hand-shaking message.
