# -*- coding: utf-8 -*-
"""
Created on Sat Mar 28 18:13:07 2015

@author: alek
"""

import socket, sys

HOST = '192.168.0.7' # This will always work. 
HOST_IP = '192.168.0.10' # Local IP of my laptop.
HOST_EXT = '81.200.64.50' # The external IP of the router.
LOCALHOST = '127.0.1.1'
PORT = 50007 # Arbitrary non-privileged port
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # In case this socket was used before.
print 'Socket created'
 
#Bind socket to local host and port
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()
     
print 'Socket bound to host {} on port {}.'.format(HOST, PORT)
 
#Start listening on socket - max 10 connections.
s.listen(10)
print 'Socket now listening on port {}.'.format(PORT)
 
" Now keep talking with the client. "
#wait to accept a connection - blocking call
conn, addr = s.accept()
print 'Connected with ' + addr[0] + ':' + str(addr[1])

# Receive stuff.
data = conn.recv(1024)

# Send stuff back.
conn.sendall("""You are: {}.
And I am: {}.
You've sent: {}
Now fuck off.""".format(addr[0]+':'+str(addr[1]), "" , data))
 
conn.close() # Close this connection...
s.close() #...and the socket.