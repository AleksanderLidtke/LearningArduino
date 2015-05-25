/* Handle the Maxim DS3234 real-time clock (RTC) via the SPI interface. Please check
@see http://tronixstuff.com/wp-content/uploads/2011/06/ds3234s.pdf for the datasheet.
Test various aspects of the clock's functionality.

The test circuit is simple: for Arduino ProMini connect 10 to Chip Select (SS/CS),
11 to MOSI and 12 to MISO (clock), and 13 to SCK (denoted CLK on Maxim DS3234).

Te keep things easy, i.e. not use the battery, also connect the Vcc on the DS3234
to the 5 V power output of the Arduino; N.B. 3.3 V will not work for some reason.
For good measure also connect the two grounds.

The RTC may also operate using a battery. //TODO figure out if it will work on battery w/o Vcc.

@author: Alek
@version: 1.0.0
@since: 24 May 2015

CHANGELOG:
24 May 2015 - 1.0.0 - First debugged version that can set and read time from the RTC powered vie the VCC released.

BASIC EXAMPLES
----------------------
Read from a register:
----------------------
digitalWrite(10, LOW); // Select the DS3234 that has its CS line on digital 10.
SPI.transfer(0x08); // Tell the DS3234 device we're requesting data from the register at 08h.
data = SPI.transfer(0); // The DS3234 sends the data back and stores it in the byte data.
digitalWrite(10, HIGH); // Deselect the DS3234 if finished with it.

----------------------
Write to a register:
----------------------
digitalWrite(10, LOW);
SPI.transfer(0x80); // Tells the device which address to write to.
SPI.transfer(b00001010); // You can send any representation of a byte to the register.
digitalWrite(10, HIGH);
*/
#include <SPI.h>

