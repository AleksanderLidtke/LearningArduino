int counter(0); // dummy counter for showing saw-tooth data in the output
int frequency(100); // sample frequency in Hertz [Hz]

// Initializations
void setup()
{
  // Arduino initializations
  Serial.begin(19200);
  Serial.println("Arduino starting");
  Serial.println("================");
  Serial.println("");
}

// Main loop, read and display data
void loop()
{
  // generate known output data
  if(++counter > 100)
    counter = 0;
  
  // begin message
  String message =  String(">");
  
  // Display accelerometer
  message = String(message+"AX,"+String(counter));
  message = String(message+",AY,"+String(counter));
  message = String(message+",AZ,"+String(counter));
  
  // Display Gyroscope
  message = String(message+",GX,"+String(counter));
  message = String(message+",GY,"+String(counter));
  message = String(message+",GZ,"+String(counter));
  
  // Print corrected magnetometer data.
  message = String(message+",MX,"+String(counter));
  message = String(message+",MY,"+String(counter));
  message = String(message+",MZ,"+String(counter));
  
  // end message
  message += ";";
  
  // send
  Serial.println(message);
  
  // End of measurement cycle.
  delay(int(1000./frequency));    
}

