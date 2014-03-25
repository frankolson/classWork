/*
Arduino Piano
Frank Olson
V.1
10 March 2014
Arduino Mega 2560 R3
Last Update: 11 March 2014

Simple Serial ECHO script : Written by ScottC 03/07/2012

This program is for lab 6. Using the new delay function created and the simple
serial connection code cited above create a program that takes computer input
and outputs tones like a piano on portB.6 on the Arduino Mega with a 50% duty
cycle.
*/

// Define hardware pointers
volatile unsigned char *portB;      // Port B
volatile unsigned char *myTCCR1A;   // Timer/Counter 1 Control Register A
volatile unsigned char *myTCCR1B;   // Timer/Counter 1 Control Register B
volatile unsigned char *myTCCR1C;   // Timer/Counter 1 Control Register C
volatile unsigned char *myTIMSK1;   // Timer/Counter 1 Interupt Mask Register
volatile unsigned char *myTIFR1;    // Timer/Counter 1 Interupt Flag Register
volatile unsigned int  *myTCNT1;    // Timer/Counter 1 16-bit Counter Register

// Define Frequency Tones (lowercase stands fo sharpnote):
//                     Note       Tone         Frequency
volatile unsigned long A     =    2272;     // 440 Hz
volatile unsigned long a     =    2146;     // 466 Hz
volatile unsigned long B     =    2024;     // 494 Hz
volatile unsigned long C     =    1912;     // 523 Hz
volatile unsigned long c     =    1805;     // 554 Hz
volatile unsigned long D     =    1704;     // 587 Hz
volatile unsigned long d     =    1603;     // 624 Hz
volatile unsigned long E     =    1517;     // 659 Hz
volatile unsigned long F     =    1433;     // 698 Hz
volatile unsigned long f     =    1351;     // 740 Hz
volatile unsigned long G     =    1276;     // 784 Hz
volatile unsigned long g     =    1203;     // 831 Hz

// Define functions
void newDelay(unsigned long mSeconds);
long convertInput(byte value);

// Use a variable called byteRead to store the data coming
// from the computer. Use as tone and defualt to 440Hz
// Note: 0b01000001 is 'A' in binary
byte byteRead = 0b01000001;

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
  *portDDRB |= 0x40;
  
  Serial.println("Notes are A, A#, B, C, C#, D, D#, E, F, F#, G, G#.");
  Serial.println("Arduino will default at A. Write sharp notes lower case.");
  Serial.println("Please Enter the note you would like to play: ");
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
    Serial.println("Notes are A, A#, B, C, C#, D, D#, E, F, F#, G, G#.");
    Serial.println("Arduino will default at A. Write sharp notes lower case");
    Serial.println("Please Enter the note you would like to play: ");
  }
  
  portB = (unsigned char *) 0x25; 
  
  // Flip portB then use byteRead/2 to define pulses
  *portB ^= 0x40;
  newDelay(convertInput(byteRead)/2);
  
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

// Convert input to tone
long convertInput(byte value) {
  if(value == 'A'){
    return A;
  }
  else if(value == 'a'){
    return a;
  }
  else if(value == 'B'){
    return B;
  }
  else if(value == 'C'){
    return C;
  }
  else if(value == 'c'){
    return c;
  }
  else if(value == 'D'){
    return D;
  }
  else if(value == 'd'){
    return d;
  }
  else if(value == 'E'){
    return E;
  }
  else if(value == 'F'){
    return F;
  }
  else if(value == 'f'){
    return f;
  }
  else if(value == 'G'){
    return G;
  }
  else if(value == 'g'){
    return g;
  }
}