const int CS = 10; // Chip select/Slave enable - digital IO pin that will be used to select a given slave device.
const char* MONTH_ABBREVS[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; // Used to format time structs to readable strings.

struct TimeStruct
// Hold information about a time in a nice and convenient way.
{
  int year; // Ignore the millennia.
  int month;
  int day;
  int hour; // In 24-hour mode.
  int minute;
  int second;
};

// Prototypes.
void SetTimeDate(int day, int month, int year, int hour, int minute, int second); // Set the time on the device.
TimeStruct ReadTimeDate(void); // Read the time from the device.
String GetReadableTime(TimeStruct inputTime); // Format the time to a readable string.

void setup(void)
/* Called when Arduino starts up. Prepares to use the real-time clock over SPI
and to print the output for debugging via serial. */
{
	pinMode(CS, OUTPUT); // Use this pin as the chip select for the real-time clock.
	
	/* Prepare the SPI bus for operation. */
	SPI.begin(); // Activate the SPI bus.
	SPI.setBitOrder(MSBFIRST); // Whether the first bit of the data is 2^0 or 2^N (where N is the number of bits sent).
	SPI.setDataMode(SPI_MODE3); // Set the mode of operation of the SPI bus, which defines the MOSI and MISO clock polarity and phase, e.g. if looking at the rising or falling edge and whether the base value is 0 or 1. Look at page 1 of the manual on the features list.
	digitalWrite(CS, HIGH); // If this is set to high the slave device will be inactive.
		
	/* Configure the RTC. The control register 0Eh/8Eh is as follows:
	bit7 EOSC   Enable Oscillator (1 if oscillator must be stopped when on battery).
	bit6 BBSQW  Battery Backed Square Wave.
	bit5 CONV   Convert temperature (1 forces a conversion NOW).
	bit4 RS2    Rate select - frequency of square wave output.
	bit3 RS1    Rate select - both are used to select the frequency.
	bit2 INTCN  Interrupt control (1 for use of the alarms and to disable square wave, 0 to use the square wave).
	bit1 A2IE   Alarm2 interrupt enable (1 to enable).
	bit0 A1IE   Alarm1 interrupt enable (1 to enable).
	*/
	digitalWrite(CS, LOW);
	SPI.transfer(0x8E); // Select the control register.
	SPI.transfer(B01100000); //Use the oscillator under battery power, use the square wave output @ 1 Hz, compensate for the temperature-induced capacitance changes, disable all alarms.
	digitalWrite(CS, HIGH);
	delay(10); // Give everything time to settle down.
	
	/* Start serial used for debugging purposes. */
	Serial.begin(9600);
	
	/* Read the control register to be sure what the settings are. */
	digitalWrite(CS, LOW);
	SPI.transfer(0x0E); // Select the control register.
	byte RTCData = SPI.transfer(0x00);
	digitalWrite(CS, HIGH);
	Serial.print("RTC settings are: "); Serial.print(RTCData,BIN); Serial.println(""); // Print the settings as 0s and 1s corresponding to bits in the settings register 0x8E.
	
	SetTimeDate(11,12,13,14,15,16); // Set the time on the device.
}

void loop(void)
/* Called repeatedly whilst Arduino is active. Will print ram seconds data
and formatted time data every second. */	
{
	/*
	----------------------------------------------------------------------------
		Example of how to print the raw data.
	----------------------------------------------------------------------------
	*/
	digitalWrite(CS, LOW);
	SPI.transfer(0x00); // Seconds register.
	byte RawSeconds = SPI.transfer(0x00);
	digitalWrite(CS, HIGH);
	// Print the raw seconds	register contents as 0s and 1s.
	Serial.print("Raw seconds data are: "); Serial.print(RawSeconds,BIN); Serial.println("");

	/*
	----------------------------------------------------------------------------
		Example of how to read and print formatted time data.
	----------------------------------------------------------------------------
	*/
	TimeStruct currentTime = ReadTimeDate(); // Read the current time on the device.

	// Print the time for debugging.
	Serial.print("Current time: ");Serial.println(GetReadableTime(currentTime));
	
	delay(1000); // Take a time step.
}

void SetTimeDate(int day, int month, int year, int hour, int minute, int second)
/* Set the time registers on the DS3234 real-time clock. The time read addresses
of the registers are 0x00h to 0x06h, and write addresses are 0x80 to 0x86. Their
orders are order second, minute, hour, day of week, day of month, month, year.

This function ignores the day of week register (read/write 0x03h/0x83h).

@param day - day of the month, 1 to 31, registers 0x04h/0x84h (read/write)
@param month - month, 1 to 12, registers 0x05h/0x85h (read/write)
@param year - year, 0, 99, registers 0x06h/0x86h (read/write)
@param hour - hour in the 24-hour time format, 0-23, registers 0x02h/0x82h (read/write)
@param minute - minute, 0 to 59, registers 0x01h/0x81h (read/write)
@param second - second, 0 to 59, registers 0x00h/0x80h (read/write)
*/
{
	int TimeDate[7] = {second,minute,hour,0,day,month,year}; // Ignore day of the week, i.e. register 0x83.
	// Go through registers 0x80 to 0x86 and set the time on the RTC.
	for(int i=0; i<7; i++){
		if(i==3) continue; // Ignore day of the week.

		// Split the time component into multiples of 10 and the remainder.
		int multOfTen = TimeDate[i]/10; // The four largest bits of time.
		int intPart = TimeDate[i] - multOfTen*10; // The four smallest bits of time.

		// Set the bits 5 and 4 for the hour register - that's where the multiples of 10 hours are kept.
		if(i==2){
			if (multOfTen==2)
				multOfTen=B00000010;
			else if (multOfTen==1)
				multOfTen=B00000001;
		}
		
		// Shift the multOfTen four bits to the left i.e. where the large time component values are kept.
		TimeDate[i] = intPart + (multOfTen<<4);

		// Set the value of this time component in the appropriate register.
		digitalWrite(CS, LOW);
		SPI.transfer(i+0x80);
		SPI.transfer(TimeDate[i]);
		digitalWrite(CS, HIGH);
  }
}

TimeStruct ReadTimeDate(void)
/* Read the time registers on the DS3234 real-time clock and return it as a time
struct containing fields: second, minute, hour (in 24-hour mode), day of month,
month, year.

This function ignores the day of week register (read/write 0x03h/0x83h).

@return - time struct containing the time that is held by the RTC.
*/
{
	struct TimeStruct timeToReturn; // Record the values here.
	
	// Go through all the time registers and read their values, ignore day of the week. The order of the registers is second, minute, hour, day of the week, day of the month, month, year.
	for(int i=0; i<7; i++){
		if(i==3) continue; // Ignore day of the week, i.e. register 0x03.
			
		digitalWrite(CS, LOW); // Read the time component.
		SPI.transfer(i+0x00);
		unsigned int n = SPI.transfer(0x00);
		digitalWrite(CS, HIGH);
		
		//TODO what?
		// Parse the time component data according to what register is being read.
		int a = n & B00001111; // Only keeps the low 4 bits - integer part.
		if (i==0){ // Second.
			int b = (n & B01110000) >> 4; 
			timeToReturn.second = a + b*10;
		}
		else if (i==1){ // Minute.
			int b=(n & B01110000)>>4;
			timeToReturn.minute = a+b*10;
		}
		else if(i==2){ // Hour.
			int b=(n & B00110000)>>4; //24 hour mode
			if(b==B00000010)
				b=20;
			else if(b==B00000001)
				b=10;
			timeToReturn.hour = a+b;
		}
		else if(i==4){
			int b=(n & B00110000)>>4;
			timeToReturn.day = a+b*10;
		}
		else if(i==5){
			int b=(n & B00010000)>>4;
			timeToReturn.month = a+b*10;
		}
		else if(i==6){
			int b=(n & B11110000)>>4;
			timeToReturn.year = a+b*10;
		}

	}

  return timeToReturn;
}

String GetReadableTime(TimeStruct inputTime)
/* Format the time struct instance to a readable string.

@param - a time struct with year, month, day, hour, minute and second fields.
@return - a string of the format DD-MMM-YY hh:mm:ss
*/
{
	String returnString; // String with the formatted time.
	
	// Format the string appropriately.
	returnString.concat(inputTime.day);
	returnString.concat("-") ;
	returnString.concat( MONTH_ABBREVS[inputTime.month-1]); // TimeStruct.month is from 1 to 12 - change it to an index.
	returnString.concat("-") ;
	returnString.concat(inputTime.year);
	returnString.concat(" ") ;
	returnString.concat(inputTime.hour);
	returnString.concat(":") ;
	returnString.concat(inputTime.minute);
	returnString.concat(":") ;
	returnString.concat(inputTime.second);
	
	return returnString;
}
