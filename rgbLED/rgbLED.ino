int ledColour = 0;

const int flashDuration = 1000;

const int red = 11;
const int green = 12;
const int blue = 13;

void setup()
{
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
  Serial.begin(19200);
  Serial.println("Ready");
}

void loop()
{
  /* This uses a common anode LED - 1 single power input from 5V pin, to switch
  on a given colour the control pin voltage is set to LOW */
  
  int oldLedColour = ledColour;
  do // loop to avoid repeating subsequent colours
  {
    ledColour = random(7); //this randomly selects a number between 0 and 6
  } while (oldLedColour == ledColour);
  Serial.println(ledColour);
  
  switch (ledColour)
  {
  case 0: //if ledColour equals 0 then the led will turn red
    digitalWrite(red, LOW);
    delay(flashDuration);
    digitalWrite(red, HIGH);
    break;
  case 1: //if ledColour equals 1 then the led will turn green
    digitalWrite(green, LOW);
    delay(flashDuration);
    digitalWrite(green, HIGH);
    break;
  case 2: //if ledColour equals 2 then the led will turn blue
    digitalWrite(blue, LOW);
    delay(flashDuration);
    digitalWrite(blue, HIGH);
    break;
  case 3: //if ledColour equals 3 then the led will turn yellow
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    delay(flashDuration);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    break;
  case 4: //if ledColour equals 4 then the led will turn cyan
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    delay(flashDuration);
    digitalWrite(red, HIGH);
    digitalWrite(blue, HIGH);
    break;
  case 5: //if ledColour equals 5 then the led will turn magenta
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    delay(flashDuration);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
    break;
  case 6: //if ledColour equals 6 then the led will turn white
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    delay(flashDuration);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
    break;
  }

}
