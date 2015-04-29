# -*- coding: utf-8 -*-

import sys, serial, time

def send(cmd, port):
    for char in cmd:
        port.write(char)
        print "sent", char

arduinoSerial=serial.Serial('/dev/ttyACM4', 19200)#, timeout = 2)

if not arduinoSerial.readable():
    raise IOError("Arduino isn't readable! Check the connection.")
    sys.exit()

#send('/a,',arduinoSerial)
arduinoSerial.write('$a!')
#arduinoSerial.write('a')
#arduinoSerial.write(',')
#ledHigh = '1'
#ledLow = '0'

#while True:
#    arduinoSerial.write(ledHigh)
#    arduinoSerial.write(b'/')
#    time.sleep(1)
#    arduinoSerial.write(ledLow)
#    arduinoSerial.write(b'/')
#    time.sleep(1)
#    print "Done loop"