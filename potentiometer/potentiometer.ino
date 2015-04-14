int LED_PIN = 9; // Pin to which the LED is connected - use PWM
int SENSOR_PIN = A0; // Port to which the sensor is connected - use serial input pin for analog input
int VALUE = 0; // Stores the sensor output

void setup(void)
{
/* Called when Arduino starts up. */
	pinMode(LED_PIN, OUTPUT); // This pin will be set high to light the LED.
}

void loop(void)
{
/* Called repeatedly whilst Arduino is active. */
	VALUE = analogRead(SENSOR_PIN);

        // this uses a threshold value to switch the LED on or off by twisting the knob
        /*if (VALUE > 512)
        {
          digitalWrite(LED_PIN, HIGH);
        }
        else
        {
          digitalWrite(LED_PIN, LOW);
        }*/
        
        // dim the LED gradually
        analogWrite(LED_PIN,VALUE/4);
}
