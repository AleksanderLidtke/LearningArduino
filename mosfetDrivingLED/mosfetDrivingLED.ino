int PIN_NO = 10; // Pin to which the MOSFET is connected.
int DURATION = 500; // How long the wait at a given level of current going through the LED, ms.
int FULL_CURRENT_DURATION = 1000; // How long to keep the current at the highest value.
int PAUSE_DURATION = 500; // How much time to wait between consecutive ramp ups of current, ms.

void setup(void)
{
  /* Called when Arduino starts up. */
  pinMode(PIN_NO, OUTPUT); // This pin will be set high and low to blink the LED.
  analogWrite(PIN_NO, 0); // Initialise at no voltage between the pin and the ground of the Arduino that is connected to the battery ground.
}

void loop(void)
{
  /* Called repeatedly whilst Arduino is active. */
  for(int level=0; level<256; level++) // Gradually ramp up the current going through the diode by 
  {
    analogWrite(PIN_NO, level); // Set the pin a bit higher...
    delay(DURATION); // ...wait for a while to be able to see how the current is getting ramped up.
  }
  delay(FULL_CURRENT_DURATION); // Wait for a while at full current.
  
  analogWrite(PIN_NO, 0); // Set the pin back to low to cut the current.
  delay(PAUSE_DURATION); // Wait until the next cycle.
}
