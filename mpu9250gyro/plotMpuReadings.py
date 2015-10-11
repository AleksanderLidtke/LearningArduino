# -*- coding: utf-8 -*-
"""
Created on Fri Feb  6 22:09:19 2015

@author: Artur Lidtke
"""

# needed for serial comms
import sys, serial, glob

# needed for plotting
# in Spyder will need to call a gui-update function as well as update matplotlib
# whenever plot is to be re-drawn
try:
    from PyQt4.QtGui import QApplication
    SPYDER = True
except ImportError:
    SPYDER = False

import matplotlib
import matplotlib.lines
import matplotlib.pyplot as plt
import numpy as np
import time
import datetime

#=======================================
# serial communication protocol
#=======================================

# constants for dfinig the format of a data packet
INPUT_START_CHAR = '>'
MESSAGE_END_CHAR = ';'
DATA_DELIMITER = ','

def getActivePorts():
    """ find the open ports - main part of the code from:
    http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
    returns a list of open ports using their string representation, e.g. /dev/ttyACM0
    """
    if sys.platform.startswith('win'):
        candidatePorts = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        candidatePorts = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        candidatePorts = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    ports = []
    for port in candidatePorts:
        try:
            s = serial.Serial(port)
            s.close()
            ports.append(port)
        except (OSError, serial.SerialException):
            pass
    
    return ports

def readMessage(s):
    """ Decode an input string into a set of keywords and values
    Arguments
    ---------
        @param s - string representation of an input message
    Returns
    ---------
        @param readings - dictionary of key-value pairs for each sensor reading
    """
    try:
        if s:
            s = s.strip()
            if s[0] == INPUT_START_CHAR and s[-1] == MESSAGE_END_CHAR:
                # split into values and keys
                s = s.lstrip(INPUT_START_CHAR).rstrip(MESSAGE_END_CHAR).split(DATA_DELIMITER)
    
                readings = {}
                try:
                    # concentrate into a dictionary
                    for iKey in range(0,len(s),2):
                        readings[s[iKey]] = s[iKey+1]
                except IndexError:
                    pass
                return readings
    except:
        pass
    return {}

