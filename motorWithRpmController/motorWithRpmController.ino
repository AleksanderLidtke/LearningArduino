int LED_PIN = 10; // Pin to which the LED is connected - use PWM
int SENSOR_PIN = A0; // Port to which the sensor is connected - use serial input pin for analog input
int MOTOR_PIN = 9; // Port from which rpm of the motor is controlled

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

        // rpm indicator diode
        analogWrite(LED_PIN, VALUE/4);
        
        // rpm controller
        analogWrite(MOTOR_PIN, VALUE/4);
}
