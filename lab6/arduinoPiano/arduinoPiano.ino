/*
Arduino Piano
Frank Olson
V.1
10 March 2014
Arduino Mega 2560 R3
Last Update: 11 March 2014

Simple Serial ECHO script : Written by ScottC 03/07/2012
*/

// Define hardware pointers
volatile unsigned char *portB;      // Port B
volatile unsigned char *myTCCR1A;   // Timer/Counter 1 Control Register A
volatile unsigned char *myTCCR1B;   // Timer/Counter 1 Control Register B
volatile unsigned char *myTCCR1C;   // Timer/Counter 1 Control Register C
volatile unsigned char *myTIMSK1;   // Timer/Counter 1 Interupt Mask Register
volatile unsigned char *myTIFR1;    // Timer/Counter 1 Interupt Flag Register
volatile unsigned int  *myTCNT1;    // Timer/Counter 1 16-bit Counter Register

// Define functions
void newDelay(unsigned long mSeconds);

// Use a variable called byteRead to temporarily store
// the data coming from the computer
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
}

void loop() {
  // Check if data has been sent from the computer:
  if (Serial.available()) {
    // Read the most recent byte
    byteRead = Serial.read();
    // ECHO the value that was read, back to the serial port
    Serial.write(byteRead);
  }
  
}

// Delay Function
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