#===============
class realTimePlot(object):
    """
    Create the plot using the constructor and then call the update method whenever
    new set of data is available.
    """
    def __init__(self,timeExtent, signalName, figName = [], figSize = (6,5),
                 margins=[0.1,0.98,0.95,0.25],embeded = False, dataLineStyle = 'k-',
                ywindowMargin = 0.1):
        """ Parameters
        --------------
            @param timeExtent - width of the time trace being shown [s]
            @param signalName - name of the signal used as y-axis label
            @param figName - (optional) name of the figure window, same as
                signalName by default
            @param figSize - (optional) size of the Figure, default (6,5)
            @param margins - (optional) margins of the adjust_subplot() method,
                default [0.1,0.98,0.95,0.25] (left, right, top, bottom)
            @param embeded - (optional) whether plot is embeded in a gui or
                elsewhere - will not call pyplot.show() at the end of __init__;
                default False for stand-alone running
            @param dataLineStyle - (optional) line style and colour, def. 'k-'
            @param ywindowMaring - (optional) adjust y-axis extents using the
                specified fraction of the mean value of the signal, def. 0.1
        """
        self.margins = margins
        self.timeExtent = timeExtent
        self.signalName = signalName
        self.dataLineStyle = dataLineStyle
        self.ywindowMargin = ywindowMargin

        if len(figName) > 0:
            self.figName = figName
        else:
            self.figName = signalName
            
        # default font and line controls
        self.labelfontsize = 16
        self.plotlinewidth = 2
        self.borderlinewidth = 2
        self.ticklength = 5
        matplotlib.rc('xtick', labelsize=self.labelfontsize) 
        matplotlib.rc('ytick', labelsize=self.labelfontsize)
        
        # create the plot axes, set up labels, etc.
        self.fig, self.ax = plt.subplots(figsize=figSize)
        plt.subplots_adjust(left=self.margins[0], right=self.margins[1], top=self.margins[2], bottom=self.margins[3])
        self.fig.canvas.set_window_title(self.figName)
        self.ax.tick_params(axis='both',reset=False,which='both',
                            length=self.ticklength,width=self.borderlinewidth)
        for spine in ['top', 'right','bottom','left']:
            self.ax.spines[spine].set_linewidth(self.borderlinewidth)
        self.ax.set_xlabel('Ellapsed time [s]',fontsize=self.labelfontsize)
        self.ax.set_ylabel(self.signalName,fontsize=self.labelfontsize)
        
        # create the relative time axis
        self.reltimeax = self.ax.twiny()
        self.reltimeax.set_frame_on(True)
        self.reltimeax.patch.set_visible(False)
        self.reltimeax.xaxis.set_ticks_position('bottom')
        self.reltimeax.xaxis.set_label_position('bottom')
        self.reltimeax.spines['bottom'].set_position(('outward', 50))
        self.reltimeax.set_xlabel('Time history [s]',fontsize=self.labelfontsize)
        self.reltimeax.tick_params(axis='both',reset=False,which='both',
                                   length=self.ticklength,width=self.borderlinewidth)
        self.reltimeax.spines['bottom'].set_linewidth(self.borderlinewidth)
        
        # data arrays where the new sampling points are added at each update of the sensor
        self.datat,self.datay,self.tplot,self.yplot = [],[],[],[]
        
        # dummy line objects to start the loop
        self.line,self.lineNpt = self.ax.plot([],'k-'),self.ax.plot([],'r-')
        
        # set the plot to interactive mode and show without halting the execution
        plt.ion()
        if not(embeded):
            plt.show(block=False)
        
    def __enter__(self):
        """ Required for the object to be used inside a with statement """
        return self
        
    def __exit__(self, type, value, traceback):
        """ Clean-up when required by the with statement """
        plt.close(self.fig)
        
    def update(self,t,y):
        """ Add a new data point to the plot
        Parameters
        ----------
            @param t - new time value
            @param y - new dependent variable value
        """
        self.datat.append(t)
        self.datay.append(y)
        
        # at the beginning of the sampling period do not adjust the time scale
        if max(self.datat) < self.timeExtent:
            self.tplot = self.datat
            self.yplot = self.datay
            # ellapsed time
            twindow = (0,self.timeExtent)
            
        # once more data is collected only focus on the points within the specified
        # window, keep the old data in memory but do not show
        else:
            # check how many oldest point from the shown record to remove
            # (for constant sampling rate it should always be just the oldest point,
            # will vary a little bit due to system performance, generally i \in [0-2])
            for i in range(len(self.tplot)):
                if (self.datat[-1]-self.tplot[i]) < self.timeExtent:
                    self.tplot = self.tplot[i:] + [self.datat[-1]]
                    self.yplot = self.yplot[i:] + [self.datay[-1]]
                    twindow = ((self.tplot[0],self.tplot[-1]))
                    break

        # relative time w.r.t. the last measured data point
        trel = [(v-self.tplot[-1]) for v in self.tplot]

        # at the beginning of the sampling period need to avoid having a zero-length y-axis
        self.ax.get_yaxis().get_major_formatter().set_useOffset(False)

        if len(self.datat) == 1:
            ywindow = ((self.datay[0]-1,self.datay[0]+1))
        # check if the data itself is not a stream of zeroes
        elif (np.abs(min(self.yplot)-max(self.yplot)) < 1e-6) and np.abs(min(self.yplot)) < 1e-6:
            self.ax.get_yaxis().get_major_formatter().set_useOffset(True)
            ywindow = ((-1e-3-self.ywindowMargin*np.abs(min(self.yplot)),
                        1e-3+self.ywindowMargin*np.abs(max(self.yplot))))
        else:
            ywindow = ((min(self.yplot)-self.ywindowMargin*np.abs(np.mean(self.yplot)),
                        max(self.yplot)+self.ywindowMargin*np.abs(np.mean(self.yplot))))
        
        # remove the line from previous iteration - do it just before plotting
        # the new one to keep things smooth for the eye
        self.line.pop(0).remove()
        
        # plot the line and set axes extents
        self.line = self.ax.plot(self.tplot,self.yplot,self.dataLineStyle,linewidth=self.plotlinewidth)
        self.ax.set_xlim(twindow)
        self.ax.set_ylim(ywindow)
            
        # plot on the relative time axis and remove the line immediately
        # to only have one of them appear
        linerel = self.reltimeax.plot(trel, self.yplot, 'r-')
        linerel.pop(0).remove()
        
        # if time is sufficiently advanced fix the time scale to avoid small
        # oscillation of the tick locations
        if self.tplot[-1] > self.timeExtent:
            self.reltimeax.set_xlim((-self.timeExtent,0))
        else:
            self.reltimeax.set_xlim((twindow[0]-self.tplot[-1],twindow[-1]-self.tplot[-1]))
        
        # update the plot
        self.fig.canvas.draw()
        
        # update the qt GUI if necessary and wait for the next set of data
        if SPYDER: QApplication.processEvents()
            
    def flush(self):
        """ Clear all stored data and prepare to re-accept a new stream of information """
        self.datat,self.datay,self.tplot,self.yplot = [],[],[],[]
        self.line.pop(0).remove()
        self.line,self.lineNpt = self.ax.plot([],'k-'),self.ax.plot([],'r-')
        self.fig.canvas.draw()

#=======================================
# main part of the code
#=======================================

# open the serial connection (assume Arduino is hooked-up to the 0th port and working)
arduinoSerialConnection = serial.Serial(getActivePorts()[0], 19200, timeout = 2)

# clear the buffer to discard any stuff already in it
arduinoSerialConnection.flush()

# which signal to show
readingToPlot = 'GX'

# sampling frequency [Hz]
fSample = 100.

# check the start time - only needed for the dummy data function
tStart = datetime.datetime.now()

# create a real time plot object, make sure it gets cleaned up when the app closes
with realTimePlot(100, 'Test reading', figName = [], figSize = (6,5),
                 margins=[0.15,0.95,0.9,0.3],embeded = False, dataLineStyle = 'k-',
                ywindowMargin = 0.1) as dataPlot:
    while True:
        try:
            # get the most recent line from the serial port
            line = arduinoSerialConnection.readline()
            
            # pass on to the parser to get a dict of key-value pairs corresponding to each parameter
            readings = readMessage(line)
            
            print readings
            
            # sometimes stuff gets lost over serial, make sure to check the inputs before
            # trying to use them
            if readings and readingToPlot in readings:
                # current time
                tNow = datetime.datetime.now()
                t = (tNow-tStart).seconds+(tNow-tStart).microseconds/1e6
                
                # update the plot
                dataPlot.update(t,float(readings[readingToPlot]))
                
                # wait for the next update cycle
                time.sleep(1./fSample)

        except KeyboardInterrupt:
            break

