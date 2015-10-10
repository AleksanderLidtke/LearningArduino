#include <Wire.h> // I2C library.

#define MPU9250_ADDRESS 0x68 // Address of the accelerometer and gyroscope module on the I2C bus.
#define MAG_ADDRESS 0x0C // Address of the magnetometer on the I2C bus.

uint8_t corrections[3] = {128, 128, 128}; // ASAX, ASAY, ASAZ sensitivity adjustment values. Defaults that will not change the read raw data.
float correctedMag[3]; // X, Y, Z magnetometer readings.

void parseMagnetometerReadings(uint8_t* rawData, float* correctedReadings)
/* Combine lower and higher bytes of the magnetometer readings, apply sensitivity
 * corrections, and return the final magnetometer readings.
 * @param rawData - array of length 6 with bytes read from HXL, HXH, HYL, HYH, HZL,
 * 	and HZH registers.
 * @param - pointer to an array of length with corrected magnetometer readings
 * 	along X, Y, and Z axes of the magnetometer.
 */
{
 	// Combine lower and higher 8 bit magnetometer readings into 16 bit outputs.
    correctedReadings[0] = float(rawData[1]<<8 | rawData[0]);
    correctedReadings[1] = float(rawData[3]<<8 | rawData[2]);
    correctedReadings[2] = float(rawData[5]<<8 | rawData[4]);
    
    // Apply corrections the sensor was shipped with.
 	correctedReadings[0] = correctedReadings[0] * (((float(corrections[0])-128.)*0.5)/128. + 1.);
 	correctedReadings[1] = correctedReadings[1] * (((float(corrections[1])-128.)*0.5)/128. + 1.);
 	correctedReadings[2] = correctedReadings[2] * (((float(corrections[2])-128.)*0.5)/128. + 1.);
}

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
/* Read NBytes from an I2C device starting at a register at a given address. The
 * read bytes will be put in the Data array.
 * @param Address - address of the device on the I2C bus in hex format, e.g. 0x0C.
 * @param Register - which register to read as the first in hex format, e.g. 0x10.
 * @param NBytes - how many bytes (consecutive registers) to read.
 * @param Data - array of length NBytes where the bytes read from the consecutive
 * 	registers will be written.
 */
{
    // Set first register address.
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.endTransmission();
    
    // Read NBytes from consecutive registers.
    Wire.requestFrom(Address, Nbytes); 
    uint8_t index=0; // Index of the register (corresponding to Data entry) that we're reading right now.
    while (Wire.available())
    {
    	Data[index++]=Wire.read();
    }
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
/* Write data to a register of an I2C device.
 * @param Address - address of the device on the I2C bus in hex format, e.g. 0x0C.
 * @param Register - which register to read as the first in hex format, e.g. 0x10.
 * @param Data - byte of data that will be written to the Register in hex format, e.g. 0x40.
 */
{
    Wire.beginTransmission(Address); // Set address of the device and register where we'll write to.
    Wire.write(Register);
    Wire.write(Data); // Write the data.
    Wire.endTransmission();
}

void setup() // Start the magnetometer and serial communications.
{
    // Arduino initializations
    Wire.begin();
    Serial.begin(9600);

    I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x22); // Set MPU9250 into bypass mode to access magnetometer data by writing to register INT_PIN_CFG. This is needed c.f. this post: https://github.com/kriswiner/MPU-9250/issues/3

	/* Read the sensitivity correction factors supplied by the manufacturer in Read Only Memory (ROM). */
	I2Cread(MAG_ADDRESS,0x10,3,corrections); // Read 3 bytes starting from register 0x10 i.e. ASAX.
	Serial.print(">ASAX,");Serial.print(corrections[0],DEC); // Print the correction factors.
	Serial.print(",ASAY,");Serial.print(corrections[1],DEC);
	Serial.print(",ASAZ,");Serial.print(corrections[2],DEC);
	Serial.print(";");Serial.println("");
    
    /* Self-test the AK9863 magnetometer as per the user's manual. */
    I2CwriteByte(MAG_ADDRESS,0x0A,0x10); // Power-down mode in 16 bits, i.e. 10000.
    I2CwriteByte(MAG_ADDRESS,0x0C,0x40); // Generate magnetic field for self-test by writing 0x40 i.e. 1000000 to register 0x0C i.e. ASTC.
    I2CwriteByte(MAG_ADDRESS,0x0A,0x18); // Enter self-test mode.
    
    // Read register Status 1 and wait for the DRDY: Data Ready
    uint8_t ST1;
    do
    {
        I2Cread(MAG_ADDRESS,0x02,1,&ST1); // Read 1 byte from register 0x02 i.e. ST1.
        Serial.println("Waiting for magnetometer self-test data.");
    }
    while (!(ST1&0x01)); // While ST1 isn't 0x01 i.e. 1.
    
    uint8_t Mag[6]; // HXL, HXH, HYL, HYH, HZL, HZH.
    I2Cread(MAG_ADDRESS,0x03,6,Mag); // Read 6 bytes starting from register 0x03 i.e. HXL.
    parseMagnetometerReadings(Mag,correctedMag); // Apply corrections and combine lower and higher bytes.
    
    I2CwriteByte(MAG_ADDRESS,0x0C,0x00); // Write 0 to ASTC to switch the self magnetic field off.
    I2CwriteByte(MAG_ADDRESS,0x0A,0x10); // Set power down.

    // Print magnetometer test data.
    Serial.print(">Xt,");Serial.print(correctedMag[0],DEC);
    Serial.print(",Yt,");Serial.print(correctedMag[1],DEC);
    Serial.print(",Zt,");Serial.print(correctedMag[2],DEC);
    Serial.print(";");
    
    /* Request continuous magnetometer measurements in 16 bits for nomial operations. */
    I2CwriteByte(MAG_ADDRESS,0x0A,0x16); // 0x0A is the CNTL1 register that deicides what measurement we'll do.
        // Use 0x16 i.e. 10110 for 16-bit Continuous measurement mode 2, i.e. every 100 Hz.
        // Use 0x12 i.e. 10010 for 16-bit Continuous mode 1, i.e. every 5 Hz.
        // Use 0x18 i.e. 11000 for 16-bit self-test.
    
    pinMode(13, OUTPUT);
}

void loop() // Main loop, read and display magnetometer data.
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

    // Check if the magnetometer has overflown. If so, Mag[6] will be 00011000. If everything's good it'll be 00010000, i.e. 0x10.
    // The ST2 register will have 1 in bit D4 (16 bit measurements) and should have 0 in bit D3 (HOFL, i.e. overflow error).
    if( !(Mag[6]&0x10) ) // Magnetometer reading not OK?
    {
        Serial.println("Magnetic sensor overflow!"); // 
    }
    
	parseMagnetometerReadings(Mag, correctedMag); // Apply corrections and combine lower and higher bytes. Will ignore ST2 on position Mag[6].
    
    // Print corrected magnetometer data.
    Serial.print(">X,");Serial.print(correctedMag[0],DEC);
    Serial.print(",Y,");Serial.print(correctedMag[1],DEC);
    Serial.print(",Z,");Serial.print(correctedMag[2],DEC);
    Serial.print(",ST2,");Serial.print(Mag[6],DEC); // Print the ST2 register in decimal format. If it isn't 16 we've got sensor overflow.
    Serial.print(";");
    
    // End of measurement cycle.
    Serial.println("");
    delay(100);
}
