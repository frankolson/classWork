/*
New Delay Blinky
Frank Olson
V.1
11 March 2014
Arduino Mega 2560 R3
Last Update: 11 March 2014

This program is for question one on the homework. Replacing the crude delay function
used previously with a more accurate deay function utilizing the board timers.
*/

// Define functions
void newDelay(unsigned long mSecondsApx);

// Setup Function
void setup() {
  unsigned char *portDDRB;
  
  portDDRB = (unsigned char *) 0x24;
  
  *portDDRB |= 0x80;
}

// Loop Function
void loop() {
  unsigned char *portB;
  
  portB = (unsigned char *) 0x25; 
    
  // Turn on portB then wait half a second
  *portB |= 0x80;
  newDelay(500);
  // Turn off portB then wait half a second 
  *portB &= 0x7F;
  newDelay(500);
}

// Delay Function
void newDelay(unsigned long mSecondsApx) {
  
}

