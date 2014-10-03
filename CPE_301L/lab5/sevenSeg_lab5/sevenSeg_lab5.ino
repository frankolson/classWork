/*
Seven Segment Display Practice in ANSI C
Frank Olson
V.1
05 March 2014
Arduino Mega 2560 R3

This program uses ANSI C to acces the ports on the Arduino Mega
controling a seven segment display. The program loops continously
through all of the hexadecimal characters. This program is
practice for port selection using ANSI C compatible with most
micro controllers.
*/

// Define functions
void MyDelay(unsigned long mSecondsApx);

// Setup Function
void setup() {
  // Next few steps set up port addresses
  unsigned char *portDDRK;
  
  portDDRK = (unsigned char *) 0x107;
  
  *portDDRK = 0xFF;
}

// Loop Function
void loop() {
  
  unsigned char *portK;

  portK = (unsigned char *) 0x108;
  
  // Run through all the hexadicamal characters on the seven 
  // segment display
  
  // LED segment 0
  *portK = 0b01111110;
  MyDelay(300);
  
  // LED segment 1
  *portK = 0b00001100;
  MyDelay(300);
 
  // LED segment 2
  *portK = 0b10110110;
  MyDelay(300);
 
  // LED segment 3
  *portK = 0b10011110;
  MyDelay(300);
 
  // LED segment 4
  *portK = 0b11001100;
  MyDelay(300);
 
  // LED segment 5
  *portK = 0b11011010;
  MyDelay(300);
 
  // LED segment 6
  *portK = 0b11111010;
  MyDelay(300);
 
  // LED segment 7
  *portK = 0b01001110;
  MyDelay(300);
 
  // LED segment 8
  *portK = 0b11111110;
  MyDelay(300);
 
  // LED segment 9
  *portK = 0b11001110;
  MyDelay(300);
 
  // LED segment A
  *portK = 0b11101110;
  MyDelay(300);
 
  // LED segment b
  *portK = 0b11111000;
  MyDelay(300);
 
  // LED segment C
  *portK = 0b01110010;
  MyDelay(300);
 
  // LED segment d
  *portK = 0b10111100;
  MyDelay(300);
 
  // LED segment E
  *portK = 0b11110010;
  MyDelay(300);
 
  // LED segment F
  *portK = 0b11100010;
  MyDelay(300);
 
}

// Delay Function
void MyDelay(unsigned long mSecondsApx) {
  volatile unsigned long i;
  unsigned long endTime = 1000 *mSecondsApx;
  
  for( i=0; i<endTime; i++);
}


