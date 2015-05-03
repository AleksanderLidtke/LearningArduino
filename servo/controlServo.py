"""Contains:
    - servoControllerMainFrame class which implements the methods used by
      the GUI defined in servoControllerGUI.py inside mainFrame class
    - definition of the servoControllerApp which uses the GUI to run
      a stand-alone application"""

helpMsg = ( "Control servo v 1.0\n"
            "\n"
            "Uses Python Serial library to establish a connection with an Arduino "
            "via a seral USB port. This may be selected from the drop-down menu. If "
            "Arduino is reconnected to the PC the list of available ports may be "
            "refreshed using the Update button. Immediately after the connection is "
            "established the slider's position is transferred to the Arduino which "
            "then adjusts the servo position to match the set demand. Transfer may "
            "be paused at any time by pressing the Pause button. Pressing it again "
            "will resume the communication from the last known point.\n"
            "\n"
            "Copyright by Artur Lidtke & Aleksander Lidtke, 2015")

import os, sys, serial, glob
import wx
import servoControllerGUI

import matplotlib
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

# add custom modules to path
sys.path.append('../Python/')
import realTimePlot

# class derived from the base object - only implements event handlers
class servoControllermainFrame( servoControllerGUI.mainFrame ):
    
    def __init__( self, parent ):
        # some constants
        self.updateFrequency = 1 # Hz
        self.timeHistoryLength = 10 # second
        
        # initialise the underlying object
        servoControllerGUI.mainFrame.__init__( self, parent )
        
        # timing function which requires the plot to be updated cyclically
        self.updateTimer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.onUpdatePlot, self.updateTimer)        
        self.updateTimer.Start(int(1.0/self.updateFrequency*1000))
        
        # create the plot
        self.plot = realTimePlot.realTimePlot(self.timeHistoryLength,
                                'Servo angle demand [deg]', embeded = True,
                                margins = [0.22,0.9,0.95,0.27],
                                dataLineStyle = 'b-',
                                ywindowMargin = 0.1)
        
        self.figure = self.plot.fig
        self.axes= self.plot.ax
        self.axes.set_axis_bgcolor('white')
        self.figure.patch.set_facecolor((0.94, 0.94, 0.94))

        self.canvas = FigureCanvas(self, -1, self.figure)
        
        # add figure to the sizer and panel
        self.figSizer = wx.BoxSizer(wx.VERTICAL)
        self.figSizer.Add(self.canvas, 1, wx.ALL | wx.GROW)
        self.plotPanel.SetSizer(self.figSizer)
        self.plotPanel.Fit()
        
        # update the ports available at start-up
        self.updateActivePorts()
        self.portChoice.SetSelection(0)
        
        # variables associated with stored connection
        self.arduinoSerialConnection = 0
        self.portOpen = False
        self.currentPort = 'None'
        self.pause = False
        
        # comms protocol
        self.inputStartChar = '>'
        self.inputEndChar = ';'
        self.dataDelimiter = ','
        
    # Handlers for mainFrame events.
    def onClose( self, event ):
        if self.portOpen:
            self.arduinoSerialConnection.close()
        self.Destroy()
    
    def onSliderPositionChanged( self, event ):
        # TODO: Implement onSliderPositionChanged
        # is this required at all?
        pass
    
    def onChoseSerialPort( self, event ):
        # ignore the None option
        if self.portChoice.GetStringSelection() != 'None':
            try:
                # don't re-open a working stream
                if self.portChoice.GetStringSelection() != self.currentPort:
                    # close any open ports if present
                    if self.portOpen:
                        self.arduinoSerialConnection.close()
                    
                    self.arduinoSerialConnection = serial.Serial(self.portChoice.GetStringSelection(),
                                                                 19200, timeout = 2)
                    
                    if self.checkConnection():
                        self.portOpen = True
                        self.currentPort = self.portChoice.GetStringSelection()
                        
                        # set the initial state
                        self.updateState()
                          
            except:
                wx.MessageBox('Unknown problem occurred while establishing connection using the chosen port!', 'Error', 
                          wx.OK | wx.ICON_ERROR)
                self.arduinoSerialConnection = 0
                self.portOpen = False
        
        # if None is chosen then close the current port and flush the data stored in the plot
        else:
            if self.portOpen:
                self.arduinoSerialConnection.close()
            self.arduinoSerialConnection = 0
            self.portOpen = False
            self.currentPort = 'None'
            self.plot.flush()
            
    def onPause( self, event ):
        self.pause = not self.pause
    
    def onHelp( self, event ):
        self.printHelp()
    
    def onUpdatePlot( self, event ):
        self.updateState()
        
    def onUpdatePorts( self, event ):
        self.updateActivePorts()

    #=================================
    # non-event funtion declarations

    def checkConnection( self ):
        """ Checks if the Arduino is still connected. """
        
        testMsgGood = True
        try:
            self.arduinoSerialConnection.inWaiting()
        except:
            testMsgGood = False
        
        if not self.arduinoSerialConnection or not self.arduinoSerialConnection.readable() or not testMsgGood:
            wx.MessageBox('Arduino isn\'t readable! Check the connection...', 'Error', 
                  wx.OK | wx.ICON_ERROR)
            
            # close the connection
            self.arduinoSerialConnection.close()
            self.arduinoSerialConnection = 0
            self.portOpen = False
            self.currentPort = 'None'
            
            # check what ports are open - will set choice as None if current port has been lost
            self.updateActivePorts()
            
            return False
        else:
            return True
    
    def updateState ( self ):
        """ Update the plot and send a message to Arduino """
        if self.portOpen and not self.pause:
            # make sure the connection has not been broken
            if self.checkConnection():
                # send the message
                self.arduinoSerialConnection.write(self.inputStartChar + 'deltaServo'
                    + self.dataDelimiter + str(self.demandSlider.GetValue()+90) + self.inputEndChar)
                
                # store the data point in the plot
                # distinguish between first and subsequent messages
                if len(self.plot.datat) == 0:
                    self.plot.update( 0 + int(1.0/self.updateFrequency),
                                 self.demandSlider.GetValue() )
                                 
                else:
                    self.plot.update( self.plot.datat[-1] + int(1.0/self.updateFrequency),
                                 self.demandSlider.GetValue() )
                             
    def updateActivePorts( self ):
        """ Checks the list of open serial ports and updates the internal list
        and the options shown in the dropdown selection menu. """
        
        # find the open ports - main part of the code from:
        # http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
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
        
        # save current selection
        currentSelection = self.portChoice.GetStringSelection()
        
        # Remove the current options
        for i in range(len(self.portChoice.GetStrings())-1,-1,-1):
            self.portChoice.Delete(i)

        # add the newly found ports
        self.portChoice.Append('None')
        for port in ports:
            self.portChoice.Append(port)
            
        # attempt to return to the last selected port, use None if it's not found
        if currentSelection in ports:
            for i in range(len(ports)):
                if ports[i] == currentSelection:
                    self.portChoice.SetSelection(i+1)
        else:
            self.portChoice.SetSelection(0)
            self.currentSelection = 'None'
    
    def printHelp( self ):
        """ Print a help text in a pop-up window """
        
        wx.MessageBox(helpMsg, 'Help', wx.OK | wx.ICON_INFORMATION)

# implements the GUI class to run a wxApp
class servoControllerApp(wx.App):
    def OnInit(self):
        self.frame = servoControllermainFrame(None)
        self.SetTopWindow(self.frame)
        self.frame.Show(True)
        return True

if __name__ == "__main__":
    # need an environment variable on Ubuntu to make the menu barsh show correctly
    env = os.environ
    if not(('UBUNTU_MENUPROXY' in env) and (env['UBUNTU_MENUPROXY'] == 0)):
        os.environ["UBUNTU_MENUPROXY"] = "0"
            
    app = servoControllerApp()
    app.MainLoop()