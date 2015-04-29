#!/usr/bin/python

from numpy import arange, sin, pi
import matplotlib
matplotlib.use('WXAgg')

from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx
from matplotlib.figure import Figure

import sys

# add custom modules to path
sys.path.append('./Python/')

import realTimePlot

import wx

class CanvasPanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        self.plot = realTimePlot.realTimePlot(10.0,'Voltage', embeded = True)
        
#        self.figure = Figure()
#        self.axes = self.figure.add_subplot(111)
        
        self.figure = self.plot.fig
        self.axes= self.plot.ax
        
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.GROW)
        self.SetSizer(self.sizer)
        self.Fit()

#    def draw(self):
#        t = arange(0.0, 3.0, 0.01)
#        s = sin(2 * pi * t)
#        self.axes.plot(t, s)


if __name__ == "__main__":
    app = wx.PySimpleApp()
    fr = wx.Frame(None, title='test')
    panel = CanvasPanel(fr)
    
    panel.draw()

    fr.Show()
    app.MainLoop()