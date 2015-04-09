int PIN_NO = 10; // Pin to which the LED is connected.
int DURATION = 5000; // How long the LED will shine for, ms.
int PAUSE_DURATION = 5000; // How much time to wait between consecutive blinks, ms.

int lightDiode(int pinNo, int duration, int pauseDuration)
{
  /* Light a diode connected to a pin for a specified duration.
  @param pinNo - number of the pin where the diode is connecetd.
  @param duration - for how long the diode is supposed to shine
    in miliseconds
  @param pauseDuration - how long to wait after lighting the diode, ms
    */
  digitalWrite(pinNo, HIGH); // Set the desired pin high...
  delay(duration); // ...wait...
  digitalWrite(pinNo, LOW); // ...and we're done.
  delay(pauseDuration);
  return 0;
}

void setup(void)
{
  /* Called when Arduino starts up. */
  pinMode(PIN_NO, OUTPUT); // This pin will be set high and low to blink the LED.
}

void loop(void)
{
  /* Called repeatedly whilst Arduino is active. */
  lightDiode(PIN_NO, DURATION, PAUSE_DURATION);
}
