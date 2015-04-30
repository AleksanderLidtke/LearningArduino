# -*- coding: utf-8 -*-

import sys, serial, time
import numpy as np

arduinoSerial=serial.Serial('/dev/ttyACM0', 19200)

if not arduinoSerial.readable():
    raise IOError("Arduino isn't readable! Check the connection.")
    sys.exit()

inputStartChar = '>'
inputEndChar = ';'
dataDelimiter = ','

t = 0

while True:
    # compute the angle demand
    delta = np.sin(2.0*np.pi*0.05*t) * 90+90
    print delta
    
    arduinoSerial.write(inputStartChar + 'deltaServo' + dataDelimiter + str(delta) + inputEndChar)
    time.sleep(1)
    t += 1