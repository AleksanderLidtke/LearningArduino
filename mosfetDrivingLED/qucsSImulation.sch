<Qucs Schematic 0.0.18>
<Properties>
  <View=0,0,911,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=qucsSImulation.dat>
  <DataDisplay=qucsSImulation.dpl>
  <OpenDisplay=1>
  <Script=qucsSImulation.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Vdc V1 1 470 140 -52 -26 0 3 "9V" 1>
  <R R1 1 280 260 -26 15 0 0 "560 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <_MOSFET T1 1 470 260 8 -26 1 0 "nfet" 0 "1.0 V" 1 "2e-5" 1 "0.0" 0 "0.6 V" 0 "0.0" 1 "0.0 Ohm" 0 "0.0 Ohm" 0 "0.0 Ohm" 0 "1e-14 A" 0 "1.0" 0 "1 um" 0 "1 um" 0 "0.0" 0 "0.1 um" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0 F" 0 "0.0 F" 0 "0.8 V" 0 "0.5" 0 "0.5" 0 "0.0" 0 "0.33" 0 "0.0 ps" 0 "0.0" 0 "0.0" 0 "1" 0 "600.0" 0 "0.0" 0 "1" 0 "1" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0 m" 0 "0.0 m" 0 "0.0" 0 "1.0" 0 "1.0" 0 "26.85" 0 "26.85" 0>
  <R R2 1 460 410 15 -26 0 1 "560 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 470 70 0 0 0 2>
  <GND * 1 720 480 0 0 0 0>
  <Diode D1 1 630 480 -26 -83 0 2 "1e-15 A" 1 "1" 1 "10 fF" 1 "0.5" 0 "0.7 V" 0 "0.5" 0 "0.0 fF" 0 "0.0" 0 "2.0" 0 "0.0 Ohm" 0 "0.0 ps" 0 "0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0" 0 "1 mA" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <Vdc ArduinoP10 1 180 260 -26 18 0 0 "5 V" 1>
  <GND * 1 120 260 0 0 0 0>
  <.DC ArduinoPinV 1 110 390 0 46 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <.SW SW1 1 750 40 0 77 0 0 "ArduinoPinV" 1 "lin" 1 "ArduinoP10" 1 "0 V" 1 "5 V" 1 "51" 1>
</Components>
<Wires>
  <470 170 470 230 "MOSFETSource" 500 170 35 "">
  <310 260 440 260 "MOSFESTGate" 320 200 60 "">
  <470 290 470 380 "MOSFETDrain" 520 340 64 "">
  <460 380 470 380 "" 0 0 0 "">
  <470 70 470 110 "" 0 0 0 "">
  <460 440 460 480 "" 0 0 0 "">
  <460 480 600 480 "DiodeInput" 600 520 93 "">
  <660 480 720 480 "" 0 0 0 "">
  <210 260 250 260 "" 0 0 0 "">
  <120 260 150 260 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
