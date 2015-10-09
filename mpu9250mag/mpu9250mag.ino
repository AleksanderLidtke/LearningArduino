#include <Wire.h> // I2C library.

#define MAG_ADDRESS 0x0C // Address of the magnetometer on the I2C bus.

// This function reads Nbytes bytes from I2C device starting from the register at address Address. 
// Put read bytes starting at register Register in the Data array.
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes from susbsequent registers.
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}

// Write a byte (Data) in device (Address) to register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

// Initializations
void setup()
{
  // Arduino initializations
  Wire.begin();
  Serial.begin(9600);
   
  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS,0x0A,0x16); // 0x0A is the CNTL1 register. 0x16 is 10110 i.e. 16-bit Continuous measurement mode 2 for the magnetometer.
    // Use 0x18 i.e. 11000 for self-test, and 0x12 i.e. 10010 for continuous mode 1.
    // For self test write 0x40 i.e. 1000000 to register 0x0C i.e. ASTC.
  
  pinMode(13, OUTPUT);
}

// Main loop, read and display data
void loop()
{ 
  // Read register Status 1 and wait for the DRDY: Data Ready
  uint8_t ST1;
  do
  {
    I2Cread(MAG_ADDRESS,0x02,1,&ST1); // Read 1 byte from register 0x02 i.e. ST1.
  }
  while (!(ST1&0x01)); // While ST1 isn't 0x01 i.e. 1.

  // Read magnetometer data stored in Little Endian format, i.e. least-significant byte.
  // The measurements go from -32760 to 32760, i.e. from -4912 to 4912 micro-Teslas in 16 bit resolution.
  uint8_t Mag[7]; // HXL, HXH, HYL, HYH, HZL, HZH, ST2.
  I2Cread(MAG_ADDRESS,0x03,7,Mag); // Read 7 bytes starting from register 0x03 i.e. HXL. Will also reset the magnetometer.
  // Create 16 bits values from 8 bits data

  // Check if the magnetometer has overflown. If so, Mag[6] will be 00011000. If everything's good it'll be 00010000, i.e. 0x10.
  // The ST2 register will have 1 in bit D4 (16 bit measurements) and should have 0 in bit D3 (HOFL, i.e. overflow error).
  if( !(Mag[6]&0x10) ) // Magnetometer reading not OK?
  {
    Serial.println("Magnetic sensor overflow!"); // 
  }
    
  // Combine lower and higher 8 bit magnetometer readings into 16 bit outputs.
  int16_t mx=-(Mag[3]<<8 | Mag[2]); // Shift Mag[3] 8 bits (1 byte) to the left, and fill the rightmost 8 bits with Mag[2].
  int16_t my=-(Mag[1]<<8 | Mag[0]);//TODO: really? I according to the docs 0 1 are for X and 2 3 for Y.
  int16_t mz=-(Mag[5]<<8 | Mag[4]);
  
  // Print magnetometer data.
  Serial.print(">X,");Serial.print(mx,DEC);
  Serial.print(",Y,");Serial.print(my,DEC);
  Serial.print(",Z,");Serial.print(mz,DEC);  
  Serial.print(",ST2,");Serial.print(Mag[6],DEC); // Print the ST2 register in decimal format. If it isn't 16 we've got sensor overflow.
  Serial.print(";");

  //TODO: figure out self test.
  
  // End of measurement cycle.
  Serial.println("");
  delay(100);    
}

