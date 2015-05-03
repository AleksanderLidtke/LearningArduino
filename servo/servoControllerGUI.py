# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Jun  6 2014)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc

###########################################################################
## Class mainFrame
###########################################################################

class mainFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Servo controller", pos = wx.DefaultPosition, size = wx.Size( 500,570 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 300,570 ), wx.DefaultSize )
		
		mainFrameSizer = wx.BoxSizer( wx.VERTICAL )
		
		self.plotPanel = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.plotPanel.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_WINDOW ) )
		
		mainFrameSizer.Add( self.plotPanel, 1, wx.EXPAND|wx.ALL, 5 )
		
		self.frameHorDividerLine = wx.StaticLine( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		mainFrameSizer.Add( self.frameHorDividerLine, 0, wx.EXPAND |wx.ALL, 5 )
		
		self.controlPanel = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.controlPanel.SetMinSize( wx.Size( -1,120 ) )
		
		controlPanerHorSizer = wx.BoxSizer( wx.HORIZONTAL )
		
		demandSliderVertSizer = wx.BoxSizer( wx.VERTICAL )
		
		self.demandSliderLabel = wx.StaticText( self.controlPanel, wx.ID_ANY, u"Angle demand [degrees]", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.demandSliderLabel.Wrap( -1 )
		demandSliderVertSizer.Add( self.demandSliderLabel, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.demandSlider = wx.Slider( self.controlPanel, wx.ID_ANY, 0, -90, 90, wx.DefaultPosition, wx.DefaultSize, wx.SL_LABELS )
		demandSliderVertSizer.Add( self.demandSlider, 1, wx.ALIGN_BOTTOM|wx.ALL|wx.EXPAND, 5 )
		
		
		controlPanerHorSizer.Add( demandSliderVertSizer, 1, wx.ALIGN_CENTER_VERTICAL|wx.ALL|wx.EXPAND, 5 )
		
		buttonAndDropdownVertSizer = wx.BoxSizer( wx.VERTICAL )
		
		self.portChoiceLabel = wx.StaticText( self.controlPanel, wx.ID_ANY, u"Serial port:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.portChoiceLabel.Wrap( -1 )
		buttonAndDropdownVertSizer.Add( self.portChoiceLabel, 0, wx.ALL|wx.EXPAND, 5 )
		
		portChoiceChoices = []
		self.portChoice = wx.Choice( self.controlPanel, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, portChoiceChoices, 0 )
		self.portChoice.SetSelection( 0 )
		buttonAndDropdownVertSizer.Add( self.portChoice, 0, wx.ALIGN_BOTTOM|wx.ALIGN_RIGHT|wx.ALL|wx.EXPAND, 5 )
		
		buttonHorSizer = wx.BoxSizer( wx.HORIZONTAL )
		
		self.pauseButton = wx.Button( self.controlPanel, wx.ID_ANY, u"Pause", wx.DefaultPosition, wx.DefaultSize, 0 )
		buttonHorSizer.Add( self.pauseButton, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.updatePortsButton = wx.Button( self.controlPanel, wx.ID_ANY, u"Update ports", wx.DefaultPosition, wx.DefaultSize, 0 )
		buttonHorSizer.Add( self.updatePortsButton, 0, wx.ALL|wx.EXPAND, 5 )
		
		
		buttonAndDropdownVertSizer.Add( buttonHorSizer, 1, wx.EXPAND, 5 )
		
		
		controlPanerHorSizer.Add( buttonAndDropdownVertSizer, 0, wx.ALL|wx.EXPAND, 5 )
		
		
		self.controlPanel.SetSizer( controlPanerHorSizer )
		self.controlPanel.Layout()
		controlPanerHorSizer.Fit( self.controlPanel )
		mainFrameSizer.Add( self.controlPanel, 0, wx.ALL|wx.EXPAND, 5 )
		
		
		self.SetSizer( mainFrameSizer )
		self.Layout()
		self.mainFrameMenuBar = wx.MenuBar( 0 )
		self.fileMenu = wx.Menu()
		self.exitItem = wx.MenuItem( self.fileMenu, wx.ID_ANY, u"Exit", wx.EmptyString, wx.ITEM_NORMAL )
		self.fileMenu.AppendItem( self.exitItem )
		
		self.mainFrameMenuBar.Append( self.fileMenu, u"File" ) 
		
		self.helpMenu = wx.Menu()
		self.helpItem = wx.MenuItem( self.helpMenu, wx.ID_ANY, u"Help", wx.EmptyString, wx.ITEM_NORMAL )
		self.helpMenu.AppendItem( self.helpItem )
		
		self.mainFrameMenuBar.Append( self.helpMenu, u"Help" ) 
		
		self.SetMenuBar( self.mainFrameMenuBar )
		
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.onClose )
		self.demandSlider.Bind( wx.EVT_SCROLL, self.onSliderPositionChanged )
		self.portChoice.Bind( wx.EVT_CHOICE, self.onChoseSerialPort )
		self.pauseButton.Bind( wx.EVT_BUTTON, self.onPause )
		self.updatePortsButton.Bind( wx.EVT_BUTTON, self.onUpdatePorts )
		self.Bind( wx.EVT_MENU, self.onClose, id = self.exitItem.GetId() )
		self.Bind( wx.EVT_MENU, self.onHelp, id = self.helpItem.GetId() )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def onClose( self, event ):
		event.Skip()
	
	def onSliderPositionChanged( self, event ):
		event.Skip()
	
	def onChoseSerialPort( self, event ):
		event.Skip()
	
	def onPause( self, event ):
		event.Skip()
	
	def onUpdatePorts( self, event ):
		event.Skip()
	
	
	def onHelp( self, event ):
		event.Skip()
	

