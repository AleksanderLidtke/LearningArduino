/* Test the antenna deployment of the University of Southampton Small Satellite,
UoS3 ( www.blog.soton.ac.uk/uos3/ ). This mechanisms holds down the antennae
using a piece of Kapton tape and a plastic wire. Upon deployment in space the
on-board computer boots up the first time and activates a switch that runs current
through a resistor. The resistor heats up, burns the wire and the antennae spring out.

This script is used to simulate this behaviour using an Arduino as a proxy of the
on-board computer.

@author: Alek Lidtke
@email: al11g09@soton.ac.uk
@version: 1.0.0
@since: 11 Apr 2015
*/

int PIN_NO = 10; // Pin to which the MOSFET is connected.
int FULL_CURRENT_DURATION = 1000; // How long to keep the current flowing through the resistor.
int INITIAL_PAUSE_DURATION = 10*60*1000; // How much time to wait after the Arduino starts up, ms.
bool FIRST_BOOT_UP = true; // Whether this is the first time the Arduino boots and enters the main loop, used to simulate the first boot-up of UoS3.

void setup(void)
{
  /* Called when Arduino starts up. Prepares all the output pins and waits
	a certain time to allow the Arduino to be placed in a vacuum chamber after
	the mechanism has been assembled and armed.*/
	pinMode(PIN_NO, OUTPUT); // This pin will be set high and low to blink the LED.
	digitalWrite(PIN_NO, LOW); // Initialise at no voltage between the pin and the ground of the Arduino that is connected to the battery ground.
	delay(INITIAL_PAUSE_DURATION); // Give people time to put the Arduino and the deployment mechanism in the vacuum chamber and prepare.
}

void loop(void)
{
  /* Called repeatedly whilst Arduino is active. 
	Will check whether this is the first time this function is called. If so it
	will run current through the pin that's connected to a MOSFET and thus run
	current through the antenna deployment resistor.*/
	if( FIRST_BOOT_UP ){ // Are we booting the first time? If so need to deploy antennae.
		FIRST_BOOT_UP = false; // We've booted already.
		digitalWrite(PIN_NO, HIGH); // Burn the wire. This should also turn on a diode.
		delay(FULL_CURRENT_DURATION); // Wait until the wire has burnt.
		digitalWrite(PIN_NO, LOW); // And we're done!
	}
//TODO: could add a diode to the circuit that will be switched on when the computer is done, e.g. use the multi-colour diode for this.
}
