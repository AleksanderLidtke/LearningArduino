# -*- coding: utf-8 -*-
"""
Created on Fri Feb  6 22:09:19 2015

@author: artur
"""

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

def nPointAverage(x,y,N):
    """ Take n-point average of the data.
    Parameters
    ----------
        @param x - independent variable vector
        @param y - dependent variable vector
        @param N - width of the averaging interval"""
        
    X,Y = np.zeros(len(x)-(N-1)),np.zeros(len(x)-(N-1))
    
    for i in range(X.shape[0]):
        X[i] = x[i+N//2]
        for j in range(N):
            Y[i] += y[i+j-N//2]
        Y[i] /= N
    
    return X,Y

#===============
class realTimePlot(object):
    """
    Create the plot using the constructor and then call the update method whenever
    new set of data is available.
    """
    def __init__(self,timeExtent, signalName, nPtAv = -1, figName = [], embeded = False):
        """ Parameters
        --------------
            @param timeExtent - width of the time trace being shown [s]
            @param signalName - name of the signal used as y-axis label
            @param nPtAv - (optional) width of the n-point moving average
                applied to the data; leave -1 to disable (default)
            @param figName - (optional) name of the figure window, same as
                signalName by default
            @param embeded - (optional) whether plot is embeded in a gui or
                elsewhere - will not call pyplot.show() at the end of __init__;
                default False for stand-alone running
        """
        self.timeExtent = timeExtent
        self.signalName = signalName
        self.nPtAv = nPtAv
        if len(figName) > 0:
            self.figName = figName
        else:
            self.figName = signalName
        
        # disable the legend and set a small top margin
        if self.nPtAv < 0:
            self.topMargin = 0.95
        else:
            self.topMargin = 0.88
            
        # default font and line controls
        self.labelfontsize = 16
        self.plotlinewidth = 2
        self.borderlinewidth = 2
        self.ticklength = 5
        matplotlib.rc('xtick', labelsize=self.labelfontsize) 
        matplotlib.rc('ytick', labelsize=self.labelfontsize)
        
        # create the plot axes, set up labels, etc.
        self.fig, self.ax = plt.subplots(figsize=(9,5))
        plt.subplots_adjust(left=0.1, right=0.98, top=self.topMargin, bottom=0.25)
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
        
        # dummy lines to put in the legend - the other ones keep getting re-drawn
        if self.nPtAv > -1:
            self.dummyLine = matplotlib.lines.Line2D([],[],c='k',ls='-',lw=self.plotlinewidth)
            self.dummyLineNptAv = matplotlib.lines.Line2D([],[],c='r',ls='-',lw=self.plotlinewidth)
            self.legend = self.fig.legend([self.dummyLine,self.dummyLineNptAv],
                                ['Signal','{} point average'.format(self.nPtAv)],
                                ncol =2,prop={'size':self.labelfontsize},loc = 'upper right',
                                bbox_to_anchor=(1,1))
        
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
        try:
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
            if len(self.datat) == 1:
                ywindow = ((self.datay[0]-1,self.datay[0]+1))
            else:
                ywindow = ((min(self.yplot),max(self.yplot)))
            
            # remove the line from previous iteration - do it just before plotting
            # the new one to keep things smooth for the eye
            self.line.pop(0).remove()
            
            # plot the line and set axes extents
            self.line = self.ax.plot(self.tplot,self.yplot,'k',linewidth=self.plotlinewidth)
            self.ax.set_xlim(twindow)
            self.ax.set_ylim(ywindow)
            
            # compute 5-point average of the signal to smooth out the noise
            if self.nPtAv > -1 and len(self.tplot) > (self.nPtAv-1):
                self.lineNpt.pop(0).remove()
                tNpt,yNpt = nPointAverage(self.tplot,self.yplot,self.nPtAv)
                self.lineNpt = self.ax.plot(tNpt,yNpt,'r-',linewidth=self.plotlinewidth)
                
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
            plt.draw()
            
            # update the qt GUI if necessary and wait for the next set of data
            if SPYDER: QApplication.processEvents()
            
        except Exception as e: print type(e).__name__,':',e

#=======================
# VALIDATION - ONLY USED WHEN THIS FILE IS EXECUTED AS A STANDALONE SCRIPT
if __name__ == '__main__':
    
    # dummy sampling function
    import datetime
    
    # check the start time - only needed for the dummy data function
    tStart = datetime.datetime.now()
    
    def getDataPoint(tStart,tNow,f=1):
        # time in seconds from the start of the simulation
        t = (tNow-tStart).seconds+(tNow-tStart).microseconds/1e6
        # sine-wave with random noise
        return t,(5*np.sin(2*np.pi*f*t)+ # main signal
                  2*(np.random.rand(1)-0.5)+ # random noise (mean at y=0)
                  np.random.rand(1)*np.sin(2*np.pi*100*t)+ # high-frequency content with random amplitude
                  np.random.rand(1)*np.sin(2*np.pi*250*t))
    #=======================
            
    # sampling frequency [Hz]
    fSample = 100
    
    with realTimePlot(10,r'Voltage [$\mu$V]',nPtAv = 5, figName = 'Sensor 1 voltage') as rtp:
        while True:
            try:
                # get the new data point and store in the main array
                t,y = getDataPoint(tStart,datetime.datetime.now())
                rtp.update(t,y)
                time.sleep(1./fSample)
            except KeyboardInterrupt:
                break

