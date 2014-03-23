/*
Arduino Camera Shutter
Frank Olson
V.1
22 March 2014
Arduino Mega 2560 R3
Last Update: 23 March 2014

Simple Serial ECHO script : Written by ScottC 03/07/2012

This program is for the extra credit on homework. Using the new delay function
created, develop a program that controls a camera shutter on portB.7. Shutter
speeds will be passed through computer using the serial monitor and the echo
script cited above. 0 should be off and 1 should be on.
*/

// Define hardware pointers
volatile unsigned char *portB;      // Port B
volatile unsigned char *myTCCR1A;   // Timer/Counter 1 Control Register A
volatile unsigned char *myTCCR1B;   // Timer/Counter 1 Control Register B
volatile unsigned char *myTCCR1C;   // Timer/Counter 1 Control Register C
volatile unsigned char *myTIMSK1;   // Timer/Counter 1 Interupt Mask Register
volatile unsigned char *myTIFR1;    // Timer/Counter 1 Interupt Flag Register
volatile unsigned int  *myTCNT1;    // Timer/Counter 1 16-bit Counter Register

// Define shutter speeds:
//                     Input      Speed (us)      Shutter Speed (s)
volatile unsigned long 0     =    1000000;     // 1
volatile unsigned long 1     =    500000;      // 1/2
volatile unsigned long 2     =    250000;      // 1/4
volatile unsigned long 3     =    125000;      // 1/8
volatile unsigned long 4     =    66666;       // 1/15
volatile unsigned long 5     =    33333;       // 1/30
volatile unsigned long 6     =    16666;       // 1/60
volatile unsigned long 7     =    8000;        // 1/125
volatile unsigned long 8     =    4000;        // 1/250
volatile unsigned long 9     =    2000;        // 1/500
volatile unsigned long 10    =    1000;        // 1/1000


// Define functions
void newDelay(unsigned long mSeconds);
long convertInput(byte value);

// Use a variable called byteRead to store the data coming
// from the computer.
byte byteRead;

void setup() {                
  // Turn the Serial Protocol ON
  Serial.begin(9600);
  
  // Initialize timer1 pointers 
  myTCCR1A = (unsigned char *) 0x80;
  myTCCR1B = (unsigned char *) 0x81;
  myTCCR1C = (unsigned char *) 0x82;
  myTIMSK1 = (unsigned char *) 0x6F;
  myTIFR1 = (unsigned char *) 0x36;
  myTCNT1 = (unsigned int *) 0x84;

  // Initialize NORMAL mode, off, and no interupts for timer1
  *myTCCR1A = 0;
  *myTCCR1B = 0;
  *myTCCR1C = 0;
  *myTIMSK1 = 0;

  // Initialize GPIO portB
  volatile unsigned char *portDDRB;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x80;
  
  // Message to user/prompt
  Serial.println("Shutter Speeds are 1, 1/2, 1/4, 1/8, 1/15, 1/30, 1/60, ");
  Serial.println("1/125, 1/250, 1/500, 1/1000. Enter 0 through 10  to");
  Serial.println("represent the speed, 0 being 1, 1 being 1/2 and so forth.");
}

void loop() {
  // Check if data has been sent from the computer:
  if (Serial.available()) {
    // Read the most recent byte
    byteRead = Serial.read();
    // ECHO the value that was read, back to the serial port
    Serial.write(byteRead);
    Serial.println();
    // Message to user/prompt
    Serial.println("Shutter Speeds are 1, 1/2, 1/4, 1/8, 1/15, 1/30, 1/60, ");
    Serial.println("1/125, 1/250, 1/500, 1/1000. Enter 0 through 10  to");
    Serial.println("represent the speed, 0 being 1, 1 being 1/2 and so forth.");
  }
  
  portB = (unsigned char *) 0x25; 
  
  // Flip portB then use byteRead to define shutter speed
  *portB ^= 0x80;
  newDelay(convertInput(byteRead));
  
}

// Delay Function (microseconds)
void newDelay(unsigned long mSeconds) {
  // used a prescaler of 64 with mSeconds/4 scaling converting to microseconds
  *myTCNT1 = (unsigned int) (65536 - (long) (mSeconds/4));
  *myTCCR1B = 0b00000011;     // Prescalar of 1024 applied to timer1
  
  // loop through timer;
  while((*myTIFR1 & 0x01) == 0);
  
  // Turn off the timer and clear the flag bit for next use
  *myTCCR1B = 0;
  *myTIFR1 &= 0x01;
}

// Convert input to tone
long convertInput(byte value) {
  if(value == '0'){
    return 0;
  }
  else if(value == '1'){
    return 1;
  }
  else if(value == '2'){
    return 2;
  }
  else if(value == '3'){
    return 3;
  }
  else if(value == '4'){
    return 4;
  }
  else if(value == '5'){
    return 5;
  }
  else if(value == '6'){
    return 6;
  }
  else if(value == '7'){
    return 7;
  }
  else if(value == '8'){
    return 8;
  }
  else if(value == '9'){
    return 9;
  }
  else if(value == '10'){
    return 10;
  }
}

