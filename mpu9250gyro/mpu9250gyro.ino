#include <Wire.h>

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS     0x0C

// Available choices for the gyroscope scale FS_SEL (deg s-1 or dps according to the doccumentation).
#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define GYRO_SENSITIVITY 131 // Least significant bit per degree a second.

//TODO: confirm these values.
#define ROOM_TEMP_OFFSET   0.00 // Typical value listed in Table 3.4.2
#define TEMP_SENSITIVITY 333.87 // Typical value listed in Table 3.4.2

// Accelerometer ranges.
#define ACC_FULL_SCALE_2_G  0x00
#define ACC_FULL_SCALE_4_G  0x08
#define ACC_FULL_SCALE_8_G  0x10
#define ACC_FULL_SCALE_16_G 0x18

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data);
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data);
void parseMagnetometerReadings(uint8_t* rawData, float* correctedReadings);

uint8_t magCorrections[3] = {128, 128, 128}; // ASAX, ASAY, ASAZ sensitivity adjustment values. Defaults that will not change the read raw data.
int16_t gyrCorrections[3]; // XG_OFFSET_H, XG_OFFSET_L, YG_OFFSET_H, YG_OFFSET_L, ZG_OFFSET_H, ZG_OFFSET_L. Values of the gyro DC bias, accounted for by the sensor.
float correctedMag[3]; // X, Y, Z magnetometer readings.

// Initializations
void setup()
{
  // Arduino initializations
  Wire.begin();
  Serial.begin(9600);
  
  // Set accelerometers low pass filter at 5 Hz //TODO: work out how to use the accelerometer.
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_2_G);
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,0x1B,GYRO_FULL_SCALE_250_DPS); // Simultaneously set fchoice_b to 00, i.e fchoice to 11 to give us filter frequency of 5 Hz.
  I2CwriteByte(MPU9250_ADDRESS,0x1A,0x06); // Write 110 to DLPF_CFG to set gyroscope low pass filter at 5 Hz.

  // Set by pass mode for the i2c_master interface pins (ES_CL and ES_DA), i.e. let them float high to be able to access the magnetometer.
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02); // Register 0x37 i.e. 55 is the INT_PIN_CFG register.

  // Request continuous magnetometer measurements in 16 bits at 100 Hz.
  I2CwriteByte(MAG_ADDRESS,0x0A,0x16); // 0x0A is the CNTL1 register. 0x16 is 10110 i.e. 16-bit Continuous measurement mode 2 for the magnetometer.

  // Read offsets.
  I2Cread(MAG_ADDRESS,0x10,3,magCorrections); // Read 3 bytes starting from register 0x10 i.e. ASAX. There are used to correct magnetometer readings into final measurements.
  
  pinMode(13, OUTPUT);
}

// Main loop, read and display data
void loop()
{
/*  uint8_t gyrCorBuf[6]; // High and low bytes of the DC bias of the gyroscope.
  I2Cread(MPU9250_ADDRESS,0x13,6,gyrCorBuf);
  gyrCorrections[0] = gyrCorBuf[0]<<8 | gyrCorBuf[1];
  gyrCorrections[1] = gyrCorBuf[2]<<8 | gyrCorBuf[3];
  gyrCorrections[2] = gyrCorBuf[4]<<8 | gyrCorBuf[5];
  Serial.print(">CG0,");Serial.print(gyrCorrections[0],DEC);
  Serial.print(",CG1,");Serial.print(gyrCorrections[1],DEC);
  Serial.print(",CG2,");Serial.print(gyrCorrections[2],DEC);
  Serial.println(";");*/
  
  // ===========================================================================
  // ACCELEROMETER, TEMPERATURE AND GYROSCOPE.
  // ===========================================================================
  
  // Read accelerometer, tempreature and gyroscope outputs.
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  
  // Parse accelerometer to 16 bit from two 8 bit values
  float correctedAcc[3]; //TODO: what are the units of this?
  correctedAcc[0] = Buf[0]<<8 | Buf[1];
  correctedAcc[1] = Buf[2]<<8 | Buf[3];
  correctedAcc[2] = Buf[4]<<8 | Buf[5];

  //TODO: convert this to Celsius. Take: (temp-RoomTemp_Offset)/Temp_Sensitivity+21. FOr some reason this gives over 30 deg C in a room in a house.
  // Get temperature - we've read it already.
  float temp = ((Buf[6]<<8 | Buf[7])-ROOM_TEMP_OFFSET)/TEMP_SENSITIVITY + 21.;
  //Serial.print(">T,");Serial.print(temp,DEC);Serial.println(";");

  //TODO: make this work...
  // Parse gyroscope to 16 bit from two 8 bit values
  float correctedGyr[3];
  correctedGyr[0] = (Buf[8]<<8 | Buf[9]);// / GYRO_SENSITIVITY; // Divide to get angular rate from bits.
  correctedGyr[1] = (Buf[10]<<8 | Buf[11]);// / GYRO_SENSITIVITY;
  correctedGyr[2] = (Buf[12]<<8 | Buf[13]);// / GYRO_SENSITIVITY;
  
  // Display accelerometer
  Serial.print(">AX,");Serial.print(correctedAcc[0],DEC);
  Serial.print(",AY,");Serial.print(correctedAcc[1],DEC);
  Serial.print(",AZ,");Serial.print(correctedAcc[2],DEC);
  Serial.println(";");
  
  // Display Gyroscope
  /*Serial.print(">GX,");Serial.print(correctedGyr[0],DEC);
  Serial.print(",GY,");Serial.print(correctedGyr[1],DEC);
  Serial.print(",GZ,");Serial.print(correctedGyr[2],DEC);
  Serial.println(";");*/
  
  // ===========================================================================
  // MAGNETOMETER
  // ===========================================================================
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
    /*Serial.print(">MX,");Serial.print(correctedMag[0],DEC);
    Serial.print(",MY,");Serial.print(correctedMag[1],DEC);
    Serial.print(",MZ,");Serial.print(correctedMag[2],DEC);
    Serial.print(",ST2,");Serial.print(Mag[6],DEC); // Print the ST2 register in decimal format. If it isn't 16 we've got sensor overflow.
    Serial.println(";");*/
  
  // End of measurement cycle.
  //Serial.println("==========================================================");
  delay(1000);    
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
 	correctedReadings[0] = correctedReadings[0] * (((float(magCorrections[0])-128.)*0.5)/128. + 1.);
 	correctedReadings[1] = correctedReadings[1] * (((float(magCorrections[1])-128.)*0.5)/128. + 1.);
 	correctedReadings[2] = correctedReadings[2] * (((float(magCorrections[2])-128.)*0.5)/128. + 1.);
}
