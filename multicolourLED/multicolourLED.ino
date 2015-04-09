int PIN_POWER = 10; // Pin to which the voltage is connected, always kept high.
int PIN_RED = 5; // Pin to which the red LED is connected.
int PIN_GREEN = 7;
int PIN_BLUE = 6;
int DURATION = 1000; // How long the LED will shine for, ms.
int PAUSE_DURATION = 500; // How much time to wait between consecutive blinks, ms.

int lightDiode(int pinNo, int duration, int pauseDuration)
{
  /* Light a diode connected to a pin for a specified duration by setting a pin low.
  @param pinNo - number of the pin where the diode is connecetd, will be set low.
  @param duration - for how long the diode is supposed to shine, in miliseconds
  @param pauseDuration - how long to wait after lighting the diode, ms
    */
  digitalWrite(pinNo, LOW); // Set the desired pin low...
  delay(duration); // ...wait...
  digitalWrite(pinNo, HIGH); // ...and we're done - back to high.
  delay(pauseDuration);
  return 0;
}

void setup(void)
{
  /* Called when Arduino starts up. */
  pinMode(PIN_POWER, OUTPUT); // This pin will be set high and low to blink the LED.
  digitalWrite(PIN_POWER, HIGH); // This is always kept high to deliver power to the diode.
  
  pinMode(PIN_RED, OUTPUT); // When any of these pins is set to low the current will flow through the diode.
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  digitalWrite(PIN_RED, HIGH); // Set these high by defualt so that there will be no voltage across the diode so that the current won't flow.
  digitalWrite(PIN_GREEN, HIGH);
  digitalWrite(PIN_BLUE, HIGH);
}

void loop(void)
{
  /* Called repeatedly whilst Arduino is active. */
  lightDiode(PIN_RED, DURATION, PAUSE_DURATION);
  lightDiode(PIN_GREEN, DURATION, PAUSE_DURATION);
  lightDiode(PIN_BLUE, DURATION, PAUSE_DURATION);
}
