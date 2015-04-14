v 20130925 2
C 40000 40000 0 0 0 title-B.sym
U 48600 46100 48600 41900 10 1
{
T 48300 46300 5 10 1 0 0 0 1
Vgnd=0
}
C 44400 41200 1 0 0 connector16-1.sym
{
T 46300 46100 5 10 0 0 0 0 1
device=CONNECTOR_16
T 44500 46300 5 10 1 2 0 0 1
ArduinoUno=0
}
T 45100 41800 9 10 1 0 0 0 1
GND
T 45100 41500 9 10 1 0 0 0 1
AREF
N 46200 41800 48600 41800 4
N 48600 41800 48600 41900 4
T 51200 40900 8 10 1 1 0 0 1
TITLE=UOS3 antenna depolyment mechanism test circuit
T 50000 40400 8 10 1 1 0 0 1
FILE=UOS3AntennaDeploymentTest
T 51500 40100 8 10 1 1 0 0 1
PAGES=1
T 50000 40100 8 10 1 1 0 0 1
PAGE=1
T 53900 40400 8 10 1 1 0 0 1
REVISION=1.0.0
T 53900 40100 8 10 1 1 0 0 1
BY=Aleksander Lidtke
C 55700 44700 1 0 0 battery-1.sym
{
T 56000 45600 5 10 0 0 0 0 1
device=BATTERY
T 55900 45200 5 10 1 1 0 0 1
refdes=B1
T 56000 46000 5 10 0 0 0 0 1
symversion=0.1
T 55700 44400 5 10 1 0 0 0 1
Vbat=5
}
C 52200 43400 1 0 0 mosfet-with-diode-1.sym
{
T 53100 43900 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 53100 43900 5 10 1 1 0 0 1
refdes=PSMN1R9-40PL
}
C 54400 44800 1 0 0 resistor-2.sym
{
T 54800 45150 5 10 0 0 0 0 1
device=RESISTOR
T 54700 45100 5 10 1 1 0 0 1
refdes=R3
T 54600 44600 5 10 1 0 0 0 1
R=6.8
}
N 52800 43400 52800 42400 4
N 48600 45500 56500 45500 4
N 52800 44900 54400 44900 4
N 52800 44400 52800 44900 4
N 55700 44900 55300 44900 4
N 56400 44900 56500 44900 4
N 56500 44900 56500 45500 4
N 48600 47100 48600 49100 4
C 47300 48000 1 0 0 resistor-2.sym
{
T 47700 48350 5 10 0 0 0 0 1
device=RESISTOR
T 47600 48300 5 10 1 1 0 0 1
refdes=R2
T 47500 47700 5 10 1 0 0 0 1
R=560
}
N 48200 48100 48600 48100 4
C 49300 48900 1 0 0 diode-1.sym
{
T 49700 49500 5 10 0 0 0 0 1
device=DIODE
T 49600 49400 5 10 1 1 0 0 1
refdes=Dred
T 49500 48700 5 10 1 0 0 0 1
Vd=2?
}
C 49300 47900 1 0 0 diode-1.sym
{
T 49700 48500 5 10 0 0 0 0 1
device=DIODE
T 49600 48400 5 10 1 1 0 0 1
refdes=Dblue
T 49500 47700 5 10 1 0 0 0 1
Vd=2?
}
C 49300 46900 1 0 0 diode-1.sym
{
T 49700 47500 5 10 0 0 0 0 1
device=DIODE
T 49600 47400 5 10 1 1 0 0 1
refdes=Dgreen
T 49500 46700 5 10 1 0 0 0 1
Vd=2?
}
N 48600 48100 49300 48100 4
N 48600 47100 49300 47100 4
N 50200 47100 51000 47100 4
N 50600 49100 50200 49100 4
N 49300 49100 48600 49100 4
N 52200 43300 52200 44400 4
N 48600 42400 52800 42400 4
T 45100 43300 5 10 1 0 0 0 1
Vs=5
T 51400 43800 5 10 1 0 0 0 1
Vgs=3.3
N 46200 43600 47200 43600 4
N 47200 43600 47200 48100 4
N 50600 49100 50600 50000 4
N 47000 50000 50600 50000 4
N 47000 50000 47000 43900 4
N 50200 48100 50800 48100 4
N 50800 48100 50800 50200 4
N 50800 50200 46800 50200 4
N 46800 50200 46800 44200 4
N 51000 47100 51000 50400 4
N 51000 50400 46600 50400 4
N 46600 50400 46600 44500 4
N 46600 44500 46200 44500 4
N 46200 44200 46800 44200 4
N 47000 43900 46200 43900 4
N 47300 48100 47200 48100 4
T 45100 43600 5 10 1 0 0 0 1
Vs=5
N 51900 44400 52200 44400 4
N 46200 43300 51000 43300 4
C 51000 43100 1 0 0 capacitor-2.sym
{
T 51200 43800 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 51300 43600 5 10 1 1 0 0 1
refdes=C1
T 51200 44000 5 10 0 0 0 0 1
symversion=0.1
}
C 51900 44600 1 180 0 capacitor-2.sym
{
T 51700 43900 5 10 0 0 180 0 1
device=POLARIZED_CAPACITOR
T 51600 44800 5 10 1 1 180 0 1
refdes=2C1
T 51700 43700 5 10 0 0 180 0 1
symversion=0.1
}
N 48600 44400 51000 44400 4
N 51900 43300 52200 43300 4
