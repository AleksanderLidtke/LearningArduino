void setup(void)
{
  /* Called when Arduino starts up. */
  pinMode(13, OUTPUT);
}

void loop(void)
{
  /* Called repeatedly whilst Arduino is active. */
  digitalWrite(13, HIGH);
  delay(1000); // ms
  digitalWrite(13,LOW);
  delay(1000);
}
  
