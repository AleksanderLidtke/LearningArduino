int LED_PIN = 13; // Pin to which the LED is connected.
int BUTTON_PIN_1 = 2; // Port to which the button is connected.
int BUTTON_PIN_2 = 3; // We've got two switches.

void setup(void)
{
/* Called when Arduino starts up. */
	pinMode(LED_PIN, OUTPUT); // This pin will be set high to light the LED.
	pinMode(BUTTON_PIN_1, INPUT);
	pinMode(BUTTON_PIN_2, INPUT);
	digitalWrite(LED_PIN, LOW);
}

void loop(void)
{
/* Called repeatedly whilst Arduino is active. */
	if( digitalRead(BUTTON_PIN_1) == LOW )
	{
		digitalWrite(LED_PIN, LOW);
	}
	else if( digitalRead(BUTTON_PIN_2) == LOW )
	{
		digitalWrite(LED_PIN, HIGH);
	}
}
